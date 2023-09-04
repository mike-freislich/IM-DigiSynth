#ifndef DSVOICESCENE_H
#define DSVOICESCENE_H

#include <DSScene.h>
#include <DSButton.h>
#include "DSOscilloscope.h"
#include "patching.h"

class DSSceneVoice : public DSScene
{
public:
    DSSceneVoice(ILI9341_t3n *lcd) : DSScene(lcd)
    {        
        color.background = 0;
        this->setSceneName(String("VOICE"));        
        scope = new DSOscilloscope(lcd, "scope");
        scope->setBoundsPosition(0, 110);
        scope->dock(centre, noneV);    
        scope->inputConnection(&oscilloscopeMixer);
        oscilloscopeMixer.gain(0, 0.5);
        oscilloscopeMixer.gain(1, 0.5);        
        addElement(scope);

        DSPanel *stripe = new DSPanel(lcd, "", Dimensions(260, 5), this);
        stripe->color.background = ILI9341_GREEN;        
        stripe->setBoundsPosition(0, 100);
        stripe->dock(centre, noneV);
        stripe->show();       
        addElement(stripe); 
    }    

private:
    DSOscilloscope *scope;
};

#endif