#ifndef DSPANEL_H
#define DSPANEL_H

#include <DSElement.h>
#include <DSColor.h>
#include <ILI9341_t3n.h>

class DSPanel : public DSElement
{
protected:
public:
    DSPanel(ILI9341_t3n *lcd, String label, Rect boundingBox,
            Color elementColor = Color(COLOR_BACKGROUND, COLOR_TOP_PANEL, ILI9341_WHITE)) : DSElement(lcd, label, boundingBox)
    {
        setColors(elementColor);
    }

    ~DSPanel()
    {
    }

    void render()
    {
        if (getShouldRedraw())
        {
            drawBackground();
        }
        DSElement::render();
    }
};

#endif