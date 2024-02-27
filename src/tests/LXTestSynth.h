#pragma once
#include <Audio.h>
#include "TestAUMapping.h"
// #include "FastMath.h"

#define BASE_NOTE 60.0f

class LXTestSynth
{
public:
    LXTestSynth() {}

    void init()
    {
        AudioMemory(64);

        auBlend_xmod.blend(0);
        auWave_oscA.begin(WAVEFORM_PULSE);
        auWave_oscB.begin(WAVEFORM_PULSE);
        auWave_oscA.frequency(BASE_NOTE);
        auWave_oscA.amplitude(1.0f);
        auWave_oscB.frequency(BASE_NOTE+10);
        auWave_oscB.amplitude(1.0f);

        AudioProcessorUsageMaxReset();
        AudioMemoryUsageMaxReset();
        syncTime.start(30);
    }

    void update()
    {
        float s = sineWavePoint(0.5f, 0.05f, 0, millis())+0.5f;
        if (syncTime.update())
        auWave_oscB.phaseModulation(s*360);

        syncTime.duration(s*100);
        //auWave_oscA.frequency(BASE_NOTE + s * (BASE_NOTE));
        //auWave_oscB.frequency(BASE_NOTE - s * BASE_NOTE);        
        //printf("s = %2.2f\n", s);
        auBlend_xmod.blend(1-s);
    }

private:
    SimpleTimer syncTime;

    float sineWavePoint(float amplitude, float frequency, float phaseOffset, uint32_t numMilliseconds)
    {
        const float twoPi = 2.0f * 3.14159f;
        double timeInSeconds = static_cast<double>(numMilliseconds) / 1000.0;
        double phase = twoPi * frequency * timeInSeconds + twoPi * phaseOffset;
        return amplitude * sin(phase);
    }

} testSynth;