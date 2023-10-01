#ifndef PS_MATHS_H
#define PS_MATHS_H

template <typename T>
T clamp(T value, T minVal, T maxVal) { return max(minVal, min(value, maxVal)); }


float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


#endif