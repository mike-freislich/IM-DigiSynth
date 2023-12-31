#ifndef DS_PARAMETERDIAL_H
#define DS_PARAMETERDIAL_H

#include "DSElements.h"
#include "PSParameter.h"
#include "FastTrig.h"

#define PARM_ATTACK 0
#define PARM_HOLD 1
#define PARM_DECAY 2
#define PARM_SUSTAIN 3
#define PARM_RELEASE 4

class DSParameterDial : public DSElement
{
public:
    DSParameterDial(ILI9341_t3n *lcd, PSParameter *parm, DSElement *parent = nullptr) : DSElement(lcd, parm->name)
    {        
        setBounds(0, 0, 50, 50);
        color.background = 0;
        color.border = ILI9341_DARKGREY;
        color.text = ILI9341_WHITE;
        color.control = CL(0, 30, 0);
        color.controlValue = ILI9341_GREEN;
        color.textHighLight = ILI9341_YELLOW;

        parameter = parm;        
        radius = boundsRect.width / 2 - 3;
        setName("PARM");

        labelName = new DSLabel(lcd, parameter->name);
        labelName->setFont(FONT_UI_PARAMETER);
        labelName->setBoundsPosition(0, -2);
        labelName->setAnchorPosition(CC_DATUM);
        labelName->dock(centre, middle);
        this->addChild(labelName);

        labelValue = new DSLabel(lcd, parameter->getValue());
        labelValue->setFont(FONT_UI_PARAMETER);
        labelValue->color.text = ILI9341_YELLOW;
        labelValue->dock(centre, bottom);
        labelValue->setAnchorPosition(CC_DATUM);
        this->addChild(labelValue);

    }
    ~DSParameterDial() {}

    void render()
    {
        if ((visible && didChange) || parameter->updateFromControl())
        {
            drawDial();
            DSElement::render();
        }
    }

private:
    PSParameter *parameter;
    String lastValue;
    uint16_t radius, sinDegrees;
    DSLabel *labelValue, *labelName;
    const float arcSweep = 270;
    const float arcStart = 180.0 + 90.0 - ((360.0 - arcSweep) / 2.0);
    const float arcEnd = -(90.0 - (360.0 - arcSweep) / 2.0);
    const float thickness = 2;

    void drawDial()
    {
        int v = parameter->getScaled(arcStart, arcEnd);
        int iv = parameter->getScaledInput(arcStart, arcEnd);

        Rect box = dockedBounds();
        Point p = box.getCentre();

        drawArc(p, v, arcEnd, color.control);
        drawArc(p, arcStart, v, color.controlValue);
        if (iv != v)
            drawInputPositionMarker(iv, p);

        String newString = String((int)parameter->getValue()) + parameter->units;                            
        labelValue->setName(newString);
        lastValue = newString;
    }

    void drawArc(Point p, int arcEnd, int arcStart, uint16_t color)
    {
        for (int i = arcEnd; i > arcStart; i--)
        {
            Point point((radius * (icos(i))) + p.x, -(radius * (isin(i))) + p.y - 2);
            lcd->fillCircle(point.x, point.y, thickness / 2, color);
        }
    }

    void drawInputPositionMarker(int inputValue, Point p, uint16_t color = ILI9341_WHITE)
    {
        Point point((radius * (icos(inputValue))) + p.x, -(radius * (isin(inputValue))) + p.y - 2);
        lcd->fillCircle(point.x, point.y, thickness / 2, color);
    }
};

#endif