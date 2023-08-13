#ifndef DS_SEQUENCER_H
#define DS_SEQUENCER_H

#include <Arduino.h>

#pragma region miditracks
// https://docs.oracle.com/javase%2Ftutorial%2F/sound/MIDI-seq-intro.html
// PPQ (pulse per quarter) formula
// ticksPerSecond =
//     resolution * (currentTempoInBeatsPerMinute / 60.0);
// tickSize = 1.0 / ticksPerSecond;
//
// and this formula in the case of SMPTE:
//
// framesPerSecond =
//   (divisionType == Sequence.SMPTE_24 ? 24
//     : (divisionType == Sequence.SMPTE_25 ? 25
//       : (divisionType == Sequence.SMPTE_30 ? 30
//         : (divisionType == Sequence.SMPTE_30DROP ?
//
//             29.97))));
// ticksPerSecond = resolution * framesPerSecond;
// tickSize = 1.0 / ticksPerSecond;

/*
enum TrackEventType // 2 bits
{
    noteOn = 0b01,
    noteOff = 0b00,
    pitchBend = 0b10
};

struct TrackEvent
{
    TrackEvent *previousEvent = nullptr;
    TrackEvent *nextEvent = nullptr;
    uint32_t deltaTime;
    TrackEventType eventType;
    uint32_t data;
    uint8_t getNote()
    {
        return (data && 0b00000000000000000000000001111111) << 25;
    }
    uint8_t getVelocity()
    {
        return ((data && 0b000000000000000000111111110000000) >> 7) << 25;
    }
};

class TrackEventList
{
public:
    void append(TrackEvent *event)
    {
        if (!_count)
        {
            _head = event;
            _tail = event;
        }
        else
        {
            _tail->nextEvent = event;
            event->previousEvent = _tail;
            _tail = event;
            _count++;
        }
    }

    void insert(TrackEvent *event, int atIndex)
    {
        if (atIndex + 1 > _count)
            return;

        TrackEvent *e = _head;
        for (int i = 0; i <= atIndex; i++)
            e = e->nextEvent;

        TrackEvent *prev = e->previousEvent;
        event->nextEvent = e;
        e->previousEvent = event;
        prev->nextEvent = event;
    }

protected:
    int _count = 0;
    TrackEvent *_head = nullptr;
    TrackEvent *_tail = nullptr;
};

class Track
{
public:
    String name;
    int channel;
    TrackEventList events;
};

void testTracks()
{
    Track track;
    TrackEvent event;
    event.eventType = noteOn;
    track.events.append(&event);
}

*/
#pragma endregion

#define SEQ_GATE_MS 64
#define SEQ_TRACKS 4
#define SEQ_STEPS 16
#define SEQ_TEMPO 30

const float SeqDivValues[13] PROGMEM = {
    2.00000000f,
    1.50000000f,
    1.33333337f,
    1.00000000f,
    0.75000000f,
    0.66666669f,
    0.50000000f,
    0.37500000f,
    0.33333334f,
    0.25000000f,
    0.16666667f,
    0.12500000f,
    0.08333334f};

enum SeqPlayMode
{
    forward,
    reverse,
    chaos,
    pingpong
};

class SeqStep
{
public:
    uint8_t velocity;
    uint8_t midiNote;
    uint8_t tie;
};

class SeqTrack
{
public:
    SeqStep steps[SEQ_STEPS];
};

class SeqPattern
{
public:
    SeqPattern()
    {
        _length = SEQ_STEPS;
        _playMode = SeqPlayMode::forward;
        _name = "unsaved";
    }

    uint8_t getLength() { return _length; }

    void load()
    {
        // TODO : load from a file  
        for (int t = 0; t < SEQ_TRACKS; t++)
        {
            for (int s = 0; s < SEQ_STEPS; s++)
            {
                SeqStep *step = &(_tracks[t].steps[s]);

                step->midiNote = random(64) + 12;
                step->velocity = (random(64) + 64 - 1);
                step->tie = false;
            }
        }
    }

    void getNotesAtStep(int stepIndex, SeqStep *notes[SEQ_TRACKS])
    {
        for (int i = 0; i < SEQ_TRACKS; i++)
        {
            notes[i] = &(_tracks[i].steps[stepIndex]);
        }
    }

private:
    String _name;
    uint8_t _length;
    SeqTrack _tracks[SEQ_TRACKS];
    SeqPlayMode _playMode;
};

class Sequencer
{
public:
    Sequencer(void (*playStepCallback)(SeqStep *steps[SEQ_TRACKS]))
    {
        _timer = 0;
        _pattern = new SeqPattern();
        setTempo(SEQ_TEMPO);
        loadPattern("foo");
        _playStepCallback = playStepCallback;
    }
    ~Sequencer() {}

    void loadPattern(String name)
    {
        if (_pattern != nullptr)
        {
            _pattern->load();
        }
        else
        {
            _pattern = new SeqPattern();
            _pattern->load();
        }
    }

    void savePattern(int index, String name)
    {
    }

    void play()
    {
        _isPlaying = true;
        _timer = 0;
    }

    void stop() { _isPlaying = false; }

    void restart() { _index = 0; }

    void setTempo(float tempo, uint8_t div = 3)
    {        
        _tempo = tempo;
        _seqDiv = div;
        _stepMicros = 60000000 / _tempo / _seqDiv;
    }

    void update()
    {
        if (_isPlaying && _timer >= _stepMicros)
        {
            _timer = 0;            
            // Serial.print("index ");
            // Serial.print(_index);
            // Serial.print(" :");
            _index = (_index + 1) % _pattern->getLength();
            SeqStep *steps[SEQ_TRACKS];
            _pattern->getNotesAtStep(_index, steps);
            _playStepCallback(steps);
        }
    }

private:
    void (*_playStepCallback)(SeqStep *_steps[SEQ_TRACKS]);
    bool _isPlaying;
    elapsedMicros _timer;
    SeqPattern *_pattern;
    uint8_t _index;
    float _tempo = SEQ_TEMPO;
    float _seqDiv = SeqDivValues[3];
    uint32_t _stepMicros;
};

#endif