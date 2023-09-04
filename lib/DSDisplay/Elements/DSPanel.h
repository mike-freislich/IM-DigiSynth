#ifndef DSPANEL_H
#define DSPANEL_H

#include <DSElement.h>
#include <DSColor.h>
#include <ILI9341_t3n.h>

class DSPanel : public DSElement
{
protected:
public:
    DSPanel(ILI9341_t3n *lcd, String name, Dimensions dimensions, DSBounds *parentSpace = nullptr) : DSElement(lcd, name)
    {
        color.background = COLOR_TOP_PANEL;
        setBounds(Rect(0,0,dimensions.width, dimensions.height));
        if (parentSpace != nullptr)
            attachToSpace(parentSpace);
    }

    void render() override
    {
        if (visible && didChange)
        {
            this->drawBackground();
        }
        DSElement::render();
    }

};

#endif