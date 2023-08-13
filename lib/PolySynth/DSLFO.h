#ifndef DS_LFO_H
#define DS_LFO_H

#include <Audio.h>
#include <AudioStream.h>
#include "patching.h"

class DSLFO
{
public:
    DSLFO(short waveType = WAVEFORM_SINE, float frequency = 1.0, float amplitude = 1.0)
    {
        wave = new AudioSynthWaveform();
        rms = new AudioAnalyzeRMS();
        cables.push_back(new AudioConnection(*wave, 0, *rms, 0));
        _waveForm = waveType;
        _amplitude = amplitude;
        _frequency = frequency;        
        start();
    }
    void start()
    {
        wave->begin(_amplitude, _frequency, _waveForm);        
    }
    void setWaveForm(short waveForm) { _waveForm = waveForm; }
    void setAmplitude(float amplitude) { _amplitude = amplitude; }
    short getWaveForm() { return _waveForm; }    
    void addConnectionOut(AudioStream *toDevice, uint8_t channel)
    {
        cables.push_back(new (AudioConnection[1]){{*wave, 0, *toDevice, channel}});
    }

private:
    AudioConnectionVector cables;
    AudioSynthWaveform *wave;
    AudioAnalyzeRMS *rms;
    short _waveForm;
    float _amplitude, _frequency;
};

#endif