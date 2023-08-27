#ifndef PS_LFO_H
#define PS_LFO_H

#include "PSComponent.h"
#include "patching.h"
#include "waves.h"

class PSLFO : public PSComponent
{
public:
    PSLFO(String name, PSComponent *parent) : PSComponent(name, parent)
    {
        wave = new AudioSynthWaveform();
        _waveForm = waves[0];
        _amplitude = 1.0;
        _frequency = 1.0;

        addParameter("Shape", WAVEFORM_SINE, 0, sizeof(waves));
        addParameter("Amp", _amplitude, 0, 1);
        addParameter("Rate", _frequency, 0, 1);
        start();
    }

    void start() { wave->begin(_amplitude, _frequency, _waveForm); }
    void sync()
    {
        // TODO how to sync two waveforms
    }

    void setWaveForm(uint8_t waveForm) { _waveForm = waveForm; }
    void setAmplitude(float amplitude) { _amplitude = amplitude; }
    short getWaveForm() { return _waveForm; }

    void attachControllers(Controls *c) override
    {
    }

    void detachControllers() override
    {
    }

    bool update() override
    {
        return false;
    }

    void updateAudioStreamComponent() override
    {
    }

private:
    AudioSynthWaveform *wave;
    uint8_t _waveForm;
    float _amplitude, _frequency;
};

#endif
