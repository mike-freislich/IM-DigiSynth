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

class Potentiometer : public InputBase
{
public:
    Potentiometer()
    {
        controllerType = ControllerType::CT_POT;
    }
    Potentiometer(uint8_t pin) : InputBase(pin)
    {
        _analogMin = POT_RANGE_MIN;
        _analogMax = POT_RANGE_MAX;
        controllerType = ControllerType::CT_POT;
    }
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

    float getValue() override { return (float)getLogValue(); }
    long getLogValue() { return (_value * (_value + 1)) >> 10; } // 10bit resolution
    void update() override { setValue(analogRead(_pin)); }

protected:
};

class Button : public InputBase
{
public:
    Button() : InputBase()
    {
        controllerType = ControllerType::CT_BTN;
        _value = 1;
    }
    Button(int pin, uint16_t debounceMillis = 10) : InputBase(pin, debounceMillis)
    {
        pinMode(pin, INPUT_PULLUP);
        controllerType = ControllerType::CT_BTN;
        _value = 1;
    }

    bool pressed() { return getValue(); }
    float getValue() override { return (_value == LOW); }

    bool didLongPress(uint16_t duration = 1000)
    {
        //Serial.printf("didLongPress %d\n", duration);
        if (_didRelease)
        {
            _didRelease = false;
            Serial.println("didRelease");
            return (releaseDuration >= duration);
        }
        return false;
    }

    bool setValue(int value)
    {
        if (value != _value && debounced())
        {

            //Serial.println(value);
            _value = value;
            if (pressed())
            {
                //Serial.println("pressed");
                pressedTime = millis();
                releaseDuration = 0;
                _didRelease = false;
            }
            else
            {

                releaseDuration = millis() - pressedTime;
                //Serial.printf("released after %d\n", releaseDuration);
                _didRelease = true;
            }

            return true;
        }
        return false;
    }

    void update() override { setValue(digitalRead(_pin)); }

protected:
    uint32_t pressedTime = 0;
    uint32_t releaseDuration = 0;
    bool _didRelease = false;
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

#endif