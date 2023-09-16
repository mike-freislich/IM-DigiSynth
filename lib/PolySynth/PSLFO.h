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
    PSLFO(String name, AudioStream *send = nullptr, uint8_t channel = 0, PSComponent *parent = nullptr) : PSComponent(name, parent)
    {
        wave = new AudioSynthWaveformModulated();
        _waveForm = waves[WAVEFORM_TRIANGLE];
        _amplitude = 1.0;
        _frequency = 0.2;

        pWaveform = addParameter("Shape", _waveForm, 0, (uint8_t)(sizeof(waves) - 1), wave, [](AudioStream *a, float v)
                                 { ((AudioSynthWaveformModulated *)a)->begin(waves[(int)v]); });
        pAmplitude = addParameter("Amp", _amplitude, 0, 1, wave, [](AudioStream *a, float v)
                                  { ((AudioSynthWaveformModulated *)a)->amplitude(v); });
        pFrequency = addParameter("Rate", _frequency, 0, LFO_MAX_FREQ, wave, [](AudioStream *a, float v)
                                  { ((AudioSynthWaveformModulated *)a)->frequency(v); });
        if (send)
            attachSend(send, channel);

        start();
    }

    void start()
    {
        if (wave != nullptr)
            wave->begin(_amplitude, _frequency, _waveForm);
    }

    FLASHMEM void attachSend(AudioStream *dest, uint8_t channel) { _connections.push_back(new AudioConnection(*wave, 0, *dest, channel)); }    

private:
    AudioSynthWaveformModulated *wave;    
    uint8_t _waveForm;
    float _amplitude, _frequency;
    PSParameter *pWaveform, *pAmplitude, *pFrequency;
};

#endif
