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
        pDelay = addParameter("Del", 0, 0, 1000);
        pAttack = addParameter("Atk", 3, 0, 1000);
        pHold = addParameter("Hld", 0, 0, 1000);
        pDecay = addParameter("Dcy", 0, 0, 1000);
        pSustain = addParameter("Sus", 50, 0, 100);
        pRelease = addParameter("Rel", 3, 0, 1000);
        pAmount = addParameter("Amt", 1.0, 0, 1);
        _envelope = envelope;
    }

    FLASHMEM void attach(AudioStream *dest, uint8_t channel)
    {
        _connections.push_back(new AudioConnection(*_envelope, 0, *dest, channel));
    }

    FLASHMEM void updateAudioStreamComponent()
    {
        _envelope->attack(params[PSP_ENV_ATTACK]->getValue());
        _envelope->decay(params[PSP_ENV_DECAY]->getValue());
        _envelope->sustain(params[PSP_ENV_SUSTAIN]->getValue());
        _envelope->release(params[PSP_ENV_RELEASE]->getValue());
        _envelope->delay(params[PSP_ENV_DELAY]->getValue());
        _envelope->hold(params[PSP_ENV_HOLD]->getValue());
    }

    FLASHMEM void noteOn()
    {
        _envelope->noteOn();
    }

    /*
     * PSEnvelope::update()
     * If any parameters for the envelope changed,
     * copy to the AudioEnvelope 
     */
    bool update()
    {
        // update from controllers
        bool updated = false;
        for (auto p : params)
        {
            if (p->didChange(true))
                updated = true;
        }

        if (updated)
            this->updateAudioStreamComponent();

        return updated;
    }

    void attachControllers(Controls *c) override
    {
        pAttack->attachController((InputBase *)&c->pots[0]);
        pDecay->attachController((InputBase *)&c->pots[1]);
        pSustain->attachController((InputBase *)&c->pots[2]);
        pRelease->attachController((InputBase *)&c->pots[3]);
    }

    void detachControllers() override
    {
        pAttack->detachController();
        pDecay->detachController();
        pSustain->detachController();
        pRelease->detachController();
    }

private:
    AudioSynthWaveformDc *_carrier;
    AudioEffectEnvelope *_envelope;
    PSParameter *pAttack, *pDecay, *pSustain, *pRelease, *pDelay, *pHold, *pAmount;
};

#endif