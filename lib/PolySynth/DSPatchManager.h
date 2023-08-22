#ifndef DS_PATCHMANAGER_H
#define DS_PATCHMANAGER_H

// #include "Parameters.h"
#include "patching.h"
#include "Audio.h"

class DSSynthPatchManager
{
};

struct ConfigItem
{
    String name;
    float value;
    ConfigItem(String name, float *destParameter, float value) : name(name), value(value)
    {
    }
};

class DSSynthPatch
{
#pragma region parameters

    uint8_t osc1_waveform = WAVEFORM_SAWTOOTH;
    uint8_t osc2_waveform = WAVEFORM_SAWTOOTH;
    
    float aenv1_attack = 0;
    float aenv1_decay = 130;
    float aenv1_sustain = 50;
    float aenv1_release = 15;
    float aenv2_attack = 0;
    float aenv2_decay = 130;
    float aenv2_sustain = 50;
    float aenv2_release = 15;
    float fenv1_attack = 0;
    float fenv1_decay = 130;
    float fenv1_sustain = 50;
    float fenv1_release = 15;
    float fenv2_attack = 0;
    float fenv2_decay = 130;
    float fenv2_sustain = 50;
    float fenv2_release = 15;
    float penv1_attack = 0;
    float penv1_decay = 130;
    float penv1_sustain = 50;
    float penv1_release = 15;
    float penv2_attack = 0;
    float penv2_decay = 130;
    float penv2_sustain = 50;
    float penv2_release = 15;
    float xmod1_amount = 0;
    float xmod2_amount = 0;

    float detune = 1.5;

    float ringMod_amount = 0;
    uint8_t ringMod_waveform = WAVEFORM_SINE;
    float ringMod_frequency = 100;


#pragma endregion

    FLASHMEM static void loadPatch()
    {
    }

    FLASHMEM static void initEnvelopes()
    {
        // OSC1 P-Env

        // OSC2 P-Env

        // Voice A-Env
        voiceAENVdc.amplitude(1);
        AENV1.delay(0);
        AENV1.attack(0);
        AENV1.hold(0);
        AENV1.decay(130);
        AENV1.sustain(50);
        AENV1.release(0);
        AENV2.delay(0);
        AENV2.attack(0);
        AENV2.hold(0);
        AENV2.decay(130);
        AENV2.sustain(50);
        AENV2.release(0);
    }
};

#endif