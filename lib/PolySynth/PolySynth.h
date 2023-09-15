#ifndef POLYSYNTH_H
#define POLYSYNTH_H

#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <patching.h>
#include "PSPatchManager.h"
#include "DSLFO.h"
#include "PSLFO.h"
#include "waveshaperTables.h"
#include "PSVoicePart.h"
#include "PSEnvelope.h"
#include "PSVoice.h"
#include "waves.h"
#include "PSMaths.h"

#define AMPLITUDE (1.0)



const float noteFreqs[128] PROGMEM = {
    8.176f, 8.662f, 9.177f, 9.723f, 10.301f,
    10.913f, 11.562f, 12.25f, 12.978f, 13.75f, 14.568f, 15.434f, 16.352f,
    17.324f, 18.354f, 19.445f, 20.602f, 21.827f, 23.125f, 24.5f, 25.957f,
    27.5f, 29.135f, 30.868f, 32.703f, 34.648f, 36.708f, 38.891f, 41.203f,
    43.654f, 46.249f, 48.999f, 51.913f, 55.0f, 58.27f, 61.735f, 65.406f,
    69.296f, 73.416f, 77.782f, 82.407f, 87.307f, 92.499f, 97.999f, 103.826f,
    110.0f, 116.541f, 123.471f, 130.813f, 138.591f, 146.832f, 155.563f,
    164.814f, 174.614f, 184.997f, 195.998f, 207.652f, 220.0f, 233.082f,
    246.942f, 261.626f, 277.183f, 293.665f, 311.127f, 329.628f, 349.228f,
    369.994f, 391.995f, 415.305f, 440.0f, 466.164f, 493.883f, 523.251f,
    554.365f, 587.33f, 622.254f, 659.255f, 698.456f, 739.989f, 783.991f,
    830.609f, 880.0f, 932.328f, 987.767f, 1046.502f, 1108.731f, 1174.659f,
    1244.508f, 1318.51f, 1396.913f, 1479.978f, 1567.982f, 1661.219f, 1760.0f,
    1864.655f, 1975.533f, 2093.005f, 2217.461f, 2349.318f, 2489.016f, 2637.02f,
    2793.826f, 2959.955f, 3135.963f, 3322.438f, 3520.0f, 3729.31f, 3951.066f,
    4186.009f, 4434.922f, 4698.636f, 4978.032f, 5274.041f, 5587.652f, 5919.911f,
    6271.927f, 6644.875f, 7040.0f, 7458.62f, 7902.133f, 8372.018f, 8869.844f,
    9397.273f, 9956.063f, 10548.08f, 11175.3f, 11839.82f, 12543.85f};

const float velocity2amplitude[127] PROGMEM = {
    0.01778, 0.01966, 0.02164, 0.02371, 0.02588, 0.02814, 0.03049, 0.03294, 0.03549, 0.03812,
    0.04086, 0.04369, 0.04661, 0.04963, 0.05274, 0.05594, 0.05924, 0.06264, 0.06613, 0.06972,
    0.07340, 0.07717, 0.08104, 0.08500, 0.08906, 0.09321, 0.09746, 0.10180, 0.10624, 0.11077,
    0.11539, 0.12011, 0.12493, 0.12984, 0.13484, 0.13994, 0.14513, 0.15042, 0.15581, 0.16128,
    0.16685, 0.17252, 0.17828, 0.18414, 0.19009, 0.19613, 0.20227, 0.20851, 0.21484, 0.22126,
    0.22778, 0.23439, 0.24110, 0.24790, 0.25480, 0.26179, 0.26887, 0.27605, 0.28333, 0.29070,
    0.29816, 0.30572, 0.31337, 0.32112, 0.32896, 0.33690, 0.34493, 0.35306, 0.36128, 0.36960,
    0.37801, 0.38651, 0.39511, 0.40381, 0.41260, 0.42148, 0.43046, 0.43953, 0.44870, 0.45796,
    0.46732, 0.47677, 0.48631, 0.49595, 0.50569, 0.51552, 0.52544, 0.53546, 0.54557, 0.55578,
    0.56609, 0.57648, 0.58697, 0.59756, 0.60824, 0.61902, 0.62989, 0.64085, 0.65191, 0.66307,
    0.67432, 0.68566, 0.69710, 0.70863, 0.72026, 0.73198, 0.74380, 0.75571, 0.76771, 0.77981,
    0.79201, 0.80430, 0.81668, 0.82916, 0.84174, 0.85440, 0.86717, 0.88003, 0.89298, 0.90602,
    0.91917, 0.93240, 0.94573, 0.95916, 0.97268, 0.98629, 1.00000};

struct StereoLevels
{
    float left, right;
    StereoLevels() {}
    StereoLevels(float left, float right) : left(left), right(right) {}
};



class PolySynth
{
public:
    PSLFO *filterLFO;
    PSLFO *filterLFO2;
    DSLFO *pitchLFO;
    DSLFO *pitchLFO2;

