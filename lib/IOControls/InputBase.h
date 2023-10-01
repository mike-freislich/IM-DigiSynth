#ifndef INPUT_BASE_H
#define INPUT_BASE_H

#include <Arduino.h>
#define ANALOG_READ_BITS 10

enum ControllerType
{
    CT_POT,
    CT_BTN,
    CT_ROTARY,
    CT_NONE
};

class InputBase
{
public:
    InputBase() {}
    InputBase(uint8_t pin, uint16_t debounceMillis = 10)
    {
        analogReadResolution(ANALOG_READ_BITS);
        _pin = pin;
        _debounceMS = debounceMillis;
        _analogMin = -1;
        _analogMax = -1;
    }
    ~InputBase() {}

    ControllerType getType() { return controllerType; }

    void setDebounceTime(uint16_t ms = 10)
    {
        _debounceMS = ms;
    }

    bool debounced()
    {
        uint32_t now = millis();
        if (now - _lastChanged > _debounceMS)
        {
            _lastChanged = now;
            return true;
        }
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
    uint16_t _debounceMS;
    uint32_t _lastChanged;
    ControllerType controllerType = CT_NONE;
};


#endif