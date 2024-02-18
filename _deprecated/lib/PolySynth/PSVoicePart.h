#ifndef PS_DCO_H
#define PS_DCO_H

#include "patching.h"
#include "PSEnvelope.h"
#include "waves.h"

class PSVoicePart : public PSComponent
{
public:
    PSEnvelope *vca_env, *vcf_env, *mod_env;
    AudioSynthWaveformModulated *wave;    

    PSVoicePart(
        String name,
        AudioEffectEnvelope *a_env,
        AudioEffectEnvelope *f_env,
        AudioEffectEnvelope *p_env,
        AudioSynthWaveformModulated *wave)
        : PSComponent(name)
    {
        vca_env = new PSEnvelope("A-ENV", a_env);
        vcf_env = new PSEnvelope("F-ENV", f_env);
        mod_env = new PSEnvelope("P-ENV", p_env);
        

        this->wave = wave;
        wave->frequencyModulation(10);

        this->addChild(vca_env);
        this->addChild(vcf_env);
        this->addChild(mod_env);        

        addParameter("waveform", 1, 0, 11);
        addParameter(F("FM Octaves"), 10, 0, 12, nullptr, [](AudioStream *a, float v)
                     {
            waveformLeft.frequencyModulation(v);
            waveformRight.frequencyModulation(v); });

        // addParameter()
    }
    ~PSVoicePart() {}

private:
};

#endif