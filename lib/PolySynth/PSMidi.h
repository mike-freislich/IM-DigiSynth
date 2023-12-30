#ifndef PS_MIDI_H
#define PS_MIDI_H

#include <Arduino.h>
#include <usb_midi.h>
#include <SimpleTimer.h>

void myNoteOn(byte channel, byte note, byte velocity);
void myNoteOff(byte channel, byte note, byte velocity);
// void myAfterTouchPoly(byte channel, byte note, byte velocity);
// void myControlChange(byte channel, byte control, byte value);
// void myProgramChange(byte channel, byte program);
// void myAfterTouch(byte channel, byte pressure);
// void myPitchChange(byte channel, int pitch);
// void mySystemExclusiveChunk(const byte *data, uint16_t length, bool last);
// void mySystemExclusive(byte *data, unsigned int length);
// void myTimeCodeQuarterFrame(byte data);
// void mySongPosition(uint16_t beats);
// void mySongSelect(byte songNumber);
// void myTuneRequest();
void myClock();
// void myStart();
// void myContinue();
// void myStop();
// void myActiveSensing();
// void mySystemReset();
// void myRealTimeSystem(byte realtimebyte);

enum ClockDiv
{
    quarter = 24,
    eighth = 12,
    sixteenth = 6,
    thirtysecond = 3
};

//volatile uint32_t lastClock;
//volatile int lastTempo = 120;
volatile int MidiCLKcount;

void myClock()
{

    MidiCLKcount++;

    if (MidiCLKcount == 1)
    {
        //lastClock = millis();
        // Serial.println("midi beat");
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

void myNoteOn(byte channel, byte note, byte velocity)
{
    Serial.print("NOTE-ON : channel ");
    Serial.print(channel);
    Serial.print(", note ");
    Serial.print(note);
    Serial.print(", velocity ");
    Serial.println(velocity);
}

void myNoteOff(byte channel, byte note, byte velocity)
{
    Serial.print("NOTE-OFF : channel ");
    Serial.print(channel);
    Serial.print(", note ");
    Serial.print(note);
    Serial.print(", velocity ");
    Serial.println(velocity);
}

// typedef void (*PlayNoteCallBack)(byte, byte);
// class PSMidi
// {
// public:
//     PlayNoteCallBack playNote;
//     PSMidi(PlayNoteCallBack callback)
//     {
//         playNote = callback;
//     }

// private:
// };

#endif