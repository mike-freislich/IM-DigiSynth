#pragma once
#include <Audio.h>
#include "AudioBlend.h"

// GUItool: begin automatically generated code
AudioSynthWaveformModulated auWave_oscA; // xy=283,269
AudioSynthWaveformModulated auWave_oscB; // xy=283,373
AudioEffectMultiply auMult_xmod;         // xy=485,343
AudioBlend auBlend_xmod;                 // xy=576.5,202.00003051757812
AudioOutputUSB usb1;                     // xy=807.1666870117188,261.5

AudioConnection patchCord1(auWave_oscA, 0, auMult_xmod, 0);
AudioConnection patchCord2(auWave_oscA, 0, auBlend_xmod, 0);
AudioConnection patchCord3(auWave_oscB, 0, auMult_xmod, 1);
AudioConnection patchCord4(auMult_xmod, 0, auBlend_xmod, 1);
AudioConnection patchCord5(auBlend_xmod, 0, usb1, 0);
AudioConnection patchCord6(auBlend_xmod, 0, usb1, 1);

// GUItool: end automatically generated code
