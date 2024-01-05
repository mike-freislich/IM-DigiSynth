#ifndef PS_PARAMETER_H
#define PS_PARAMETER_H

#include <Arduino.h>
#include "controls.h"
#include "PSMaths.h"
#include <vector>

class PSParameter;
class PSComponent;
typedef void (*ParameterTarget)(AudioStream *, float);
typedef void (*ParameterTargetC)(PSComponent *, float);
typedef std::vector<PSParameter *> PSParameterVector;

enum PSDisplayMode
{
    asINTEGER,
    asDECIMAL,
    asPERCENTAGE
};

class PSParameter
{
public:
    String name;
    String units = "";
    String alt = "";
    PSDisplayMode displayMode = asINTEGER;
    PotTaper potTaper = PotTaper::linear;

    PSParameter() {}
    PSParameter(PSComponent *c, ParameterTargetC t = nullptr)
    {
        targetC = t;
        component = c;
    }

    PSParameter(AudioStream *a, ParameterTarget t = nullptr)
    {
        target = t;
        audioStream = a;
    }

    PSParameter *init(String name, float value = 0, float min = 0, float max = 0, PSDisplayMode displayMode = asINTEGER)
    {
        this->displayMode = displayMode;
        this->name = name;
        isTracking = false;
        setRange(min, max);
        _value = value;
        return this;
    }

    void attachController(InputBase *controller) { _controller = controller; }
    void attachController(Potentiometer *pot)
    {
        _controller = pot;
        pot->taper = potTaper;
    }
    void detachController() { _controller = nullptr; }

    // PSParameter::updateFromControl()
    // Stores the currently assigned controller value into the
    // parameter and sets the parameter state is didChange = true;
    bool updateFromControl(bool rawValue = false)
    {
        if (_active && (_controller != nullptr))
        {
            _controller->update();
            float val = (rawValue) ? _controller->getValue() : _controller->getValue();
            float cvalScaled = mapf(val, _controller->getAnalogMin(), _controller->getAnalogMax(), _rangeMin, _rangeMax);
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

        if (_rangeMin == _rangeMax)
            return ".";

        String result = "";

        switch (displayMode)
        {
        case asDECIMAL:
            result = (_value < 10) ? String((float)(getValue())) : String((int)getValue());
            break;
        case asINTEGER:
            result = String((int)getValue());
            break;
        case asPERCENTAGE:
            result = String((int)(getValueAsRangeRatio() * 100));
            break;
        }
        // int mult = (abs(_rangeMax - _rangeMin) == 1) ? 100 : 1;
        // return String((float)(_value)); // * mult));
        return result;
    }
    float getInputPos() { return lastInputPos; }

    // returns the value as number from 0.0f to 1.0f
    // which reflects the percentage of the range
    float getValueAsRangeRatio() { return mapf(_value, _rangeMin, _rangeMax, 0.0f, 1.0f); }

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

    void callTarget()
    {
        if (_didChange)
        {

            if (target != nullptr && audioStream != nullptr)
            {
                // if (potTaper == audio)
                //     Serial.println("Audio Taper");
                // Serial.printf("Setting Target AudioComp for '%s' to %2.2f\n", name.c_str(), getValue());
                target(audioStream, getValue());
                _didChange = false;
            }
            else if (targetC != nullptr && component != nullptr)
            {
                // if (potTaper == audio)
                //     Serial.println("Audio Taper");
                // Serial.printf("Setting Target PSComp for '%s' to %2.2f\n", name.c_str(), getValue());
                targetC(component, getValue());
                _didChange = false;
            }
        }
    }

private:
    float _value, _rangeMin, _rangeMax;
    ParameterTarget target = nullptr;
    ParameterTargetC targetC = nullptr;
    AudioStream *audioStream = nullptr;
    PSComponent *component = nullptr;

    InputBase *_controller = nullptr;
    bool isTracking = false;
    float trackingTarget = 0;
    float lastInputPos = -1;
    bool _active = false;
    bool _didChange = false;
    bool firstUpdate = true;
};

#endif
