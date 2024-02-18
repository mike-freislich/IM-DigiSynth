#ifndef PS_MIDI_H
#define PS_MIDI_H

#include <Arduino.h>
#include <usb_midi.h>
#include <SimpleTimer.h>
#include <PolySynth.h>

void myNoteOn(byte channel, byte note, byte velocity);
void myNoteOff(byte channel, byte note, byte velocity);
void myClock();
void myControlChange(byte channel, byte control, byte value);
void myAfterTouch(byte channel, byte pressure);
void myPitchChange(byte channel, int pitch);
//  void myAfterTouchPoly(byte channel, byte note, byte velocity);
//  void myProgramChange(byte channel, byte program);
//  void mySystemExclusiveChunk(const byte *data, uint16_t length, bool last);
//  void mySystemExclusive(byte *data, unsigned int length);
//  void myTimeCodeQuarterFrame(byte data);
//  void mySongPosition(uint16_t beats);
//  void mySongSelect(byte songNumber);
//  void myTuneRequest();
//  void myStart();
//  void myContinue();
//  void myStop();
//  void myActiveSensing();
//  void mySystemReset();
//  void myRealTimeSystem(byte realtimebyte);

enum ClockDiv
{
    quarter = 24,
    eighth = 12,
    sixteenth = 6,
    thirtysecond = 3
};

PolySynth *midiSynth = nullptr;

class PSMidi
{
public:
    PSMidi() {}

    byte lastNote = 0, afterTouch = 0;
    int pitchBend = 0;

    void begin(PolySynth *synth)
    {
        this->synth = synth;
        midiSynth = synth;
        usbMIDI.setHandleClock(myClock);
        usbMIDI.setHandleNoteOn(myNoteOn);
        usbMIDI.setHandleNoteOff(myNoteOff);
        usbMIDI.setHandlePitchChange(myPitchChange);
        usbMIDI.setHandleAfterTouch(myAfterTouch);
        usbMIDI.setHandleControlChange(myControlChange);
    }

    void update()
    {
        usbMIDI.read();
    }

private:
    PolySynth *synth;
} psMidi;

void myNoteOn(byte channel, byte note, byte velocity)
{
    if (psMidi.lastNote != note)
    {
        midiSynth->playNote(note, velocity);
        psMidi.lastNote = note;
        printf("Note : channel(%d), note(%d), velocity(%d)\n", channel, note, velocity);
    }
}

void myNoteOff(byte channel, byte note, byte velocity)
{
    if (psMidi.lastNote == note)
    {
        midiSynth->stopNote();
        psMidi.lastNote = 0;
    }
}

void myClock()
{
    static int MidiCLKcount;
    MidiCLKcount++;

    if (MidiCLKcount == 1)
    {
        // lastClock = millis();
        //  Serial.println("midi beat");
    }
    // else if (MidiCLKcount == 2)
    // {
    //     // uint32_t now = millis();
    //     // uint32_t elapsed = now - lastClock;
    //     // uint32_t millisPerBeat = elapsed * (uint8_t)ClockDiv::quarter;
    //     // Serial.print("elapsed : ");
    //     // Serial.println(elapsed);
    //     // float bpm = 1.0f / (millisPerBeat / 1000) * 60;
    //     // if ((int)bpm != lastTempo)
    //     // {
    //     //     // lastTempo = (int)bpm;
    //     //     // Serial.print("tempo changed : ");
    //     //     // Serial.println(bpm);
    //     // }
    // }

    if (MidiCLKcount >= ClockDiv::quarter)
    {
        MidiCLKcount = 0;
    }
}

void myPitchChange(byte channel, int pitch)
{
    psMidi.pitchBend = pitch;
    //printf("Pitch Bend : channel(%d), pitch(%d)\n", channel, pitch);
}

void myAfterTouch(byte channel, byte pressure)
{
    psMidi.afterTouch = pressure;
    //printf("AfterTouch : channel(%d), pressure(%d)\n", channel, pressure);
}

void myControlChange(byte channel, byte control, byte value)
{
    //printf("Midi CC chan [%d], cc [%d] : value(%d)\n", channel, control, value);
}

#endif