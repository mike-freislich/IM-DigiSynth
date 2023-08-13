#ifndef DS_SYNTH_H
#define DS_SYNTH_H

#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include "PlaySynthMusic.h"

#pragma region--------------------  SETUP AUDIO --------------------

unsigned char *sp = score;
#define AMPLITUDE (0.2)

#pragma region GUItool output

// GUItool: begin automatically generated code
AudioSynthWaveform sine0;  // xy=433,680
AudioSynthWaveform sine1;  // xy=433,680
AudioSynthWaveform sine2;  // xy=433,680
AudioSynthWaveform sine15; // xy=433,680
AudioSynthWaveform sine8;  // xy=433,680
AudioSynthWaveform sine10; // xy=433,680
AudioSynthWaveform sine6;  // xy=433,680
AudioSynthWaveform sine3;  // xy=433,680
AudioSynthWaveform sine7;  // xy=433,680
AudioSynthWaveform sine9;  // xy=433,680
AudioSynthWaveform sine12; // xy=433,680
AudioSynthWaveform sine4;  // xy=433,680
AudioSynthWaveform sine13; // xy=433,680
AudioSynthWaveform sine14; // xy=433,680
AudioSynthWaveform sine5;  // xy=433,680
AudioSynthWaveform sine11; // xy=433,680
AudioEffectEnvelope env0;  // xy=433,680
AudioEffectEnvelope env1;  // xy=433,680
AudioEffectEnvelope env2;  // xy=433,680
AudioEffectEnvelope env4;  // xy=433,680
AudioEffectEnvelope env3;  // xy=433,680
AudioEffectEnvelope env6;  // xy=433,680
AudioEffectEnvelope env7;  // xy=433,680
AudioEffectEnvelope env8;  // xy=433,680
AudioEffectEnvelope env5;  // xy=433,680
AudioEffectEnvelope env11; // xy=433,680
AudioEffectEnvelope env9;  // xy=433,680
AudioEffectEnvelope env10; // xy=433,680
AudioEffectEnvelope env14; // xy=466,511
AudioEffectEnvelope env13; // xy=473,427
AudioEffectEnvelope env12; // xy=490,590
AudioEffectEnvelope env15; // xy=514,336
AudioMixer4 mixer4;        // xy=803,746
AudioMixer4 mixerMainLeft;        // xy=807,1080
AudioMixer4 mixerMainRight;        // xy=830,571
AudioMixer4 mixer3;        // xy=845,861
AudioMixer4 mixerLeft;     // xy=1102,790
AudioMixer4 mixerRight;    // xy=1109,939
AudioAnalyzeRMS rms1;      // xy=1281,677
AudioAnalyzePeak peak2;    // xy=1282,1061
AudioAnalyzePeak peak1;    // xy=1284,720
AudioAnalyzeRMS rms2;      // xy=1283,1007
AudioOutputUSB audioOut;   // xy=1310,856
AudioConnection patchCord1(sine0, env0);
AudioConnection patchCord2(sine1, env1);
AudioConnection patchCord3(sine2, env2);
AudioConnection patchCord4(sine15, env15);
AudioConnection patchCord5(sine8, env8);
AudioConnection patchCord6(sine10, env10);
AudioConnection patchCord7(sine6, env6);
AudioConnection patchCord8(sine3, env3);
AudioConnection patchCord9(sine7, env7);
AudioConnection patchCord10(sine9, env9);
AudioConnection patchCord11(sine12, env12);
AudioConnection patchCord12(sine4, env4);
AudioConnection patchCord13(sine13, env13);
AudioConnection patchCord14(sine14, env14);
AudioConnection patchCord15(sine5, env5);
AudioConnection patchCord16(sine11, env11);
AudioConnection patchCord17(env0, 0, mixerMainLeft, 0);
AudioConnection patchCord18(env1, 0, mixerMainLeft, 1);
AudioConnection patchCord19(env2, 0, mixerMainLeft, 2);
AudioConnection patchCord20(env4, 0, mixerMainRight, 0);
AudioConnection patchCord21(env3, 0, mixerMainLeft, 3);
AudioConnection patchCord22(env6, 0, mixerMainRight, 2);
AudioConnection patchCord23(env7, 0, mixerMainRight, 3);
AudioConnection patchCord24(env8, 0, mixer3, 0);
AudioConnection patchCord25(env5, 0, mixerMainRight, 1);
AudioConnection patchCord26(env11, 0, mixer3, 3);
AudioConnection patchCord27(env9, 0, mixer3, 1);
AudioConnection patchCord28(env10, 0, mixer3, 2);
AudioConnection patchCord29(env14, 0, mixer4, 2);
AudioConnection patchCord30(env13, 0, mixer4, 1);
AudioConnection patchCord31(env12, 0, mixer4, 0);
AudioConnection patchCord32(env15, 0, mixer4, 3);
AudioConnection patchCord33(mixer4, 0, mixerLeft, 3);
AudioConnection patchCord34(mixer4, 0, mixerRight, 3);
AudioConnection patchCord35(mixerMainLeft, 0, mixerLeft, 0);
AudioConnection patchCord36(mixerMainLeft, 0, mixerRight, 0);
AudioConnection patchCord37(mixerMainRight, 0, mixerLeft, 1);
AudioConnection patchCord38(mixerMainRight, 0, mixerRight, 1);
AudioConnection patchCord39(mixer3, 0, mixerLeft, 2);
AudioConnection patchCord40(mixer3, 0, mixerRight, 2);
AudioConnection patchCord41(mixerLeft, 0, audioOut, 0);
AudioConnection patchCord42(mixerLeft, rms1);
AudioConnection patchCord43(mixerLeft, peak1);
AudioConnection patchCord44(mixerRight, 0, audioOut, 1);
AudioConnection patchCord45(mixerRight, rms2);
AudioConnection patchCord46(mixerRight, peak2);
// GUItool: end automatically generated code

