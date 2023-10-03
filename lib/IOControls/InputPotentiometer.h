#ifndef INPUT_POTENTIOMETER_H
#define INPUT_POTENTIOMETER_H

#include <InputBase.h>
#include <AnalogMux.h>

//#define NUMPOTS 4
#define POT_RESOLUTION 2
#define POT_RANGE_MIN 10
#define POT_RANGE_MAX 1014

enum PotTaper
{
    linear,
    audio
};

class Potentiometer : public InputBase
{
public:
    PotTaper taper = linear;

    Potentiometer() { controllerType = ControllerType::CT_POT; }
    Potentiometer(uint8_t pin, IOType ioType = IO_EXPANSION) : InputBase(pin)
    {
        controllerType = ControllerType::CT_POT;
        _analogMin = POT_RANGE_MIN;
        _analogMax = POT_RANGE_MAX;
    }
    ~Potentiometer() {}

    bool setValue(int value)
    {
        if (value >= _value + POT_RESOLUTION || value <= _value - POT_RESOLUTION)
        {
            _value = value;
            Serial.printf("POT %02d - value changed to %04d log(%04d)\n", _pin, _value, (int)getLogValue());
            return true;
        }
        return false;
    }

    float getValue() override
    {
        if (taper == audio)
            return getLogValue();

        return _value;
    }

    float getLogValue()
    {
        float result = (_value * (_value + 1)) >> ANALOG_READ_BITS;
        return result;
    }

    void update() override
    {
        int v = (ioType == IO_EXPANSION) ? analogMux.analogReadIO(_pin) : analogRead(_pin);
        setValue(v);
    }

protected:
};

#endif