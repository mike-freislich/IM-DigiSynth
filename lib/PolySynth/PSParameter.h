#ifndef PS_PARAMETER_H
#define PS_PARAMETER_H

#include <Arduino.h>
#include "controls.h"
#include "PSMaths.h"
#include "PSParamaterTargets.h"
#include <vector>

class PSParameter;
typedef void (*ParameterTarget)(AudioStream *, float);
typedef std::vector<PSParameter *> PSParameterVector;

class PSParameter
{
public:
    String name;
    String units = "";

    PSParameter(String name = "", float value = 0, float min = 0, float max = 0, AudioStream *a = nullptr, ParameterTarget t = nullptr)
    {
        this->name = name;
        isTracking = false;
        setRange(min, max);
        _value = value;
        target = t;
        audioStream = a;        
    }

    void attachController(InputBase *controller) { _controller = controller; }
    void detachController() { _controller = nullptr; }

    // PSParameter::updateFromControl()
    // Stores the currently assigned controller value into the
    // parameter and sets the parameter state is didChange = true;
    bool updateFromControl(bool rawValue = false)
    {        
        if (_active && (_controller != nullptr))// && !_didChange)
        {
            _controller->update();
            float val = (rawValue) ? _controller->getRawValue() : _controller->getValue();
            float cvalScaled = mapf(val, _controller->getAnalogMin(), _controller->getAnalogMax(), _rangeMin, _rangeMax);                
            //Serial.println(cvalScaled);
            //Serial.printf("setting value for %s to %f\n", name.c_str(), cvalScaled);
            _didChange = setValue(cvalScaled);
            return _didChange;
        }        
        return false;
    }

    void setRange(float rangeMin, float rangeMax)
    {
        _rangeMin = rangeMin;
        _rangeMax = rangeMax;
        setValue(_value);
    }

    bool setValue(float value, bool force = false)
    {
        float inputPos = clamp(value, _rangeMin, _rangeMax);

        if (!force)
        {
            if (firstUpdate)
            {
                lastInputPos = inputPos;
                firstUpdate = false;
                _didChange = true;
                return true;
            }

            if (inputPos == lastInputPos)
                return false;
        }

        if (force || isTracking)
        {
            lastInputPos = inputPos;
            _value = inputPos;
        }
        else
        {
            if ((lastInputPos <= _value && inputPos >= _value) || (lastInputPos >= _value && inputPos <= _value))            
                isTracking = true;
            
            lastInputPos = inputPos;
        }
        _didChange = true;
        return true;
    }

    // PSParameter::didChange(bool reset)
    // set reset to true if you want to reset the didChange flag
    bool didChange(bool reset = false)
    {
        bool result = _didChange;
        if (result && reset)
            _didChange = false;
        return result;
    }

    float getValue() { return _value; }
    String displayValue() 
    {        
        int mult = (abs(_rangeMax - _rangeMin) == 1) ? 100 : 1;
        return String((int)(_value * mult));    
    }
    float getInputPos() { return lastInputPos; }

    // returns the value as number from 0.0f to 1.0f
    // which reflects the percentage of the range
    float getValueAsRangeRatio() { return (_rangeMax - _rangeMin) / (_value - _rangeMin); }

    // scales the value to an external range
    float getScaled(float scalemin, float scalemax) { return map(_value, _rangeMin, _rangeMax, scalemin, scalemax); }
    float getScaledInput(float scalemin, float scalemax) { return map(lastInputPos, _rangeMin, _rangeMax, scalemin, scalemax); }
    float getMax() { return _rangeMax; }
    float getMin() { return _rangeMin; }

    void activate()
    {
        firstUpdate = true;
        isTracking = false;
        _active = true;        
    }
    void deactivate() { _active = false; }
    void markChanged() { _didChange = true; }

    void callTarget() {

        if (audioStream != nullptr && target != nullptr && _didChange)
        {
            Serial.printf("Setting Target Parameter for '%s' to %2.2f\n", name.c_str(), _value);
            target(audioStream, _value);
                         
            //_didChange = false;
        }
    }

private:
    float _value, _rangeMin, _rangeMax;
    ParameterTarget target = nullptr;
    AudioStream *audioStream = nullptr;

    InputBase *_controller = nullptr;
    bool isTracking = false;
    float trackingTarget = 0;
    float lastInputPos = -1;
    bool _active = false;
    bool _didChange = false;
    bool firstUpdate = true;
};

#endif