    PSVoice voice1 = PSVoice("Voice1");

    PolySynth() { }
    ~PolySynth() {}

    void init()
    {
        AudioMemory(32);
        // filterLFO = new DSLFO(WAVEFORM_TRIANGLE, 0.05, 1.0);
        // filterLFO->addConnectionOut(&filter1modMixer, 1);
        // filterLFO->addConnectionOut(&filter2modMixer, 1);
        filterLFO2 = new PSLFO(F("Filter-LFO1"));// WAVEFORM_TRIANGLE, 0.05, 1.0);
        filterLFO2->setWaveForm(WAVEFORM_TRIANGLE);
        filterLFO2->setAmplitude(0.2f);
        filterLFO2->setFrequency(36.0f);
        filterLFO2->attachSend(&filter1modMixer, 2);
        filterLFO2->attachSend(&filter2modMixer, 2);
        //filterLFO2->
        // filterLFO2 = new DSLFO(WAVEFORM_TRIANGLE, 36, 0.2);
        //filterLFO2->addConnectionOut(&filter1modMixer, 2);
        //filterLFO2->addConnectionOut(&filter2modMixer, 2);

        filter1blockMixer.gain(0, 0);
        filter1blockMixer.gain(1, 0);
        filter1blockMixer.gain(2, 0);
        filter1blockMixer.gain(3, 1);
        filter2blockMixer.gain(0, 0);
        filter2blockMixer.gain(1, 0);
        filter2blockMixer.gain(2, 0);
        filter2blockMixer.gain(3, 1);
        HPFdc.amplitude(10);
        hpf1.resonance(0);
        hpf2.resonance(0);

        // pitchLFO = new DSLFO(WAVEFORM_SAMPLE_HOLD, 32, 0.005);
        // pitchLFO->addConnectionOut(&osc1mod, 0);
        // pitchLFO->addConnectionOut(&osc2modMixer, 0);
        // pitchLFO2 = new DSLFO(WAVEFORM_SINE, 0.2, 1);
        // pitchLFO2->addConnectionOut(&osc1modMixer, 1);
        // pitchLFO2->addConnectionOut(&osc2modMixer, 1);        
        // this->osc1_PEnv.setParameters(5, 15, 130, 0, 0);
        // this->osc2_PEnv.setParameters(5, 15, 130, 0, 0);

        voiceMainLeftmixer.gain(0, 1.0);
        voiceMainLeftmixer.gain(1, 1.0);
        voiceMainLeftmixer.gain(2, 1.0);
        voiceMainLeftmixer.gain(3, 1.0);
        voiceMainRightmixer.gain(0, 1.0);
        voiceMainRightmixer.gain(1, 1.0);
        voiceMainRightmixer.gain(2, 1.0);
        voiceMainRightmixer.gain(3, 1.0);
        osc1FMmixer.gain(0, 1.0);
        osc1FMmixer.gain(1, 1.0);
        osc1FMmixer.gain(2, 1.0);
        osc1FMmixer.gain(3, 1.0);
        osc2FMmixer.gain(0, 1.0);
        osc2FMmixer.gain(1, 1.0);
        osc2FMmixer.gain(2, 1.0);
        osc2FMmixer.gain(3, 1.0);

        // filter
        // voice1.dco1->vcf_env->set(10, 0, 130, 0, 0);
        // osc1_FEnv.setParameters(10, 0, 130, 0, 0);
        // osc2_FEnv.setParameters(10, 0, 130, 0, 0);
        ladder1.resonance(0.8);
        ladder2.resonance(0.8);
        voiceFENVdc.amplitude(1);

        waveformLeft.phaseModulation(30);
        waveformRight.phaseModulation(45);

        voiceAENVdc.amplitude(1);
        voicePENVdc.amplitude(1);

        voiceGainLeft.gain(1.0);
        voiceGainRight.gain(1.0);

        _detune = 0.1;

        balance(0);
        pan(0, -1);
        pan(1, 1);

        waveshapePreAmp1.gain(1);
        waveshapePreAmp2.gain(1);
        setWaveShaper(29);
        initOscMixer();
        setNoise(NoiseType::pinkNoise);

        ringMod(1.0, 500, WAVEFORM_TRIANGLE);
        xmod(0, 0.5);
        xmod(1, 0.5);

        AudioProcessorUsageMaxReset();
        AudioMemoryUsageMaxReset();

        // dco1 = new PSDCO("DCO1", &AENV1, &filter1env, &osc1PENV);
        // dco1->set(PSEnvelopeParam::PSP_ENV_AMOUNT, 0.0);
        // dco1->params[PSEnvelopeParam::PSP_ENV_AMOUNT]->value;        
    }

