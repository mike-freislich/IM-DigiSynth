#ifndef PS_PARAMETER_H
#define PS_PARAMETER_H

#include <Arduino.h>
#include "controls.h"
#include "PSMaths.h"
#include <vector>

class PSParameter;
typedef vector<PSParameter *> PSParameterVector;

class PSParameter
{
public:
    String name;
    String units = "";

    PSParameter(String name = "", float value = 0, float min = 0, float max = 0)
    {
        this->name = name;

        isTracking = false;
        setRange(min, max);
        _value = value;
    }

    void attachController(InputBase *controller) { _controller = controller; }
    void detachController() { _controller = nullptr; }

    /* PSParameter::update()
     * Stores the currently assigned controller value into the
     * parameter and sets the parameter state is didChange = true;
     */
    bool update()
    {
        if (_active && (_controller != nullptr) && !_didChange)
        {
            _controller->update();
            float cvalScaled = map(_controller->getValue(), _controller->getAnalogMin(), _controller->getAnalogMax(), _rangeMin, _rangeMax);

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
            {
                isTracking = true;
            }
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
    float getInputPos() { return lastInputPos; }
    float getValueAsPercent() { return (_rangeMax - _rangeMin) / (_value - _rangeMin) * 100; }
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

private:
    float _value, _rangeMin, _rangeMax;

    InputBase *_controller = nullptr;
    bool isTracking = false;
    float trackingTarget = 0;
    float lastInputPos = -1;
    bool _active = false;
    bool _didChange = false;
    bool firstUpdate = true;
};

#endif
