#ifndef PS_LFO_H
#define PS_LFO_H

#include "PSComponent.h"
#include "patching.h"
#include "waves.h"

#define LFO_MAX_FREQ 1000

enum PSLFO_PARM
{
    PSP_LFO_SHAPE = 0,
    PSP_LFO_AMP = 1,
    PSP_LFO_RATE = 2
};

class PSLFO : public PSComponent
{
public:
    PSLFO(String name, PSComponent *parent = nullptr) : PSComponent(name, parent)
    {
        this->wave = new AudioSynthWaveformModulated();
        _waveForm = waves[0];
        _amplitude = 1.0;
        _frequency = 10;

        pWaveform = addParameter("Shape", WAVEFORM_TRIANGLE, 0, 5);
        pAmplitude = addParameter("Amp", _amplitude, 0, 1);
        pFrequency = addParameter("Rate", _frequency, 0, LFO_MAX_FREQ);
        start();
    }

    void start()
    {
        if (wave != nullptr)
            wave->begin(_amplitude, _frequency, _waveForm);
    }

    void sync()
    {
        // TODO how to sync two waveforms
    }

    void setWaveForm(uint8_t waveForm) { _waveForm = clamp(waveForm, (uint8_t)0, (uint8_t)(sizeof(waves) - 1)); }
    void setAmplitude(float amplitude) { _amplitude = clamp(amplitude, 0.0f, 1.0f); }
    void setFrequency(float frequency) { _frequency = clamp(frequency, 0.0f, (float)LFO_MAX_FREQ); }
    uint8_t getWaveForm() { return _waveForm; }

    void attachControllers(Controls *c) override
    {
        //pWaveform->attachController((InputBase *)&c->pots[0]);
        //pAmplitude->attachController((InputBase *)&c->pots[1]);
        //pFrequency->attachController((InputBase *)&c->pots[2]);
        //pRelease->attachController((InputBase *)&c->pots[3]);
    }

    void detachControllers() override
    {
        //pWaveform->detachController();
        //pAmplitude->detachController();
        //pFrequency->detachController();
    }

    /*
     * PSLFO::update()
     * If any parameters for the LFO changed,
     * copy to the audioStreamComponent
     */
    bool updateFromControl() override { return PSComponent::updateFromControl(); }

    FLASHMEM void attachSend(AudioStream *dest, uint8_t channel) { _connections.push_back(new AudioConnection(*wave, 0, *dest, channel)); }
    //FLASHMEM void attachInput(AudioStream *dest, uint8_t channel) { _connections.push_back(new AudioConnection(*wave, 0, *dest, channel)); }

    void updateAudioStreamComponent() override
    {
        float a = params[PSP_LFO_AMP]->getValue();
        float f = params[PSP_LFO_RATE]->getValue();
        uint8_t w = params[PSP_LFO_SHAPE]->getValue();
        Serial.printf("a %2.2f, f %2.2f, w %d\n", a, f, w);

        if (a != _amplitude)
        {
            _amplitude = a;
            wave->amplitude(a);
        }

        if (f != _frequency)
        {
            _frequency = f;
            wave->frequency(f);
        }

        if (w != _waveForm)
        {
            _waveForm = w;
            wave->begin(_waveForm);
        }

        // if (!wave->isActive())
        // {
        //     wave->begin(_amplitude, _frequency, _waveForm);
        // }
    }

private:
    AudioSynthWaveformModulated *wave;
    // AudioSynthWaveformDc *_carrier;    
    uint8_t _waveForm;
    float _amplitude, _frequency;
    PSParameter *pWaveform, *pAmplitude, *pFrequency;
};

#endif
