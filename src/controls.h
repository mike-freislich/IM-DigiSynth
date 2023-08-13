#ifndef DS_CONTROLS_H
#define DS_CONTROLS_H

#include <Arduino.h>

#define NUMPOTS 4
#define POT_AVERAGE_SAMPLING 16
#define POT_RESOLUTION 4
#define POT_RANGE_MIN 0
#define POT_RANGE_MAX 980
#define PIN_POTA A0
#define PIN_POTB A1
#define PIN_POTC A2
#define PIN_POTD A3

#define NUMBUTTONS 1
#define PIN_ROTARY_BUTTONA A17

class InputBase
{
public:
    InputBase() {}
    InputBase(uint8_t pin, uint16_t debounceMillis = 10)
    {
        _pin = pin;
        _debounceMS = debounceMillis;
    }
    ~InputBase() {}

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

    uint8_t getPin() { return _pin; }

    // virtual bool setValue(int value);
    // virtual void update();
    // virtual float getValue();

protected:
    uint8_t _pin;
    int _value;
    uint16_t _debounceMS;
    uint32_t _lastChanged;
};

class Potentiometer : public InputBase
{
public:
    Potentiometer() {}
    Potentiometer(uint8_t pin) : InputBase(pin) {}
    ~Potentiometer() {}

    bool setValue(int value)
    {
        if (value > _value + POT_RESOLUTION || value < _value - POT_RESOLUTION)
        {
            _value = value;
            return true;
        }
        return false;
    }

    float getValue() { return _value; }

    long getLogValue()
    {
        return (_value * (_value + 1)) >> 10; // 10bit resolution
    }
    void update() { setValue(analogRead(_pin)); }

protected:
};

class Button : public InputBase
{
public:
    Button() : InputBase() {}
    Button(int pin, uint16_t debounceMillis = 10) : InputBase(pin, debounceMillis)
    {
        pinMode(pin, INPUT_PULLUP);
    }

    bool pressed()
    {
        return getValue();
    }

    float getValue()
    {
        return (_value == LOW);
    }

    bool setValue(int value)
    {
        if (value != _value && debounced())
        {
            Serial.println(value);
            _value = value;
            return true;
        }
        return false;
    }

    void update() { setValue(digitalRead(_pin)); }

protected:
};

class Controls
{
public:
    Potentiometer pots[NUMPOTS] = {
        Potentiometer(PIN_POTA),
        Potentiometer(PIN_POTB),
        Potentiometer(PIN_POTC),
        Potentiometer(PIN_POTD)};

    Button buttons[NUMBUTTONS] = {
        Button(PIN_ROTARY_BUTTONA)};

    Controls() { analogReadAveraging(POT_AVERAGE_SAMPLING); }

    void update()
    {
        updatePots();
        updateButtons();
    }

protected:
    void updatePots()
    {
        for (int i = 0; i < NUMPOTS; i++)
            pots[i].update();
    }
    void updateButtons()
    {
        for (int i = 0; i < NUMBUTTONS; i++)
            buttons[i].update();
    }
};

Controls controls;

void testControls()
{
}
#endif