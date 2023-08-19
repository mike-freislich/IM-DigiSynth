#ifndef PS_PARAMETER_H
#define PS_PARAMETER_H

#include <Arduino.h>

class PSParameter
{
public:
    String name;
    float value;
    float lastValue;
    PSParameter(String name = "", float value = 0)
    {
        this->name = name;
        this->value = value;
        this->lastValue = value;
    }
};

#endif
