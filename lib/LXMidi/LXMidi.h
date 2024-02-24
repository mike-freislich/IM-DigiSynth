#pragma once
#include <usb_midi.h>
#include "MidiTables.h"

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

#define BUFFER_SIZE 64
class LXMidi
{
public:
    LXMidi()
    {
        clearBuffer();
    }
    byte lastNote = 0, afterTouch = 0;
    int pitchBend = 0;
    void update() { usbMIDI.read(); }

    byte pressedCount = 0;

    // Function to add a MIDI note to the buffer
    void addNote(int8_t note)
    {
        _notebuffer[head] = note;        // Add the note at the head index
        head = (head + 1) % BUFFER_SIZE; // Move head circularly
        if (size < BUFFER_SIZE)
        {
            ++size; // Increase size if not full
        }
        else
        {
            tail = (tail + 1) % BUFFER_SIZE; // Move tail if buffer is full
        }
    }

    int8_t releaseNote(int8_t note)
    {
        for (int i = 0; i < BUFFER_SIZE; ++i)
        {
            if (_notebuffer[i] == note)
            {
                _notebuffer[i] = -1;
                break;
            }
        }

        int8_t highestPitch = _notebuffer[0];
        for (int8_t i = 0; i < BUFFER_SIZE; i++)
        {
            if (_notebuffer[i] > highestPitch)
                highestPitch = _notebuffer[i];
        }

        return highestPitch;
    }

    // Function to print the buffer
    void printBuffer()
    {
        int index = tail;
        for (int i = 0; i < BUFFER_SIZE; ++i)
        {
            //_notebuffer[index];
            index = (index + 1) % BUFFER_SIZE;
        }
    }

    int notesCount()
    {
        int count = 0;
        for (int i = 0; i < BUFFER_SIZE; i++)
            if (_notebuffer[i] > -1)
                count++;
        return count;
    }

private:
    int8_t head = 0, tail = 0, size = 0;
    int8_t _notebuffer[BUFFER_SIZE];
    void clearBuffer()
    {
        for (uint8_t i = 0; i < BUFFER_SIZE; i ++)
        {
            _notebuffer[i] = -1;
        }
    }
} midi;

void initMidi()
{
    LOG("[INIT] Midi");
    usbMIDI.setHandleClock(myClock);
    usbMIDI.setHandleNoteOn(myNoteOn);
    usbMIDI.setHandleNoteOff(myNoteOff);
    usbMIDI.setHandlePitchChange(myPitchChange);
    usbMIDI.setHandleAfterTouch(myAfterTouch);
    usbMIDI.setHandleControlChange(myControlChange);
}