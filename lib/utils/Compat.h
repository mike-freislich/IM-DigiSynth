#pragma once
#include <Arduino.h>
//#include <stdlib.h>

template <typename T>
T clampf(T value, T minVal, T maxVal)
{    
    return max(minVal, min(value, maxVal));
}