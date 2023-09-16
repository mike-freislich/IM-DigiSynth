#ifndef PS_ENVELOPE_H
#define PS_ENVELOPE_H

#include "Audio.h"
#include "PSComponent.h"
#include "PSParamaterTargets.h"

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
        pDelay = addParameter("Del", 0, 0, 1000, envelope, [](AudioStream *audioStream, float value)
                               { ((AudioEffectEnvelope *)audioStream)->delay(value); });
        pAttack = addParameter("Atk", 3, 0, 1000, envelope, [](AudioStream *audioStream, float value)
                               { ((AudioEffectEnvelope *)audioStream)->attack(value); });
        pHold = addParameter("Hld", 0, 0, 1000, envelope, [](AudioStream *audioStream, float value)
                               { ((AudioEffectEnvelope *)audioStream)->hold(value); });
        pDecay = addParameter("Dcy", 0, 0, 1000, envelope, [](AudioStream *audioStream, float value)
                               { ((AudioEffectEnvelope *)audioStream)->decay(value); });
        pSustain = addParameter("Sus", 50, 0, 100, envelope, [](AudioStream *audioStream, float value)
                               { ((AudioEffectEnvelope *)audioStream)->sustain(value / 100); });
        pRelease = addParameter("Rel", 3, 0, 1000, envelope, [](AudioStream *audioStream, float value)
                               { ((AudioEffectEnvelope *)audioStream)->release(value); });
        pAmount = addParameter("Amt", 1.0, 0, 1);

        _envelope = envelope;
    }

    FLASHMEM void connectOutput(AudioStream *dest, uint8_t channel) { _connections.push_back(new AudioConnection(*_envelope, 0, *dest, channel)); }
    FLASHMEM void noteOn() { _envelope->noteOn(); }
    FLASHMEM void noteOff() { _envelope->noteOff(); }

    // PSEnvelope::update()
    // If any parameters for the envelope changed,
    // copy to the AudioEnvelope
    bool updateFromControl() override
    {
        for (auto parameter : params)
        {
            if (parameter != nullptr)
            {
                if (parameter->didChange())
                    parameter->callTarget();
            }
        }
        return PSComponent::updateFromControl();
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

protected:

private:
    AudioSynthWaveformDc *_carrier;
    AudioEffectEnvelope *_envelope;
    PSParameter *pAttack, *pDecay, *pSustain, *pRelease, *pDelay, *pHold, *pAmount;
};

#endif