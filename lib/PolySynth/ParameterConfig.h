#ifndef DS_PARAMETER_CONFIG_H
#define DS_PARAMETER_CONFIG_H

#include "Parameters.h"
#include "patching.h"
#include "waveshaperTables.h"

#pragma region WAVE SHAPER




#pragma endregion

#pragma region ENVELOPES

// DCO

// MasterAmpEnvelope

// FilterEnvelope

// PitchEnvelope

#pragma endregion


#pragma region LFOs

// FILTER LFO

// PITCH LFO

// AMP LFO


#pragma endregion


/*


SynthParameters
    LFO1 --> AudioStream { OSC(osc1modMixer, osc2modMixer), VCF(filter1modMixer, filter2modMixer), VCFamt(filterEGdc) }
    LFO2 --> AudioStream { OSC(osc1modMixer, osc2modMixer), VCF(filter1modMixer, filter2modMixer), VCFamt(filterEGdc) }
    LFO3 --> 
    LFO4 -->
    A-ENV       --> { mastergainAmpLeft, mastergainAmpRight }
    F-ENV       --> { filter1modMixer, filter2modMixer }
    P-ENV       --> { osc1modMixer, osc2modMixer }
    A-ENV-AMT   <--
    F-ENV-AMT   <--
    P-ENV-AMT   <--

    F-Freq  <-- 
    F-Res   <--


AudioEffectEnvelope.sustain




*/


#endif