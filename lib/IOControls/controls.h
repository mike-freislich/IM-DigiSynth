#ifndef DS_CONTROLS_H
#define DS_CONTROLS_H

#include <Arduino.h>
#include "PSMaths.h"
#include "DigitalIO.h"
#include "AnalogMux.h"
#include <InputButton.h>
#include <InputPotentiometer.h>
#include <InputEncoder.h>
#include <controlPins.h>

class Controls
{
public:
    Potentiometer pots[NUMPOTS] = {
        Potentiometer(PIN_POTA),
        Potentiometer(PIN_POTB),
        Potentiometer(PIN_POTC),
        Potentiometer(PIN_POTD)};

    Button buttons[NUMBUTTONS] = {
        Button(PIN_ROTARY1_BUTTON),
        Button(PIN_ROTARY2_BUTTON),
        Button(PIN_TACTBUTTON_1),
        Button(PIN_TACTBUTTON_2)};

    Encoder encoders[NUMENCODERS] = {
        Encoder(PIN_ENC1_A, PIN_ENC1_B, encoder1ISRA, encoder1ISRB),
        Encoder(PIN_ENC2_A, PIN_ENC2_B, encoder2ISRA, encoder2ISRB)};

    Controls() { analogReadAveraging(POT_AVERAGE_SAMPLING); }

    void update()
    {
        updatePots();
        updateButtons();
        updateEncoders();
    }

protected:
    void updatePots()
    {
        for (int i = 0; i < NUMPOTS; i++)
            pots[i].update();
    }
    void updateButtons()
    {
        for (int i = 0; i < NUMBUTTONS; i++)
            buttons[i].update();
    }
    void updateEncoders()
    {
        for (int i = 0; i < NUMENCODERS; i++)
            encoders[i].update();
    }
} controls;

void encoder1ISRA() { controls.encoders[0].isrA(); }
void encoder1ISRB() { controls.encoders[0].isrB(); }
void encoder2ISRA() { controls.encoders[1].isrA(); }
void encoder2ISRB() { controls.encoders[1].isrB(); }

#endif