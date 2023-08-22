#ifndef DS_PARAMETERELEMENT_H
#define DS_PARAMETERELEMENT_H

#include "DSElement.h"
#include "PSParameter.h"
#include "FastTrig.h"

#define PARM_ATTACK 0
#define PARM_HOLD 1
#define PARM_DECAY 2
#define PARM_SUSTAIN 3
#define PARM_RELEASE 4

class DSParameterElement : public DSElement, public Docker
{
public:
    DSParameterElement(ILI9341_t3n *lcd, PSParameter *parm, DSElement *parent = nullptr)
        : DSElement(lcd, parm->name, Rect(0, 0, 50, 50), parent), Docker(DockPositionX::xNone, DockPositionY::yNone)
    {
        color.fill = 0;
        color.border = ILI9341_DARKGREY;
        color.text = ILI9341_WHITE;
        color.control = CL(0, 30, 0);
        color.controlValue = ILI9341_GREEN;
        color.textHighlighted = ILI9341_YELLOW;

        parameter = parm;
        radius = box.width / 2 - 3;
        this->_updateDelay = 100;
        setLabel("PARM");

        DSLabel *l = new DSLabel(lcd, parameter->name, color);
        l->setFont(FONT_UI_PARAMETER);
        l->setDockOptions(DockPositionX::xCentre, DockPositionY::yCentre);
        l->setAnchors(AnchorPosition(HorizontalPosition::centre, VerticalPosition::middle));

        this->addElement(l);
    }
    ~DSParameterElement() {}

    void render()
    {
        if ((getShouldRedraw() && this->updateTimerReady()) || parameter->update())
        {
            dock(this, _parent);
            drawDial();
            DSElement::render();
        }
    }

    void drawDial()
    {
        int i, v = parameter->getScaled(arcStart, arcEnd);
        int iv = parameter->getScaledInput(arcStart, arcEnd);
        Point p = box.getCentre();        

        for (i = v; i > arcEnd; i--)
            _lcd->fillCircle((radius * (icos(i))) + p.x, -(radius * (isin(i))) + p.y - 2, thickness / 2, color.control);

        for (i = arcStart; i > v; i--)
            _lcd->fillCircle((radius * (icos(i))) + p.x, -(radius * (isin(i))) + p.y - 2, thickness / 2, color.controlValue);

        if (iv != v)
            _lcd->fillCircle((radius * (icos(iv))) + p.x, -(radius * (isin(iv))) + p.y - 2, thickness / 2, ILI9341_WHITE);

        String newString = String((int)parameter->getValue()) + parameter->units;

        if (lastValue.length() > 0)
            drawText(p.x, p.y + box.height / 2 - 4, lastValue, FONT_UI_PARAMETER, ILI9341_BLACK);
        drawText(p.x, p.y + box.height / 2 - 4, newString, FONT_UI_PARAMETER, color.textHighlighted);
        lastValue = newString;
    }

    // int sinInc(int degrees)
    // {
    //     sinDegrees = (sinDegrees + degrees) % 180;
    //     setValue(parameter->getMax() - (parameter->getMax() * isin(sinDegrees)));
    //     return getValue();
    // }

private:
    PSParameter *parameter;
    String lastValue;
    uint16_t radius, sinDegrees;
    const float arcSweep = 270;
    const float arcStart = 180.0 + 90.0 - ((360.0 - arcSweep) / 2.0);
    const float arcEnd = -(90.0 - (360.0 - arcSweep) / 2.0);
    const float thickness = 2;    
};

#endif