#ifndef DS_CONTROLS_H
#define DS_CONTROLS_H

#include <Arduino.h>
#include "PSMaths.h"

#define NUMPOTS 4
#define POT_AVERAGE_SAMPLING 16
#define POT_RESOLUTION 4
#define POT_RANGE_MIN 0
#define POT_RANGE_MAX 980
#define PIN_POTA A3
#define PIN_POTB A2
#define PIN_POTC A0
#define PIN_POTD A1
#define NUMBUTTONS 4
#define PIN_TACTBUTTON_1 A8
#define PIN_TACTBUTTON_2 A9
#define PIN_ROTARY1_BUTTON A16
#define PIN_ROTARY2_BUTTON A17
#define NUMENCODERS 2
#define PIN_ENC1_A A5
#define PIN_ENC1_B A4
#define PIN_ENC2_A A7
#define PIN_ENC2_B A6

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

    float getValue() override { return getLogValue(); }    
    float getLogValue() { return (float)((_value * (_value + 1)) >> 10); } // 10bit resolution
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

    FLASHMEM bool didLongPress(uint16_t duration = 1000)
    {
        // Serial.printf("didLongPress %d\n", duration);
        if (_didRelease)
        {
            _didRelease = false;
            Serial.println("didRelease");
            return (releaseDuration >= duration);
        }
        return false;
    }

    FLASHMEM bool setValue(int value)
    {
        if (value != _value && debounced())
        {
            _value = value;
            if (pressed())
            {
                pressedTime = millis();
                releaseDuration = 0;
                _didRelease = false;
            }
            else
            {
                releaseDuration = millis() - pressedTime;
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

static const int8_t enc_states[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0}; // Lookup table

class Encoder : public InputBase
{
public:
    Encoder(uint8_t pinA, uint8_t pinB, void (*isrA)(), void (*isrB)())
    {
        controllerType = ControllerType::CT_ROTARY;
        _pinA = pinA;
        _pinB = pinB;
        pinMode(pinA, INPUT_PULLUP);
        pinMode(pinB, INPUT_PULLUP);
        attachInterrupt(_pinA, isrA, CHANGE);
        attachInterrupt(_pinB, isrB, CHANGE);
    }

    void setRange(int rangeMin, int rangeMax)
    {
        _value = clamp(_value, rangeMin, rangeMax);
        _analogMax = rangeMax;
        _analogMin = rangeMin;        
    }

    int getPosition()
    {
        return _value;
    }

    void isrA()
    {
        if (rotating)
            delayMicroseconds(1000);
        if (digitalRead(_pinA) != encA_set)
        {
            encA_set = !encA_set;
            if (encA_set && !encB_set)
                counter++;
            rotating = false;
        }
    }
    
    void isrB()
    {
        if (rotating)
            delayMicroseconds(1000);
        if (digitalRead(_pinB) != encB_set)
        {
            encB_set = !encB_set;
            if (encB_set && !encA_set)
                counter--;
            rotating = false;
        }
    }

    void setValue(float value) override
    {
        _value = value;        
    }

    void update()
    {
        rotating = true; // reset the debouncer
        if (counter != lastCounter)
        {
            if (counter > lastCounter)
                _value++;
            else if (counter < lastCounter)
                _value--;

            if (_analogMax != _analogMin)
                _value = clamp(_value, _analogMin, _analogMax);
            //Serial.printf("range set : min %d, max %d\n", _analogMin, _analogMax);
            //Serial.print("Index:");
            //Serial.println(_value, DEC);
            lastCounter = counter; 
        }
    }

protected:
    uint8_t _pinA, _pinB;
    const uint32_t _pauseLength = 100000;
    volatile uint32_t _lastISRChanged = 0;
    volatile int counter = 0;
    volatile bool encA_set, encB_set, rotating;
    int lastCounter = 0;
};

static void encoder1ISRA();
static void encoder1ISRB();
static void encoder2ISRA();
static void encoder2ISRB();

class Controls
{
public:
    Potentiometer pots[NUMPOTS] = {
        Potentiometer(PIN_POTA),
        Potentiometer(PIN_POTB),
        Potentiometer(PIN_POTC),
        Potentiometer(PIN_POTD)};

    Button buttons[NUMBUTTONS] = {
        Button(PIN_ROTARY1_BUTTON),
        Button(PIN_ROTARY2_BUTTON),
        Button(PIN_TACTBUTTON_1),
        Button(PIN_TACTBUTTON_2)};

    Encoder encoders[NUMENCODERS] = {
        Encoder(PIN_ENC1_A, PIN_ENC1_B, encoder1ISRA, encoder1ISRB),
        Encoder(PIN_ENC2_A, PIN_ENC2_B, encoder2ISRA, encoder2ISRB)};

    Controls() { analogReadAveraging(POT_AVERAGE_SAMPLING); }

    void update()
    {
        updatePots();
        updateButtons();
        updateEncoders();
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
    void updateEncoders()
    {
        for (int i = 0; i < NUMENCODERS; i++)
            encoders[i].update();
    }
};

Controls controls;
static void encoder1ISRA() { controls.encoders[0].isrA(); }
static void encoder1ISRB() { controls.encoders[0].isrB(); }
static void encoder2ISRA() { controls.encoders[1].isrA(); }
static void encoder2ISRB() { controls.encoders[1].isrB(); }

#endif