#ifndef DSSPLASHSCENE_H
#define DSSPLASHSCENE_H

#include <DSScene.h>
#include <DSButton.h>

class DSSceneSplash : public DSScene
{
public:
    DSSceneSplash(ILI9341_t3n *lcd) : DSScene(lcd)
    {        
        DSButton button(lcd, "SPLASH");
        button.setBounds(Rect(lcd->width()/2-50, lcd->height()/2-25, 100, 50), this);
        addElement(new DSButton(lcd, "SPLASH"));
        this->setSceneName(String(F("SPLASH")));         
        color.background = 0;        
    }        
};

#endif