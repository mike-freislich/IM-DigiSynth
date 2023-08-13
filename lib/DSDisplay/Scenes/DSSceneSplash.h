#ifndef DSSPLASHSCENE_H
#define DSSPLASHSCENE_H

#include <DSScene.h>
#include <DSButton.h>

class DSSceneSplash : public DSScene
{
private:

public:
    DSSceneSplash(ILI9341_t3n *lcd) : DSScene(lcd)
    {        
        addElement(new DSButton(lcd, "SPLASH", Rect(320/2-50, 240/2-25, 100, 50),
        DSElement::Color(ILI9341_BLACK, ILI9341_WHITE, ILI9341_BLACK)));  
        color.background = 0; //lcd->color565(200, 96, 120);
        this->setSceneName(String(F("SPLASH")));                
    }

    ~DSSceneSplash()
    {
        DSScene::~DSScene();
    }
    
};

#endif