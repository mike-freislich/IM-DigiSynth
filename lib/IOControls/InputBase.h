#ifndef INPUT_BASE_H
#define INPUT_BASE_H

#include <Arduino.h>
#include <SimpleTimer.h>
#define ANALOG_READ_BITS 10
#define POT_AVERAGE_SAMPLING 32

enum ControllerType
{
    CT_POT,
    CT_BTN,
    CT_ROTARY,
    CT_NONE
};

enum IOType
{
    IO_TEENSY,
    IO_EXPANSION
};

class InputBase
{
public:   
    InputBase() {}
    InputBase(uint8_t pin, IOType ioType = IO_EXPANSION, uint16_t debounceMillis = 10): _pin(pin), ioType(ioType)
    {
        analogReadResolution(ANALOG_READ_BITS);
        analogReadAveraging(POT_AVERAGE_SAMPLING);             
        debounceTimer.setDuration(debounceMillis);
        _analogMin = -1;
        _analogMax = -1;
    }
    ~InputBase() {}

    ControllerType getType() { return controllerType; }

    void setDebounceTime(uint16_t ms = 10) { debounceTimer.setDuration(ms); }

    void setIoType(IOType ioType) { this->ioType = ioType; }

    bool debounced()
    {
        if (debounceTimer.update())
            return true;
        else
            return false;
    }

    virtual void update() {}
    virtual float getValue() { return _value; }
    virtual float getRawValue() { return _value; }
    virtual void setValue(float value) { _value = value; }

    uint8_t getPin() { return _pin; }
    float getAnalogMin() { return _analogMin; }
    float getAnalogMax() { return _analogMax; }

protected:
    uint8_t _pin;
    int _value, _analogMin, _analogMax;
    ControllerType controllerType = CT_NONE;
    SimpleTimer debounceTimer;
    IOType ioType = IO_EXPANSION;
};

#endif