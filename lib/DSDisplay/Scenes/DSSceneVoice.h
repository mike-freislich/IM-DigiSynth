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
        //DSElement::Color elementColor = DSElement::Color(COLOR_BORDER, ILI9341_WHITE, ILI9341_BLACK);                
        color.background = 0;
        this->setSceneName(String("VOICE"));

        scope = new DSOscilloscope(lcd, "scope");
        scope->setScene(this);        
        scope->inputConnection(&oscilloscopeMixer);
        oscilloscopeMixer.gain(0, 0.5);
        oscilloscopeMixer.gain(1, 0.5);
        addElement(scope);        
    }

    ~DSSceneVoice()
    {
        DSScene::~DSScene();
    }


    
private:
    DSOscilloscope *scope;
};

#endif