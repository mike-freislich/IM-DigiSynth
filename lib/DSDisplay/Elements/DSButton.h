#ifndef DSBUTTON_H
#define DSBUTTON_H

#include <DSElement.h>
#include <ILI9341_t3n.h>

class DSButton : public DSElement
{
public:
    DSButton(ILI9341_t3n *lcd, String label, Rect boundingBox,
             Color elementColor = Color(0, ILI9341_WHITE, 0),
             DSElement *parent = NULL) : DSElement(lcd, label, boundingBox, parent)
    {
        setColors(elementColor); 
        _anchor.horizontal = centre;
    }
    ~DSButton()
    {
    }

    void render()
    {
        if (getShouldRedraw())
        {
            drawBackground();
            drawBorder();                        
            drawLabel(box.getCentre());
            setShouldRedraw(false);
        }
        DSElement::render();
    }
};

#endif