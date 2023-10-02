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
#include <vector>

typedef std::vector<Potentiometer> PotentiometerVector;
typedef std::vector<Button> ButtonVector;

class Controls
{
public:
    PotentiometerVector pots;
    ButtonVector buttons;

    Encoder encoders[NUMENCODERS] = {
        Encoder(PIN_ENC1_A, PIN_ENC1_B, encoder1ISRA, encoder1ISRB),
        Encoder(PIN_ENC2_A, PIN_ENC2_B, encoder2ISRA, encoder2ISRB)};

    Controls()
    {
    }

    void setup()
    {
        for (uint8_t i = 0; i < sizeof(potPinsActive); i++)
            pots.push_back(Potentiometer(potPinsActive[i], IO_EXPANSION));

        for (uint8_t x = 0; x < sizeof(buttonPinsActive); x++)
            buttons.push_back(Button(buttonPinsActive[x], IO_EXPANSION));        
    }

    void update()
    {
        updatePots();
        updateButtons();
        updateEncoders();
    }

protected:
    void updatePots()
    {
        for (uint8_t i = 0; i < sizeof(potPinsActive); i++)
            pots[i].update();
    }
    void updateButtons()
    {        
        for (uint8_t i = 0; i < sizeof(buttonPinsActive); i++)        
            buttons[i].update();                    
    }
    void updateEncoders()
    {
        for (uint8_t i = 0; i < NUMENCODERS; i++)
            encoders[i].update();
    }

} controls;

void encoder1ISRA() { controls.encoders[0].isrA(); }
void encoder1ISRB() { controls.encoders[0].isrB(); }
void encoder2ISRA() { controls.encoders[1].isrA(); }
void encoder2ISRB() { controls.encoders[1].isrB(); }

#endif