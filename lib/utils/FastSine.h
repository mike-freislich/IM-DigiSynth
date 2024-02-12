//#include <iostream>
//#include <cmath>

constexpr int tableSize = 360;
constexpr double pi = 3.14159265358979323846f;

const double *getStaticSinTable()
{
    static double sinTable[tableSize];
    static bool initialized = false;

    if (!initialized)
    {
        for (int i = 0; i < tableSize; ++i)
        {
            double radians = i * (2.0f * pi / tableSize);
            sinTable[i] = std::sin(radians);
        }
        initialized = true;
    }
    return sinTable;
}

double sinewave(double angle)
{
    const double *sinTable = getStaticSinTable();
    angle = std::fmod(angle, 2.0f * pi);
    if (angle < 0.0f)
    {
        angle += 2.0f * pi;
    }
    int index1 = static_cast<int>(angle * (tableSize / (2.0f * pi)));
    int index2 = (index1 + 1) % tableSize;
    double weight2 = angle * (tableSize / (2.0f * pi)) - index1;
    double weight1 = 1.0f - weight2;
    return weight1 * sinTable[index1] + weight2 * sinTable[index2];
}
