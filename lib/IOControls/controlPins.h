#ifndef CONTROL_PINS_H
#define CONTROL_PINS_H

#include <Arduino.h>

#pragma region MUX POTS

// envelope board (v1)
#define POT_V1_Fader1 0
#define POT_V1_Fader2 1
#define POT_V1_Fader3 2
#define POT_V1_Fader4 3
#define POT_V1_EnvLevel 4
#define POT_V1_Cutoff 5
#define POT_V1_Resonance 6

// vco board (v1)
#define POT_V1_Waveform 7
#define POT_V1_PulseWidth 8
#define POT_V1_Tune 9
#define POT_V1_Mix 10
#define POT_V1_RingLevel 11
#define POT_V1_RingFreq 12
#define POT_V1_NoiseLevel 13
#define POT_V1_Pan 14
#define POT_V1_ShaperWave 15
#define POT_V1_ShaperCurve 16
#define POT_V1_XmodSend 17

// display board : data pots
#define POT_Data1 18
#define POT_Data2 19
#define POT_Data3 20
#define POT_Data4 21

const uint8_t PROGMEM potPinsActive[] = {0,1,2,3, 32,33,34, 36,37,38,39};

#pragma endregion

#pragma region Buttons

// BUTTONS
#define BTN_V1_NoiseType 10
#define BTN_V1_EnvSelect 3
#define BTN_Shift 1
#define PIN_ROTARY1_BUTTON 0
//#define PIN_ROTARY2_BUTTON 4

const uint8_t PROGMEM buttonPinsActive[] = {BTN_Shift, PIN_ROTARY1_BUTTON}; //, BTN_V1_EnvSelect, BTN_V1_NoiseType};
//const uint16_t BUTTONMASK = word(B00000000, B00000011);
// uint16_t GET_BUTTONMASK()
// {
//     uint16_t mask = 0;
//     for (uint8_t i = 0; i < sizeof(buttonPinsActive); i ++)
//     {        
//         mask = bitSet(mask, buttonPinsActive[i]);
//     }
// }

#pragma endregion

// ENCODERS
#define NUMENCODERS 1
#define PIN_ENC1_A A7
#define PIN_ENC1_B A6
//#define PIN_ENC2_A A8
//#define PIN_ENC2_B A9

#endif