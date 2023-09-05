#ifndef DSSCENE_H
#define DSSCENE_H

#include <ILI9341_t3n.h>
#include <vector>
#include <XPT2046_Touchscreen.h>

#include <DSButton.h>
#include <DSPanel.h>
#include <DSLabel.h>

#include <DSVerticalMeter.h>
#include <DSColor.h>
#include "PSParameter.h"
#include <DSBounds.h>

class DSScene : public DSBounds
{

public:
    DSScene(ILI9341_t3n *lcd) : DSBounds()
    {
        _lcd = lcd;
        int offsetx = 3;
        setBounds(offsetx, 0, lcd->width() - offsetx, lcd->height());

        DSElement *panel = new DSPanel(_lcd, "TOPBAR", Dimensions(this->getBounds().width, 25), this);
        panel->color.background = ILI9341_BLUE;
        addElement(panel);

        _sceneName = new DSLabel(_lcd, ".");
        _sceneName->setFont(FONT_UI_HEADING);
        _sceneName->dock(left, middle, 3);
        panel->addChild(_sceneName);

        _verticalMeter = new DSVerticalMeter(_lcd, "");
        _verticalMeter->setBounds(Rect(0, 0, 25, 25));
        panel->addChild(_verticalMeter);
        _verticalMeter->dock(right, middle);
    }

    DSVerticalMeter *getAudioMeter() { return _verticalMeter; }
    virtual ~DSScene() { elements.clear(); }

    virtual void render()
    {
        if (visible)
        {
            for (const auto e : elements)
                e->render();
        }
    }

    DSElement *addElement(DSElement *element)
    {
        element->setDidChange();
        element->attachToSpace((DSBounds *)this);
        elements.push_back(element);
        return element;
    }

    void clear()
    {
        if (visible)
        {
            for (auto e : elements)
            {
                e->setDidChange();
            }
        }
        _lcd->fillScreen(color.background);
    }

    void setSceneName(String name) { _sceneName->setName(name); }
    uint16_t width() { return _lcd->height(); }
    uint16_t height() { return _lcd->width(); }
    PSParameterVector getParameters() { return _parms; };

    DSElement *getElement(String s)
    {
        for (auto e : elements)
        {
            if (e->getName() == s)
                return e;
        }
        return nullptr;
    }

    virtual void show()
    {
        visible = true;
        for (auto e : elements)
            e->show();
    }

    virtual void hide()
    {
        visible = false;
        for (auto e : elements)
            e->hide();
    }

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

typedef std::vector<DSScene *> DSSceneVector;

#endif