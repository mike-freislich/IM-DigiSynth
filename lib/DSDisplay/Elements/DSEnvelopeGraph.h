#ifndef DS_ENVELOPE_H
#define DS_ENVELOPE_H

#include "PSEnvelope.h"
#include "DSElement.h"
#include "DSGraphics.h"
#include "DSSceneEnvelope.h"


class DSEnvelopeGraph : public DSElement
{
public:
    DSEnvelopeGraph(ILI9341_t3n *lcd, String label, PSParameterVector plist)
        : DSElement(lcd, label)
    {
        setBounds(Rect(0,25,317,150));
        _parms = plist;        
        color.border = ILI9341_WHITE;        
        color.background = ILI9341_BLACK;
        color.control = ILI9341_WHITE;
        color.controlValue = ILI9341_GREENYELLOW;                
    }

    ~DSEnvelopeGraph() { DSElement::~DSElement(); }

    void render()
    {
        if (didChange && visible)
        {
            drawBorder();
            drawGraph();
            DSElement::render();
        }
    }

    void drawGraph()
    {
        // set up the graph area
        const float sustainArea = 0.2;
        const uint8_t padding = 6;
        
        Rect gbox = dockedBounds();
        gbox.x += padding;
        gbox.y += padding;
        gbox.width -= padding * 2;
        gbox.height -= padding * 2;

        // calculate the total milliseconds in the envelope (sustain is 0)
        float totalMillis = _parms[PSP_ENV_ATTACK]->getMax() + _parms[PSP_ENV_HOLD]->getMax() + _parms[PSP_ENV_DECAY]->getMax() + _parms[PSP_ENV_RELEASE]->getMax();

        uint16_t lineColor, dotColor;
        if (didChange && polyLine != nullptr)
        {
            lineColor = 0;
            dotColor = 0;
        }
        else
        {
            if (polyLine == nullptr)
                polyLine = new PolyLine(6);
            else
                polyLine->reset();

            lineColor = color.control;
            dotColor = color.controlValue;
            float factor = gbox.width * (1 - sustainArea);

            polyLine->add(gbox.x, gbox.y + gbox.height);                                                                                                // start
            polyLine->add(_parms[PSP_ENV_ATTACK]->getValue() / totalMillis * factor, -gbox.height);                                                     // attack
            polyLine->add(_parms[PSP_ENV_HOLD]->getValue() / totalMillis * factor, 0);                                                                  // hold
            polyLine->add(_parms[PSP_ENV_DECAY]->getValue() / totalMillis * factor, (gbox.height * (100 - _parms[PSP_ENV_SUSTAIN]->getValue()) / 100)); // decay
            polyLine->add(sustainArea * gbox.width, 0);                                                                                                 // sustain
            polyLine->add(_parms[PSP_ENV_RELEASE]->getValue() / totalMillis * factor, (gbox.height * (_parms[PSP_ENV_SUSTAIN]->getValue()) / 100));                     // release
        }

        Line l;
        for (int i = 0; i < polyLine->lineCount; i++)
        {
            l = polyLine->lineAtIndex(i);
            lcd->drawLine(l.start.x, l.start.y, l.end.x, l.end.y, lineColor);
            lcd->fillCircle(l.start.x, l.start.y, 3, dotColor);
        }
        lcd->fillCircle(l.end.x, l.end.y, 3, dotColor);

        if (didChange)
        {
            didChange = false;
            drawGraph();
        }
    }

protected:
private:
    PSParameterVector _parms;
    PolyLine *polyLine = nullptr;    
};

#endif