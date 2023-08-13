#ifndef DS_PATCHING_H
#define DS_PATCHING_H

#include <vector>
#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <vector>

typedef std::vector<AudioConnection *> AudioConnectionVector;

#ifdef USB_SERIAL
AudioOutputMQS stereoOut; // xy=1116,373
#else
#ifdef USB_AUDIO
AudioOutputUSB stereoOut; // xy=1116,373
#endif
#endif

// GUItool: begin automatically generated code
AudioSynthWaveformDc voiceFENVdc;          // xy=136.33330917358398,820.0953035354614
AudioSynthWaveformDc voicePENVdc;          // xy=188.88891983032227,781.5873203277588
AudioEffectEnvelope filter1env;            // xy=243.8888702392578,474.6508312225342
AudioEffectEnvelope filter2env;            // xy=242.11108779907227,1165.2065172195435
AudioEffectEnvelope osc2PENV;              // xy=333.6666450500488,923.8730335235596
AudioMixer4 osc1FMmixer;                   // xy=334.9999809265137,595.4285821914673
AudioEffectEnvelope osc1PENV;              // xy=335.2222099304199,700.3174724578857
AudioMixer4 osc2FMmixer;                   // xy=335.9999809265137,1033.4285821914673
AudioSynthWaveformModulated waveformRight; // xy=335.9999809265137,1093.4285821914673
AudioSynthWaveformModulated waveformLeft;  // xy=339.22218322753906,538.428596496582
AudioSynthWaveformDc osc1XMODamt;          // xy=371,367.5
AudioSynthWaveformDc voiceRINGamt;         // xy=378,810.5
AudioEffectMultiply multiplyOsc1XMOD;      // xy=380,404.5
AudioSynthWaveformDc osc2XMODamt;          // xy=398,1298.5
AudioEffectMultiply multiplyOsc2XMOD;      // xy=410,1253.5
AudioMixer4 osc2AMmixer;                   // xy=586.888916015625,971.873046875
AudioMixer4 osc1AMmixer;                   // xy=590.3174438476562,651.396728515625
AudioEffectMultiply osc2AMpost;            // xy=646,1046.5
AudioEffectMultiply osc1AMpost;            // xy=675.333309173584,570.9444608688354
AudioSynthWaveform voiceRingModwave;       // xy=700.888916015625,800.6983032226562
AudioEffectMultiply multiplyOsc1Ring;      // xy=705,751.5
AudioEffectMultiply mulitplyOsc2Ring;      // xy=705,853.5
AudioMixer4 osc1waveMixer;                 // xy=955.5555801391602,559.2063980102539
AudioSynthNoisePink pink1;                 // xy=959.0000038146973,822.5397233963013
AudioSynthNoiseWhite noise1;               // xy=959.8889770507812,787.761962890625
AudioMixer4 osc2waveMixer;                 // xy=978.1111297607422,1034.4286270141602
AudioMixer4 filter1modMixer;               // xy=1157.3332557678223,485.8730640411377
AudioMixer4 filter2modMixer;               // xy=1158.6666450500488,1177.761999130249
AudioFilterLadder ladder2;                 // xy=1161.77779006958,1112.5397272109985
AudioFilterLadder ladder1;                 // xy=1167.333267211914,551.9840908050537
AudioSynthWaveformDc voiceAENVdc;          // xy=1170.6667137145996,813.5873222351074
AudioEffectEnvelope AENV1;                 // xy=1323.5555686950684,774.2063665390015
AudioEffectEnvelope AENV2;                 // xy=1328.2221908569336,859.8729724884033
AudioAmplifier waveshapePreAmp1;           // xy=1391.7778129577637,558.9841661453247
AudioEffectWaveshaper waveshape1;          // xy=1392.777759552002,598.7619371414185
AudioAmplifier waveshapePreAmp2;           // xy=1419.9999732971191,1132.7619590759277
AudioEffectWaveshaper waveshape2;          // xy=1422.5555381774902,1095.9842100143433
AudioEffectMultiply AENV2post;             // xy=1505.1112174987793,923.5872821807861
AudioEffectMultiply AENV1post;             // xy=1507.3331413269043,704.6984510421753
AudioMixer4 voiceMainLeftmixer;            // xy=1719.2222175598145,715.7619867324829
AudioMixer4 voiceMainRightmixer;           // xy=1724.333408355713,932.3175268173218
AudioAmplifier voiceGainLeft;              // xy=1725.8888549804688,640.0953712463379
AudioAmplifier voiceGainRight;             // xy=1728.6667747497559,1006.6508531570435
AudioAnalyzePeak peakLeft;                 // xy=1890.444450378418,586.4285669326782
AudioAnalyzePeak peakRight;                // xy=1910.111198425293,1042.5396733283997
// AudioOutputUSB           stereoOut;      //xy=1932.1111030578613,856.9842100143433
AudioMixer4 oscilloscopeMixer; // xy=1955.666603088379,799.2064580917358
AudioConnection patchCord1(voiceFENVdc, filter1env);
AudioConnection patchCord2(voiceFENVdc, filter2env);
AudioConnection patchCord3(voicePENVdc, osc1PENV);
AudioConnection patchCord4(voicePENVdc, osc2PENV);
AudioConnection patchCord5(filter1env, 0, filter1modMixer, 0);
AudioConnection patchCord6(filter2env, 0, filter2modMixer, 0);
AudioConnection patchCord7(osc2PENV, 0, osc2FMmixer, 0);
AudioConnection patchCord8(osc1FMmixer, 0, waveformLeft, 0);
AudioConnection patchCord9(osc1PENV, 0, osc1FMmixer, 0);
AudioConnection patchCord10(osc2FMmixer, 0, waveformRight, 0);
AudioConnection patchCord11(waveformRight, 0, osc2waveMixer, 0);
AudioConnection patchCord12(waveformRight, 0, osc2AMpost, 0);
AudioConnection patchCord13(waveformRight, 0, multiplyOsc2XMOD, 0);
AudioConnection patchCord14(waveformLeft, 0, osc1waveMixer, 0);
AudioConnection patchCord15(waveformLeft, 0, osc1AMpost, 0);
AudioConnection patchCord16(waveformLeft, 0, multiplyOsc1XMOD, 0);
AudioConnection patchCord17(osc1XMODamt, 0, multiplyOsc1XMOD, 1);
AudioConnection patchCord18(voiceRINGamt, 0, multiplyOsc1Ring, 1);
AudioConnection patchCord19(voiceRINGamt, 0, mulitplyOsc2Ring, 1);
AudioConnection patchCord20(multiplyOsc1XMOD, 0, osc2AMmixer, 0);
AudioConnection patchCord21(osc2XMODamt, 0, multiplyOsc2XMOD, 1);
AudioConnection patchCord22(multiplyOsc2XMOD, 0, osc1AMmixer, 0);
AudioConnection patchCord23(osc2AMmixer, 0, osc2AMpost, 1);
AudioConnection patchCord24(osc1AMmixer, 0, osc1AMpost, 1);
AudioConnection patchCord25(osc2AMpost, 0, osc2waveMixer, 1);
AudioConnection patchCord26(osc1AMpost, 0, osc1waveMixer, 1);
AudioConnection patchCord27(voiceRingModwave, 0, multiplyOsc1Ring, 0);
AudioConnection patchCord28(voiceRingModwave, 0, mulitplyOsc2Ring, 0);
AudioConnection patchCord29(multiplyOsc1Ring, 0, osc1AMmixer, 1);
AudioConnection patchCord30(mulitplyOsc2Ring, 0, osc2AMmixer, 1);
AudioConnection patchCord31(osc1waveMixer, 0, ladder1, 0);
AudioConnection patchCord32(pink1, 0, osc1waveMixer, 3);
AudioConnection patchCord33(pink1, 0, osc2waveMixer, 3);
AudioConnection patchCord34(noise1, 0, osc1waveMixer, 2);
AudioConnection patchCord35(noise1, 0, osc2waveMixer, 2);
AudioConnection patchCord36(osc2waveMixer, 0, ladder2, 0);
AudioConnection patchCord37(filter1modMixer, 0, ladder1, 1);
AudioConnection patchCord38(filter2modMixer, 0, ladder2, 1);
AudioConnection patchCord39(ladder2, waveshapePreAmp2);
AudioConnection patchCord40(ladder1, waveshapePreAmp1);
AudioConnection patchCord41(voiceAENVdc, AENV1);
AudioConnection patchCord42(voiceAENVdc, AENV2);
AudioConnection patchCord43(AENV1, 0, AENV1post, 1);
AudioConnection patchCord44(AENV2, 0, AENV2post, 1);
AudioConnection patchCord45(waveshapePreAmp1, waveshape1);
AudioConnection patchCord46(waveshape1, 0, AENV1post, 0);
AudioConnection patchCord47(waveshapePreAmp2, waveshape2);
AudioConnection patchCord48(waveshape2, 0, AENV2post, 0);
AudioConnection patchCord49(AENV2post, 0, voiceMainLeftmixer, 1);
AudioConnection patchCord50(AENV2post, 0, voiceMainRightmixer, 1);
AudioConnection patchCord51(AENV1post, 0, voiceMainLeftmixer, 0);
AudioConnection patchCord52(AENV1post, 0, voiceMainRightmixer, 0);
AudioConnection patchCord53(voiceMainLeftmixer, voiceGainLeft);
AudioConnection patchCord54(voiceMainRightmixer, voiceGainRight);
AudioConnection patchCord55(voiceGainLeft, peakLeft);
AudioConnection patchCord56(voiceGainLeft, 0, stereoOut, 0);
AudioConnection patchCord57(voiceGainLeft, 0, oscilloscopeMixer, 0);
AudioConnection patchCord58(voiceGainRight, 0, stereoOut, 1);
AudioConnection patchCord59(voiceGainRight, peakRight);
AudioConnection patchCord60(voiceGainRight, 0, oscilloscopeMixer, 1);
// GUItool: end automatically generated code

#endif
