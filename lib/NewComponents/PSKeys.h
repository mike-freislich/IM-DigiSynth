#pragma once

/**
 * @brief PSKeys is an enum containing all key-identifier for modules, parameters, controllers and scenes
 *
 */

enum PSK
{
    NONE = 0,
#pragma region MODULES
    // ---------------------- MODULES ----------------------
    // Part A
    MOD_LFO1a,
    MOD_LFO2a,
    MOD_PENVa,
    MOD_AENVa,
    MOD_FENVa,
    MOD_VCFa,
    MOD_VCOa,
    MOD_VCAa,
    MOD_NOISEa,
    MOD_RINGMODa,
    MOD_FOLDa,

    // Part B
    MOD_LFO1b,
    MOD_LFO2b,
    MOD_PENVb,
    MOD_AENVb,
    MOD_FENVb,
    MOD_VCFb,
    MOD_VCOb,
    MOD_VCAb,
    MOD_NOISEb,
    MOD_RINGMODb,
    MOD_FOLDb,

#pragma endregion

#pragma region parameters
    // ---------------------- PARAMETERS ----------------------
    // Envelope
    PARM_AENVa_ATTACK,
    PARM_AENVa_HOLD,
    PARM_AENVa_DECAY,
    PARM_AENVa_SUSTAIN,
    PARM_AENVa_RELEASE,
    PARM_AENVa_AMOUNT,
    PARM_AENVb_ATTACK,
    PARM_AENVb_HOLD,
    PARM_AENVb_DECAY,
    PARM_AENVb_SUSTAIN,
    PARM_AENVb_RELEASE,
    PARM_AENVb_AMOUNT,

    PARM_PENVa_ATTACK,
    PARM_PENVa_HOLD,
    PARM_PENVa_DECAY,
    PARM_PENVa_SUSTAIN,
    PARM_PENVa_RELEASE,
    PARM_PENVa_AMOUNT,
    PARM_PENVb_ATTACK,
    PARM_PENVb_HOLD,
    PARM_PENVb_DECAY,
    PARM_PENVb_SUSTAIN,
    PARM_PENVb_RELEASE,
    PARM_PENVb_AMOUNT,

    PARM_FENVa_ATTACK,
    PARM_FENVa_HOLD,
    PARM_FENVa_DECAY,
    PARM_FENVa_SUSTAIN,
    PARM_FENVa_RELEASE,
    PARM_FENVa_AMOUNT,
    PARM_FENVb_ATTACK,
    PARM_FENVb_HOLD,
    PARM_FENVb_DECAY,
    PARM_FENVb_SUSTAIN,
    PARM_FENVb_RELEASE,
    PARM_FENVb_AMOUNT,

    // LFO
    PARM_LFO_SHAPE,
    PARM_LFO_FREQ,
    PARM_LFO_LEVEL,

    // Ring Modulation
    PARM_RING_LEVEL,
    PARM_RING_FREQ,
    PARM_RING_WAVE,
#pragma endregion

#pragma region CONTROLLERS
    // ---------------------- CONTROLLERS ----------------------
    // USAGE : name = pinNumber
    CTRL_POT_DataA = 7,
    CTRL_POT_DataB = 8,
    CTRL_POT_DataC = 9,
    CTRL_POT_DataD = 10,
    CTRL_ROTARY_DataG_pA = 11,
    CTRL_ROTARY_DataG_pB = 12,
    CTRL_BTN_DataG = 13,
    CTRL_BTN_Shift = 14,
    CTRL_POT1 = 71,
    CTRL_POT2 = 2,
    CTRL_POT3 = 32,
    CTRL_POT4 = 4,
    CTRL_POT5 = 5,
    CTRL_POT6 = 6,

#pragma endregion

#pragma region SCENES
    // ---------------------- SCENES ----------------------
    SCN_ENVELOPE

#pragma endregion
};

struct Range
{
    Range(float min, float max) : min(min), max(max) {}
    float min;
    float max;
};