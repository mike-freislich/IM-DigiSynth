#ifndef DSSCENE_H
#define DSSCENE_H

#include <ILI9341_t3n.h>
#include <vector>
#include <XPT2046_Touchscreen.h>
#include <DSElement.h>
#include <DSButton.h>
#include <DSPanel.h>
#include <DSLabel.h>
#include <DSVerticalMeter.h>
#include <DSColor.h>
#include "PSParameter.h"

class DSScene
{

public:
    DSScene(ILI9341_t3n *lcd) : _lcd(lcd)
    {
        DSElement *panel = new DSPanel(_lcd, "TOPBAR", Rect(2, 0, 318, 25), DSElement::Color(COLOR_BORDER, COLOR_HEADING_FILL, COLOR_BACKGROUND));
        DSElement *heading = new DSLabel(_lcd, ".", DSElement::Color(0, 0, ILI9341_WHITE));

        heading->setFont(FONT_UI_HEADING);
        heading->setAnchors(AnchorPosition(left, middle));
        _sceneName = heading;
        panel->addElement(heading);
        _verticalMeter = new DSVerticalMeter(_lcd, "", panel);
        panel->addElement(_verticalMeter);
        addElement(panel);
    }

    DSVerticalMeter *getAudioMeter()
    {
        return _verticalMeter;
    }

    virtual ~DSScene()
    {
        elements.clear();
    };

    void render()
    {
        if (visible)
        {
            for (const auto e : elements)
                e->render();
        }
    }

    DSElement *addElement(DSElement *element)
    {
        element->setShouldRedraw(true);
        elements.push_back(element);
        return element;
    }

    void update(TS_Point point)
    {
        for (auto e : elements)
        {
            if (e->isPointInElement(point))
            {
                // Serial.printf(" %d \n");
            }
        }
    }

    void clear()
    {
        if (visible)
        {
            for (auto e : elements)
            {
                e->setShouldRedraw(true);
            }
        }
        _lcd->fillScreen(color.background);
    }

    void setSceneName(String name)
    {
        _sceneName->setLabel(name);
    }

    uint16_t width()
    {
        return _lcd->height();
    }

    uint16_t height()
    {
        return _lcd->width();
    }

    PSParameterVector getParameters() { return _parms; };

    DSElement *getElement(String s)
    {
        for (auto e : elements)
        {
            if (e->getLabel() == s)
                return e;
        }
        return nullptr;
    }

    virtual void show() { visible = true; }
    virtual void hide() { visible = false; }
    virtual bool isVisible() { return visible; }

protected:
    ILI9341_t3n *_lcd;
    DSElement *_sceneName;
    DSVerticalMeter *_verticalMeter;
    DSElementVector elements;
    PSParameterVector _parms;
    struct Color
    {
        uint16_t background = 0;
    } color;
    bool visible = false;
};

typedef vector<DSScene *> DSSceneVector;

#endif