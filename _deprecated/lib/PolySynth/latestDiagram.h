/* 1-JAN-2024 

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveformDc     voiceFENVdc;    //xy=144,512
AudioSynthWaveformDc     pitchBendDC;            //xy=144,562
AudioSynthWaveformDc     voicePENVdc;    //xy=319,792
AudioEffectEnvelope      filter1env;     //xy=401,500
AudioEffectEnvelope      filter2env;     //xy=400,1191
AudioEffectEnvelope      osc2PENV;       //xy=491,949
AudioEffectEnvelope      osc1PENV;       //xy=493,726
AudioMixer4              osc2FMmixer;    //xy=493,1059
AudioSynthWaveformModulated waveformRight;  //xy=493,1119
AudioSynthWaveformModulated waveformLeft;   //xy=497,564
AudioMixer4              osc1FMmixer;    //xy=497,622
AudioSynthWaveformDc     osc1XMODamt;    //xy=529,393
AudioEffectMultiply      multiplyOsc1XMOD; //xy=538,430
AudioSynthWaveformDc     osc2XMODamt;    //xy=556,1324
AudioSynthWaveformDc     voiceRINGamt;   //xy=562,827
AudioEffectMultiply      multiplyOsc2XMOD; //xy=568,1279
AudioMixer4              osc2AMmixer;    //xy=744,997
AudioMixer4              osc1AMmixer;    //xy=748,677
AudioEffectMultiply      osc2AMpost;     //xy=804,1072
AudioEffectMultiply      osc1AMpost;     //xy=833,596
AudioSynthWaveform       voiceRingModwave; //xy=858,826
AudioEffectMultiply      multiplyOsc1Ring; //xy=863,777
AudioEffectMultiply      mulitplyOsc2Ring; //xy=863,879
AudioMixer4              osc1waveMixer;  //xy=1094,567
AudioMixer4              filter1modMixer; //xy=1095,499
AudioSynthNoiseWhite     noise1;         //xy=1117,813
AudioSynthNoisePink      pink1;          //xy=1117,848
AudioMixer4              filter2modMixer; //xy=1125,1153
AudioMixer4              osc2waveMixer;  //xy=1127,1086
AudioFilterStateVariable filter1;        //xy=1309,478
AudioFilterLadder        ladder1;        //xy=1310,531
AudioFilterStateVariable filter2;        //xy=1318,1194
AudioFilterLadder        ladder2;        //xy=1319,1138
AudioSynthWaveformDc     voiceAENVdc;    //xy=1328,839
AudioEffectEnvelope      AENV1;          //xy=1481,800
AudioMixer4              filter1blockMixer; //xy=1487,492
AudioEffectEnvelope      AENV2;          //xy=1486,885
AudioMixer4              filter2blockMixer; //xy=1503,1183
AudioAmplifier           waveshapePreAmp2; //xy=1542,1081
AudioEffectWaveshaper    waveshape2;     //xy=1545,1044
AudioAmplifier           waveshapePreAmp1; //xy=1549,584
AudioEffectWaveshaper    waveshape1;     //xy=1550,624
AudioEffectMultiply      AENV1post;      //xy=1599,729
AudioEffectMultiply      AENV2post;      //xy=1600,948
AudioSynthWaveformDc     HPFdc;          //xy=1613,843
AudioFilterStateVariable hpf1;           //xy=1740,781
AudioFilterStateVariable hpf2;           //xy=1744,901
AudioMixer4              voiceMainLeftmixer; //xy=1968,815
AudioMixer4              voiceMainRightmixer; //xy=1968,879
AudioAmplifier           voiceGainLeft;  //xy=2185,662
AudioOutputUSB           stereoOut;      //xy=2188,885
AudioAmplifier           voiceGainRight; //xy=2197,1043
AudioMixer4              oscilloscopeMixer; //xy=2212,827
AudioAnalyzePeak         peakLeft;       //xy=2238,621
AudioAnalyzePeak         peakRight;      //xy=2253,1089
AudioConnection          patchCord1(voiceFENVdc, filter1env);
AudioConnection          patchCord2(voiceFENVdc, filter2env);
AudioConnection          patchCord3(pitchBendDC, 0, osc1FMmixer, 1);
AudioConnection          patchCord4(pitchBendDC, 0, osc2FMmixer, 1);
AudioConnection          patchCord5(voicePENVdc, osc1PENV);
AudioConnection          patchCord6(voicePENVdc, osc2PENV);
AudioConnection          patchCord7(filter1env, 0, filter1modMixer, 0);
AudioConnection          patchCord8(filter2env, 0, filter2modMixer, 0);
AudioConnection          patchCord9(osc2PENV, 0, osc2FMmixer, 0);
AudioConnection          patchCord10(osc1PENV, 0, osc1FMmixer, 0);
AudioConnection          patchCord11(osc2FMmixer, 0, waveformRight, 0);
AudioConnection          patchCord12(waveformRight, 0, osc2waveMixer, 0);
AudioConnection          patchCord13(waveformRight, 0, osc2AMpost, 0);
AudioConnection          patchCord14(waveformRight, 0, multiplyOsc2XMOD, 0);
AudioConnection          patchCord15(waveformLeft, 0, osc1waveMixer, 0);
AudioConnection          patchCord16(waveformLeft, 0, osc1AMpost, 0);
AudioConnection          patchCord17(waveformLeft, 0, multiplyOsc1XMOD, 0);
AudioConnection          patchCord18(osc1FMmixer, 0, waveformLeft, 0);
AudioConnection          patchCord19(osc1XMODamt, 0, multiplyOsc1XMOD, 1);
AudioConnection          patchCord20(multiplyOsc1XMOD, 0, osc2AMmixer, 0);
AudioConnection          patchCord21(osc2XMODamt, 0, multiplyOsc2XMOD, 1);
AudioConnection          patchCord22(voiceRINGamt, 0, multiplyOsc1Ring, 1);
AudioConnection          patchCord23(voiceRINGamt, 0, mulitplyOsc2Ring, 1);
AudioConnection          patchCord24(multiplyOsc2XMOD, 0, osc1AMmixer, 0);
AudioConnection          patchCord25(osc2AMmixer, 0, osc2AMpost, 1);
AudioConnection          patchCord26(osc1AMmixer, 0, osc1AMpost, 1);
AudioConnection          patchCord27(osc2AMpost, 0, osc2waveMixer, 1);
AudioConnection          patchCord28(osc1AMpost, 0, osc1waveMixer, 1);
AudioConnection          patchCord29(voiceRingModwave, 0, multiplyOsc1Ring, 0);
AudioConnection          patchCord30(voiceRingModwave, 0, mulitplyOsc2Ring, 0);
AudioConnection          patchCord31(multiplyOsc1Ring, 0, osc1AMmixer, 1);
AudioConnection          patchCord32(mulitplyOsc2Ring, 0, osc2AMmixer, 1);
AudioConnection          patchCord33(osc1waveMixer, 0, ladder1, 0);
AudioConnection          patchCord34(osc1waveMixer, 0, filter1, 0);
AudioConnection          patchCord35(filter1modMixer, 0, ladder1, 1);
AudioConnection          patchCord36(filter1modMixer, 0, filter1, 1);
AudioConnection          patchCord37(noise1, 0, osc1waveMixer, 2);
AudioConnection          patchCord38(noise1, 0, osc2waveMixer, 2);
AudioConnection          patchCord39(pink1, 0, osc1waveMixer, 3);
AudioConnection          patchCord40(pink1, 0, osc2waveMixer, 3);
AudioConnection          patchCord41(filter2modMixer, 0, ladder2, 1);
AudioConnection          patchCord42(filter2modMixer, 0, filter2, 1);
AudioConnection          patchCord43(osc2waveMixer, 0, ladder2, 0);
AudioConnection          patchCord44(osc2waveMixer, 0, filter2, 0);
AudioConnection          patchCord45(filter1, 0, filter1blockMixer, 0);
AudioConnection          patchCord46(filter1, 1, filter1blockMixer, 1);
AudioConnection          patchCord47(filter1, 2, filter1blockMixer, 2);
AudioConnection          patchCord48(ladder1, 0, filter1blockMixer, 3);
AudioConnection          patchCord49(filter2, 0, filter2blockMixer, 0);
AudioConnection          patchCord50(filter2, 1, filter2blockMixer, 1);
AudioConnection          patchCord51(filter2, 2, filter2blockMixer, 2);
AudioConnection          patchCord52(ladder2, 0, filter2blockMixer, 3);
AudioConnection          patchCord53(voiceAENVdc, AENV1);
AudioConnection          patchCord54(voiceAENVdc, AENV2);
AudioConnection          patchCord55(AENV1, 0, AENV1post, 1);
AudioConnection          patchCord56(filter1blockMixer, waveshapePreAmp1);
AudioConnection          patchCord57(AENV2, 0, AENV2post, 1);
AudioConnection          patchCord58(filter2blockMixer, waveshapePreAmp2);
AudioConnection          patchCord59(waveshapePreAmp2, waveshape2);
AudioConnection          patchCord60(waveshape2, 0, AENV2post, 0);
AudioConnection          patchCord61(waveshapePreAmp1, waveshape1);
AudioConnection          patchCord62(waveshape1, 0, AENV1post, 0);
AudioConnection          patchCord63(AENV1post, 0, hpf1, 0);
AudioConnection          patchCord64(AENV2post, 0, hpf2, 0);
AudioConnection          patchCord65(HPFdc, 0, hpf1, 1);
AudioConnection          patchCord66(HPFdc, 0, hpf2, 1);
AudioConnection          patchCord67(hpf1, 0, voiceMainLeftmixer, 0);
AudioConnection          patchCord68(hpf1, 0, voiceMainRightmixer, 0);
AudioConnection          patchCord69(hpf2, 0, voiceMainLeftmixer, 1);
AudioConnection          patchCord70(hpf2, 0, voiceMainRightmixer, 1);
AudioConnection          patchCord71(voiceMainLeftmixer, voiceGainLeft);
AudioConnection          patchCord72(voiceMainRightmixer, voiceGainRight);
AudioConnection          patchCord73(voiceGainLeft, peakLeft);
AudioConnection          patchCord74(voiceGainLeft, 0, stereoOut, 0);
AudioConnection          patchCord75(voiceGainLeft, 0, oscilloscopeMixer, 0);
AudioConnection          patchCord76(voiceGainRight, 0, stereoOut, 1);
AudioConnection          patchCord77(voiceGainRight, peakRight);
AudioConnection          patchCord78(voiceGainRight, 0, oscilloscopeMixer, 1);
// GUItool: end automatically generated code

*/