    void playNote(uint8_t midiNote, uint8_t velocity)
    {
        uint32_t now = millis();
        if (now - waveTimer > 10000)
        {
            waveTimer = now;
            //nextWaveForm();
        }

        AudioNoInterrupts();
        waveformLeft.begin(AMPLITUDE * velocity2amplitude[velocity], noteFreqs[midiNote] - _detune, (uint16_t)voice1.part1->params[0]->getValue());
        waveformRight.begin(AMPLITUDE * velocity2amplitude[velocity], noteFreqs[midiNote] + _detune, waves[waveIndex]); // detune

        voice1.part1->vca_env->noteOn();
        voice1.part1->vcf_env->noteOn();
        voice1.part1->mod_env->noteOn();
        voice1.part2->vca_env->noteOn();
        voice1.part2->vcf_env->noteOn();
        voice1.part2->mod_env->noteOn();
        AudioInterrupts();
    }

    void nextWaveForm() { waveIndex = (waveIndex + 1) % (sizeof(waves) / sizeof(waves[0])); }

    void stopNote()
    {
        // voice1.dco1->vca_env->noteOn();
        // voice1.dco1->vcf_env->noteOn();
        // voice1.dco1->mod_env->noteOn();
        // voice1.dco2->vca_env->noteOn();
        // voice1.dco2->vcf_env->noteOn();
        // voice1.dco2->mod_env->noteOn();       
    }

    StereoLevels getPeakLevel()
    {
        return StereoLevels(peakLeft.read(), peakRight.read());
    }    

#pragma region Synth Init

    FLASHMEM void initOscMixer()
    {
        osc1waveMixer.gain(0, 0.5); // waveform osc1 left
        osc1waveMixer.gain(1, 1.0); // waveform osc1 post AM left
        osc1waveMixer.gain(2, 0.01); // white noise left
        osc1waveMixer.gain(3, 0.01); // pink noise left

        osc2waveMixer.gain(0, 0.5); // waveform osc2 right
        osc2waveMixer.gain(1, 1.0); // waveform osc2 post AM left
        osc2waveMixer.gain(2, 0.01); // white noise right
        osc2waveMixer.gain(3, 0.01); // pink noise right
    }

    enum NoiseType
    {
        noiseOff,
        whiteNoise,
        pinkNoise
    };
    FLASHMEM void setNoise(NoiseType noiseType = noiseOff, float mixerVolume = 0)
    {
        switch (noiseType)
        {
        case noiseOff:
            noise1.amplitude(0);
            pink1.amplitude(0);
            break;
        case whiteNoise:
            noise1.amplitude(1);
            pink1.amplitude(0);
            break;
        case pinkNoise:
            noise1.amplitude(0);
            pink1.amplitude(1);
            break;
        }
    }

    FLASHMEM void initWaveShaper()
    {
        setWaveShaper(0);
    }

    FLASHMEM void setWaveShaper(uint8_t shapeIndex)
    {
        if (shapeIndex > 29)
            shapeIndex = 29;
        else if (shapeIndex < 0)
            shapeIndex = 0;
        int tlen = (shapeIndex == 0) ? 3 : 257;
        waveshape1.shape(WST[shapeIndex], tlen);
        waveshape2.shape(WST[shapeIndex], tlen);
    }

    // left = -1.0, centre = 0, right = 1.0;
    FLASHMEM void pan(uint8_t osc, float value)
    {
        osc = clamp(osc, (uint8_t)0, (uint8_t)1);
        value = clamp(value, (float)-1, (float)1);

        // float balanceFactorRight = (_balance + 1) / 2;
        // float balanceFactorLeft = 1 - balanceFactorRight;

        float rightAmount = ((value + 1) / 2); // * balanceFactorRight;
        float leftAmount = (1 - rightAmount);  // * balanceFactorLeft;

        voiceMainLeftmixer.gain(osc, leftAmount);
        voiceMainRightmixer.gain(osc, rightAmount);
    }

    FLASHMEM void balance(float value)
    {
        _balance = clamp(value, (float)-1.0, (float)1.0);
    }

    FLASHMEM void ringMod(float amount, float freq = 10, uint8_t waveform = WAVEFORM_SINE)
    {
        voiceRINGamt.amplitude(amount);
        voiceRingModwave.begin(amount, freq, waveform);
    }

    FLASHMEM void xmod(uint8_t osc, float amount)
    {
        osc = clamp(osc, (uint8_t)0, (uint8_t)1);
        if (osc == 0)
            osc1XMODamt.amplitude(amount);
        else
            osc2XMODamt.amplitude(amount);
    }

#pragma endregion

    void savePatch(uint16_t patchNo) 
    {   
        String data = voice1.toString();
        patchManager.savePatch("patch000.psp", data);
    }

    void loadPatch(uint16_t patchNo)
    {
        String data = patchManager.loadPatch("patch000.psp");
        if (data)
            voice1.fromString(data);

    }
private:
    uint32_t waveTimer;
    uint8_t waveIndex;
    float _detune;
    float _balance;

    // template <typename T>
    // FLASHMEM T clamp(T a, T minValue, T maxValue)
    // {
    //     a = max(a, minValue);
    //     a = min(a, maxValue);
    //     return a;
    // }

};

#endif
