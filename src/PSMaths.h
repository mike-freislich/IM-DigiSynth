#ifndef PS_MATHS_H
#define PS_MATHS_H

template <typename T>
T clamp(T value, T minVal, T maxVal) { return max(minVal, min(value, maxVal)); }


#endif