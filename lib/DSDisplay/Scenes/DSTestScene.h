#ifndef DS_TESTSCENE_H
#define DS_TESTSCENE_H

#include <DSScene.h>
#include <DSElements.h>

class DSTestScene : public DSScene
{
public:
    DSLabel *text;
    DSPanel *cp;
    DSPanel *p;

    DSTestScene(ILI9341_t3n *lcd) : DSScene(lcd)
    {
        this->setSceneName(F("This is a test"));

        p = new DSPanel(_lcd, "panel main", Dimensions(100,100), this);
        p->color.background = ILI9341_WHITE;        
        p->setBoundsPosition(100,100);
        p->show();
        addElement(p);

        cp = new DSPanel(_lcd, "child sub", Dimensions(20,20), p);        
        cp->color.background = ILI9341_PINK;
        cp->color.border = ILI9341_BLACK;        
        cp->setBoundsPosition(0,0);
        cp->dock(centre, bottom);
        cp->show();
        p->addChild(cp);
        
        text = new DSLabel(lcd, F("The labeL"));
        text->color.text = ILI9341_BLUE;
        text->color.border = ILI9341_DARKGREEN;
        text->dock(centre, middle);
        text->setAnchorPosition(TL_DATUM);
        text->show();
        cp->addChild(text);
    }

    void render() override
    {
        DSScene::render();
    }
};

#endif