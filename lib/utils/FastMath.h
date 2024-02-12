#pragma once
//#include <iostream>
//#include <cmath>
#include "FastSine.h"

//using namespace std;

// Number of entries in the logarithm table
const int TABLE_SIZE = 64;

// Function to create a static logarithm table
const float *createStaticLogTable(float min, float max, int size)
{
    static float logTable[TABLE_SIZE];

    float step = (max - min) / static_cast<float>(size - 1);

    for (int i = 0; i < size; ++i)
    {
        float value = min + step * static_cast<float>(i);
        logTable[i] = std::log(value);
    }

    return logTable;
}

// // Function to perform a logarithmic lookup with linear interpolation
// float fastLogLookup(float x, const float *logTable, float min, float max, int size)
// {
//     if (x <= 0.0f)
//         return -std::numeric_limits<float>::infinity();
//     if (x >= max)
//         return logTable[size - 1];

//     float indexFloat = (x - min) / (max - min) * (size - 1);
//     int indexLow = static_cast<int>(indexFloat);
//     int indexHigh = indexLow + 1;

//     // Perform linear interpolation
//     float frac = indexFloat - indexLow;
//     float logLow = logTable[indexLow];
//     float logHigh = logTable[indexHigh];
//     return (1.0f - frac) * logLow + frac * logHigh;
// }

float convertToAudio(float linearValue, float linearMin, float linearMax, float audioMin = 0, float audioMax = 0, float exponent = 2.0f)
{
    if (audioMax == audioMin)
    {
        audioMax = linearMax;
        audioMin = linearMin;
    }
    // Ensure linearValue is within the specified range
    linearValue = max(min(linearValue, linearMax), linearMin);

    // Perform the conversion using the formula
    float audioValue = audioMin + (audioMax - audioMin) * std::pow((linearValue - linearMin) / (linearMax - linearMin), exponent);

    return audioValue;
}

// // Fast sine approximation using a polynomial
// float fastSin(float x)
// {
//     // Map x to the range [-pi, pi]
//     x = std::fmod(x + M_PI, 2.0f * M_PI) - M_PI;
//     // Polynomial coefficients
//     float x2 = x * x;
//     float x3 = x2 * x;
//     float x5 = x3 * x2;
//     float result = x * (1.0f - x2 / 6.0f + x5 / 120.0f);
//     return result;
// }

float sineWavePoint(float amplitude, float frequency, float phaseOffset, uint32_t numMilliseconds)
{
    const float twoPi = 2.0f * 3.14159f;
    double timeInSeconds = static_cast<double>(numMilliseconds) / 1000.0;
    double phase = twoPi * frequency * timeInSeconds + twoPi * phaseOffset;
    return amplitude * sinewave(phase);
}