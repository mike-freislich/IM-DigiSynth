#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <vector>
#include "FastTrig.h"

class Parameter;

typedef void (*ParameterChangedCallback)(Parameter *, float);

class Parameter
{
public:
    String label, units;

    Parameter() {}

    Parameter(String label, String units, float value, float min = 0, float max = 100, bool asInteger = false)
    {
        _value = value;
        _min = min;
        _max = max;
        this->label = label;
        this->units = units;
        _defaultValue = value;
        _asInteger = asInteger;
    }

    float getMax() { return _max; }
    float getMin() { return _min; }

    // if the value has changed, return true
    bool setValue(float value)
    {
        // check bounds
        if (value > _max)
            value = _max;
        else if (value < _min)
            value = _min;

        // did value change?
        if (_value != value)
        {
            _value = value;
            return true; // changed
        }
        return false; // same as before
    }

    float getValue()
    {
        if (_asInteger)
            return (int)_value;

        return _value;
    }

    void setDefaultValue(float value) { _defaultValue = value; }
    void reset() { _value = _defaultValue; }

    int valueScaled(int rangeMin, int rangeMax)
    {
        return map(_value, _min, _max, rangeMin, rangeMax);
    }

private:
    float _value, _defaultValue, _max, _min;
    bool _asInteger;
};

typedef std::vector<Parameter *> ParameterVector;

class ParameterList
{
public:
    ParameterList(String name)
    {
        _name = name;
    }
    ~ParameterList() { _parms.clear(); }

    Parameter *add(Parameter *parm)
    {
        _parms.push_back(parm);
        return parm;
    }
    void clear() { _parms.clear(); }

    Parameter *getParm(String label)
    {
        for (auto p : _parms)
            if (p->label == label)
                return p;
        return nullptr;
    }

    Parameter *getParm(int atIndex) { return _parms[atIndex]; }

    int getLength()
    {
        return _parms.size();
    }

    String getData()
    {
        String s;
        for (auto p : _parms)
        {
            s = s + p->label + "," + String(p->getValue()) + "\n";
        }
        return s;
    }

protected:
    ParameterVector _parms;
    String _name;

private:
};

class EnvParameters : public ParameterList
{
public:
    Parameter *attack;
    Parameter *hold;
    Parameter *decay;
    Parameter *sustain;
    Parameter *release;

    EnvParameters(String name) : ParameterList(name)
    {
        attack = add(new Parameter("Atk", "ms", 0, 0, 2000, true));
        hold = add(new Parameter("Hold", "ms", 0, 0, 2000, true));
        decay = add(new Parameter("Dec", "ms", 133, 0, 2000, true));
        sustain = add(new Parameter("Sus", "%", 17, 0, 100));
        release = add(new Parameter("Rel", "ms", 0, 0, 2000, true));
    }
    ~EnvParameters()
    {
        ParameterList::~ParameterList();
    }

    void loadData(String data)
    {
    }
};

#endif