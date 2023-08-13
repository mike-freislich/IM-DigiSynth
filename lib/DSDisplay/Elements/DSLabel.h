#ifndef DSLABEL_H
#define DSLABEL_H

#include <DSElement.h>
#include <ILI9341_t3n.h>

class DSLabel : public DSElement, public Docker
{
public:
    DSLabel(ILI9341_t3n *lcd, String(label),
            Color elementColor = Color(ILI9341_BLACK, ILI9341_WHITE, ILI9341_BLACK), DSElement *parent = nullptr)
        : DSElement(lcd, label, Rect(), parent), Docker(left, yCentre)
    {
        setColors(elementColor);
        box = _textBox;
        _anchor = AnchorPosition(HorizontalPosition::left, VerticalPosition::middle);
    }

    ~DSLabel()
    {
    }

    void render()
    {
        if (getShouldRedraw())
        {
            dock(this, _parent);
            drawLabel(box.getCentre());
            //logToScreen("x " + String(box.x) + ", w " + String(box.width));
        }
        DSElement::render();
    }

    void setLabel(String s)
    {
        label = s;
        _textBox = getTextBox(label);
        docked = false;        
    }
};

#endif