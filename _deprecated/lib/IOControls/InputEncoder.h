#ifndef INPUT_ENCODER_H
#define INPUT_ENCODER_H

#include <InputBase.h>
#include "PSMaths.h"

static const PROGMEM int8_t enc_states[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0}; // Lookup table

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
            // Serial.printf("range set : min %d, max %d\n", _analogMin, _analogMax);
            // Serial.print("Index:");
            // Serial.println(_value, DEC);
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
// static void encoder2ISRA();
// static void encoder2ISRB();

#endif