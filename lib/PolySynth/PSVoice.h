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

    PSVoice(String name) : PSComponent(name)
    {
        // components
        part1 = (PSVoicePart *)addChild(new PSVoicePart("PART1", &AENV1, &filter1env, &osc1PENV));
        part2 = (PSVoicePart *)addChild(new PSVoicePart("PART2", &AENV2, &filter2env, &osc2PENV));

        // parameters
        aenv_lvl = addParameter(F("A-Env Level"), 1, 0, 1, &voiceAENVdc, [](AudioStream *a, float v) { ((AudioSynthWaveformDc *)a)->amplitude(v); });
        penv_lvl = addParameter(F("P-Env Level"), 1, 0, 1);
        fenv_lvl = addParameter(F("F-Env Level"), 1, 0, 1);
        xmod1 = addParameter(F("X-Mod 1->2"), 1, 0, 1);
        xmod2 = addParameter(F("X-Mod 2->1"), 1, 0, 1);
        ringmod = addParameter(F("Ring Mod Level"), 1, 0, 1);
        ringmodFreq = addParameter(F("Ring Mod Freq"), 500, 0, 3000);
        shaperShape = addParameter(F("Shaper wave"), 1, 0, 27);
    }

    void updateAudioStreamComponent() override
    {
        Serial.printf("updateAudioStreamComponent -  %2.2f\n", aenv_lvl->getValue());
        //voiceAENVdc.amplitude(aenv_lvl->getValue());
        voicePENVdc.amplitude(penv_lvl->getValue());
        voiceFENVdc.amplitude(fenv_lvl->getValue());
        osc1XMODamt.amplitude(xmod1->getValue());
        osc2XMODamt.amplitude(xmod2->getValue());
        voiceRINGamt.amplitude(ringmod->getValue());
        osc1waveMixer.gain(1, 1.0 - ringmod->getValue());

        voiceRingModwave.frequency(ringmodFreq->getValue());

        waveshape1.shape(WST[(uint8_t)shaperShape->getValue()], 257);
        waveshape2.shape(WST[(uint8_t)shaperShape->getValue()], 257);
    }
};

#endif