#pragma endregion

// // Create 16 waveforms, one for each MIDI channel
AudioSynthWaveform *waves[16] = {
    &sine0, &sine1, &sine2, &sine3,
    &sine4, &sine5, &sine6, &sine7,
    &sine8, &sine9, &sine10, &sine11,
    &sine12, &sine13, &sine14, &sine15};

// allocate a wave type to each channel.
// The types used and their order is purely arbitrary.
short wave_type[16] = {
    WAVEFORM_SINE,
    WAVEFORM_SQUARE,
    WAVEFORM_SAWTOOTH,
    WAVEFORM_TRIANGLE,
    WAVEFORM_SINE,
    WAVEFORM_SQUARE,
    WAVEFORM_SAWTOOTH,
    WAVEFORM_TRIANGLE,
    WAVEFORM_SINE,
    WAVEFORM_SQUARE,
    WAVEFORM_SAWTOOTH,
    WAVEFORM_TRIANGLE,
    WAVEFORM_SINE,
    WAVEFORM_SQUARE,
    WAVEFORM_SAWTOOTH,
    WAVEFORM_TRIANGLE};

// Each waveform will be shaped by an envelope
// AudioEffectEnvelope env0, env1, env2, env3;
// AudioEffectEnvelope env4, env5, env6, env7;
// AudioEffectEnvelope env8, env9, env10, env11;
// AudioEffectEnvelope env12, env13, env14, env15;
AudioEffectEnvelope *envs[16] = {
    &env0, &env1, &env2, &env3,
    &env4, &env5, &env6, &env7,
    &env8, &env9, &env10, &env11,
    &env12, &env13, &env14, &env15};

// AudioControlSGTL5000 codec;

// Initial value of the volume control
int volume = 50;
#pragma endregion

FLASHMEM void setupSynth()
{
  // Proc = 12 (13),  Mem = 2 (8)
  // Audio connections require memory to work.
  // The memory usage code indicates that 10 is the maximum
  // so give it 12 just to be sure.
  AudioMemory(32);

  // codec.enable();
  // codec.volume(0.45);
  //  reduce the gain on some channels, so half of the channels
  //  are "positioned" to the left, half to the right, but all
  //  are heard at least partially on both ears
  mixerLeft.gain(1, 0.36);
  mixerLeft.gain(3, 0.36);
  mixerRight.gain(0, 0.36);
  mixerRight.gain(2, 0.36);

  // set envelope parameters, for pleasing sound :-)
  for (int i = 0; i < 16; i++)
  {
    envs[i]->attack(9.2);
    envs[i]->hold(2.1);
    envs[i]->decay(31.4);
    envs[i]->sustain(0.6);
    envs[i]->release(84.5);
    // uncomment these to hear without envelope effects
    // envs[i]->attack(0.0);
    // envs[i]->hold(0.0);
    // envs[i]->decay(0.0);
    // envs[i]->release(0.0);
  }

  // Initialize processor and memory measurements
  AudioProcessorUsageMaxReset();
  AudioMemoryUsageMaxReset();
}

uint32_t last_time = 0;
void updateSynth()
{
  unsigned char c, opcode, chan;
  unsigned long d_time;

  // if (millis() - last_time >= 5000)
  // {
  //   // Serial.print("Proc = ");
  //   // Serial.print(AudioProcessorUsage());
  //   // Serial.print(" (");
  //   // Serial.print(AudioProcessorUsageMax());
  //   // Serial.print("),  Mem = ");
  //   // Serial.print(AudioMemoryUsage());
  //   // Serial.print(" (");
  //   // Serial.print(AudioMemoryUsageMax());
  //   // Serial.println(")");
  //   last_time = millis();
  // }
  // Volume control
  //  uncomment if you have a volume pot soldered to your audio shield
  // int n = analogRead(15);
  // if (n != volume) {
  //   volume = n;
  //   codec.volume((float)n / 1023);
  // }

  // read the next note from the table
  c = *sp++;
  opcode = c & 0xF0;
  chan = c & 0x0F;

  if (c < 0x80)
  {
    // Delay
    d_time = (c << 8) | *sp++;
    
    //delay(d_time);
    uint32_t now = millis();
    while (millis() - now < d_time) { yield(); }
    
    return;
  }
  if (*sp == CMD_STOP)
  {
    for (chan = 0; chan < 10; chan++)
    {
      envs[chan]->noteOff();
      waves[chan]->amplitude(0);
    }
    Serial.println("DONE... Starting again");
    sp = score;
    return;
  }

  // It is a command

  // Stop the note on 'chan'
  if (opcode == CMD_STOPNOTE)
  {
    envs[chan]->noteOff();
    return;
  }

  // Play the note on 'chan'
  if (opcode == CMD_PLAYNOTE)
  {
    unsigned char note = *sp++;
    unsigned char velocity = *sp++;
    AudioNoInterrupts();
    waves[chan]->begin(AMPLITUDE * velocity2amplitude[velocity - 1],
                       tune_frequencies2_PGM[note],
                       wave_type[chan]);
    envs[chan]->noteOn();
    AudioInterrupts();
    return;
  }

  // replay the tune
  if (opcode == CMD_RESTART)
  {
    sp = score;
    return;
  }
}

#endif
