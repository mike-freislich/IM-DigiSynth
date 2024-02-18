#pragma once
#include <usb_midi.h>

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

class LXMidi
{
public:
    byte lastNote = 0, afterTouch = 0;
    int pitchBend = 0;
    void update() { usbMIDI.read(); }
} midi;

void initMidi()
{
    usbMIDI.setHandleClock(myClock);
    usbMIDI.setHandleNoteOn(myNoteOn);
    usbMIDI.setHandleNoteOff(myNoteOff);
    usbMIDI.setHandlePitchChange(myPitchChange);
    usbMIDI.setHandleAfterTouch(myAfterTouch);
    usbMIDI.setHandleControlChange(myControlChange);
}