#ifndef PS_PARAMETER_REGISTRY_H
#define PS_PARAMETER_REGISTRY_H

#include "patching.h"
#include "PSParameter.h"
/*

VOICE
    Part1
        Oscillator: waveform
        Oscillator: phaseMod
        Oscillator: pitch

        P-Env: delay
        P-Env: amplitude
        P-Env: hold
        P-Env: decay
        P-Env: sustain
        P-Env: release

        A-Env: delay
        A-Env: amplitude
        A-Env: hold
        A-Env: decay
        A-Env: sustain
        A-Env: release

        F-Env: delay
        F-Env: amplitude
        F-Env: hold
        F-Env: decay
        F-Env: sustain
        F-Env: release

        Noise: pinkLevel
        Noise: whiteLevel

        WaveShaper: shape

        SVFilter: type (hp / bp / lp, ladder)
        SVFilter: cutoff
        SVFilter: resonance
        SVFilter: modulationSource


    P-Env: level
    A-Env: level
    F-Env: level

    Tune: pitch

    RingMod: waveForm
    RingMod: wavePhaseMod
    Ringmod: wavePhaseModSource
    RingMod: frequency
    RingMod: part1_mix
    RingMod: part2_mix

    XMod: part1_2_level
    XMod: part2_1_level

    Detune
        amount
    StereoMix
    PitchBend
    Glide

    GainLeft
    GainRight
*/

typedef void (*onChangeFloat)(float value);

class RegistryItem
{
public:
    RegistryItem(onChangeFloat trigger);
    void callTrigger(float value);

private:
    onChangeFloat trigger;
};

RegistryItem::RegistryItem(onChangeFloat trigger)
{
    this->trigger = trigger;
}

void RegistryItem::callTrigger(float value)
{
    if (trigger != nullptr)
        trigger(value);
}

// RegistryItem item(&filter1envattack);
// filter1env
// filter1env.attack(1.0);

class MEnvelope : protected AudioEffectEnvelope
{
public:
    MEnvelope() : AudioEffectEnvelope() {}

    static void setEnvelopeParameter(AudioEffectEnvelope *env, PSParameter *p)
    {
    }
};

#endif