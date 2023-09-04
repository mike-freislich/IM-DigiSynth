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
    DSParameterDial(ILI9341_t3n *lcd, PSParameter *parm, DSElement *parent = nullptr)
        : DSElement(lcd, parm->name)
    {
        Rect box = Rect(0, 0, 50, 50);
        setBounds(box);
        color.background = 0;
        color.border = ILI9341_DARKGREY;
        color.text = ILI9341_WHITE;
        color.control = CL(0, 30, 0);
        color.controlValue = ILI9341_GREEN;
        textHighlightColor = ILI9341_YELLOW;

        parameter = parm;
        radius = box.width / 2 - 3;
        setName("PARM");

        DSLabel *l = new DSLabel(lcd, parameter->name);
        l->setFont(FONT_UI_PARAMETER);
        this->addChild(l);
    }
    ~DSParameterDial() {}

    void render()
    {
        if ((visible && didChange) || parameter->update())
        {
            drawDial();
            DSElement::render();
        }
    }

private:
    PSParameter *parameter;
    String lastValue;
    uint16_t radius, sinDegrees;
    const float arcSweep = 270;
    const float arcStart = 180.0 + 90.0 - ((360.0 - arcSweep) / 2.0);
    const float arcEnd = -(90.0 - (360.0 - arcSweep) / 2.0);
    const float thickness = 2;
    uint16_t controlColor, valueColor, textHighlightColor;

    void drawDial()
    {
        int v = parameter->getScaled(arcStart, arcEnd);
        int iv = parameter->getScaledInput(arcStart, arcEnd);

        Rect box = dockedBounds();
        Point p = box.getCentre();

        drawArc(p, v, arcEnd, controlColor);
        drawArc(p, arcStart, v, valueColor);
        if (iv != v)
            drawInputPositionMarker(iv, p);

        String newString = String((int)parameter->getValue()) + parameter->units;

        // TODO add the text back to DSParameterDial
        //if (lastValue.length() > 0)
        //    drawText(p.x, p.y + box.height / 2 - 4, lastValue, FONT_UI_PARAMETER, ILI9341_BLACK);
        //drawText(p.x, p.y + box.height / 2 - 4, newString, FONT_UI_PARAMETER, textHighlightColor);
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
        lcd->fillCircle(point.x, point.y, thickness / 2, ILI9341_WHITE);
    }
};

#endif