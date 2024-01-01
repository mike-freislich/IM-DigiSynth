#ifndef PS_VOICE_H
#define PS_VOICE_H

#include "PSComponent.h"
#include "PSVoicePart.h"
#include "waveshaperTables.h"

class PSVoice : public PSComponent
{
public:
    PSVoicePart *part1, *part2;
    PSParameter *xmod1, *xmod2, *aenv_lvl, *penv_lvl, *fenv_lvl, *ringmod, *ringmodFreq, *shaperShape;

    PSVoice(String name, PSComponent *parent = nullptr) : PSComponent(name, parent)
    {
        // components
        part1 = (PSVoicePart *)addChild(new PSVoicePart("PART1", &AENV1, &filter1env, &osc1PENV, &waveformLeft));
        part2 = (PSVoicePart *)addChild(new PSVoicePart("PART2", &AENV2, &filter2env, &osc2PENV, &waveformRight));

        // parameters
        ParameterTarget target_amplitude = [](AudioStream *a, float v)
        { ((AudioSynthWaveformDc *)a)->amplitude(v); };

        aenv_lvl = addParameter(F("A-Env Level"), 1, 0, 1, &voiceAENVdc, target_amplitude, asPERCENTAGE);
        penv_lvl = addParameter(F("P-Env Level"), 1, 0, 1, &voicePENVdc, target_amplitude, asPERCENTAGE);
        fenv_lvl = addParameter(F("F-Env Level"), 1, 0, 1, &voiceFENVdc, target_amplitude, asPERCENTAGE);
        xmod1 = addParameter(F("X-Mod 1->2"), 1, 0, 1, &osc1XMODamt, target_amplitude, asPERCENTAGE);
        xmod2 = addParameter(F("X-Mod 2->1"), 1, 0, 1, &osc2XMODamt, target_amplitude, asPERCENTAGE);
        ringmod = addParameter(
            F("Ring Mod Level"), 1, 0, 1, &voiceRINGamt, [](AudioStream *a, float v)
            { voiceRINGamt.amplitude(v); osc1waveMixer.gain(1, 1.0 - v); },
            asPERCENTAGE);
        ringmodFreq = addParameter(F("Ring Mod Freq"), 500, 0, 5000, &voiceRingModwave, [](AudioStream *a, float v)
                                   { voiceRingModwave.frequency(v); });
        shaperShape = addParameter(F("Shaper wave"), 1, 0, 29, &waveshape1, [](AudioStream *a, float v)
                                   { waveshape1.shape(WST[(uint8_t)v], 257); waveshape2.shape(WST[(uint8_t)v], 257); });        
    }

    void setupControllers() // TODO rig controllers to parameters
    {
        aenv_lvl->attachController(&controls.pots[0]);
        aenv_lvl->activate();
        ringmod->attachController(&controls.pots[1]);
        ringmod->activate();
    }
};

#endif