#ifndef PS_ENVELOPE_H
#define PS_ENVELOPE_H

#include "Audio.h"
#include "PSComponent.h"

enum PSEnvelopeParam
{
    PSP_ENV_DELAY = 0,
    PSP_ENV_ATTACK = 1,
    PSP_ENV_HOLD = 2,
    PSP_ENV_DECAY = 3,
    PSP_ENV_SUSTAIN = 4,
    PSP_ENV_RELEASE = 5,
    PSP_ENV_AMOUNT = 6
};

class PSEnvelope : public PSComponent
{
public:
    PSEnvelope(String name, AudioEffectEnvelope *envelope) : PSComponent(name)
    {
        addParameter("Del", 0);
        addParameter("Atk", 3);
        addParameter("Hld", 0);
        addParameter("Dcy", 0);
        addParameter("Sus", 50);
        addParameter("Rel", 3);
        addParameter("Amt", 1.0);
        _envelope = envelope;
    }

    FLASHMEM void attach(AudioStream *dest, uint8_t channel)
    {
        _connections.push_back(new AudioConnection(*_envelope, 0, *dest, channel));
    }

    FLASHMEM void update(float attack, float decay, float sustain, float release, float delay = 0, float hold = 0)
    {
        params[PSP_ENV_ATTACK]->value = attack;
        params[PSP_ENV_DECAY]->value = decay;
        params[PSP_ENV_SUSTAIN]->value = sustain;
        params[PSP_ENV_RELEASE]->value = release;
        params[PSP_ENV_DELAY]->value = delay;
        params[PSP_ENV_HOLD]->value = hold;
        this->updateAudioStreamComponent();
    }

    FLASHMEM void updateAudioStreamComponent()
    {        
        _envelope->attack(params[PSP_ENV_ATTACK]->value);
        _envelope->decay(params[PSP_ENV_DECAY]->value);
        _envelope->sustain(params[PSP_ENV_SUSTAIN]->value);
        _envelope->release(params[PSP_ENV_RELEASE]->value);
        _envelope->delay(params[PSP_ENV_DELAY]->value);
        _envelope->hold(params[PSP_ENV_HOLD]->value);
    }

private:
    AudioSynthWaveformDc *_carrier;
    AudioEffectEnvelope *_envelope;
};

#endif