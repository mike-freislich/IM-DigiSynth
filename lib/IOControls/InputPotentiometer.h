#ifndef INPUT_POTENTIOMETER_H
#define INPUT_POTENTIOMETER_H

#include <InputBase.h>

#define NUMPOTS 4
#define POT_AVERAGE_SAMPLING 16
#define POT_RESOLUTION 4

#define POT_RANGE_MIN 10
#define POT_RANGE_MAX 980

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

    float getValue() override
    {
        if (taper == audio)
            return getLogValue();

        return _value;
    }
    float getLogValue()
    {        
        // const float minLogValue = log10(1); 
        // const float maxLogValue = log10(POT_RANGE_MAX);
        // float logValue = minLogValue + (maxLogValue - minLogValue) * ((float)(_value) / POT_RANGE_MAX);        
        // float audioTaperValue = (pow(10, logValue))-1;
        // Serial.printf("POT AUDIO TAPER %2.2f\n", audioTaperValue);        
        // return audioTaperValue;
        float result =(_value * (_value + 1)) >> ANALOG_READ_BITS;
        //Serial.printf("POT AUDIO TAPER %2.2f\n", result);
        return result;
    }
    void update() override { setValue(analogRead(_pin)); }

protected:
};


#endif