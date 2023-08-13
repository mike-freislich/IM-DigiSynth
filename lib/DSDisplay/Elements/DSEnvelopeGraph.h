#ifndef DS_ENVELOPE_H
#define DS_ENVELOPE_H

#include "DSElement.h"
#include "Parameters.h"
#include "DSGraphics.h"
#include "DSSceneEnvelope.h"

class DSEnvelopeGraph : public DSElement
{
public:
    DSEnvelopeGraph(ILI9341_t3n *lcd, String label, EnvParameters *plist, DSElement *parent = nullptr)
        : DSElement(lcd, label, Rect(3, 25, 317, 150), parent)
    {
        _parms = plist;
        color.border = ILI9341_WHITE;
        color.fill = ILI9341_BLACK;
        color.control = ILI9341_WHITE;
        color.controlValue = ILI9341_GREENYELLOW;     
        this->_updateDelay = 1000/30; // 15 FPS max
    }

    ~DSEnvelopeGraph() { DSElement::~DSElement(); }

    void render()
    {
        if (this->getShouldRedraw() && this->updateTimerReady())
        {
            drawBorder();
            drawGraph();
            DSElement::render();
        }
        
    }

    // void clear() {    
    //     drawGraph(true);    
    //     setShouldRedraw(true);
    // }

    void drawGraph()
    {           
        // set up the graph area
        const float sustainArea = 0.2;
        const uint8_t padding = 6;
        Rect gbox(box.x + padding, box.y + padding, box.width - padding * 2, box.height - padding * 2);

        // calculate the total milliseconds in the envelope (sustain is 0)
        float totalMillis = _parms->attack->getMax() + _parms->hold->getMax() + _parms->decay->getMax() + _parms->release->getMax();

        uint16_t lineColor, dotColor;        
        if (getShouldRedraw() && polyLine != nullptr)
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

            polyLine->add(gbox.x, gbox.y + gbox.height);                                                                                  // start
            polyLine->add(_parms->attack->getValue() / totalMillis * factor, -gbox.height);                                               // attack
            polyLine->add(_parms->hold->getValue() / totalMillis * factor, 0);                                                            // hold
            polyLine->add(_parms->decay->getValue() / totalMillis * factor, (gbox.height * (100 - _parms->sustain->getValue())/100)); // decay
            polyLine->add(sustainArea * gbox.width, 0);                                                                                   // sustain
            polyLine->add(_parms->release->getValue() / totalMillis * factor, (gbox.height * (_parms->sustain->getValue())/100));     // release
        }

        Line l;
        for (int i = 0; i < polyLine->lineCount; i++)
        {
            l = polyLine->lineAtIndex(i);
            _lcd->drawLine(l.start.x, l.start.y, l.end.x, l.end.y, lineColor);
            _lcd->fillCircle(l.start.x, l.start.y, 3, dotColor);
        }
        _lcd->fillCircle(l.end.x, l.end.y, 3, dotColor);                                           

        if (getShouldRedraw()) {
            setShouldRedraw(false);
            drawGraph();
        }
    }

protected:
private:
    EnvParameters *_parms;
    PolyLine *polyLine = nullptr;
};

#endif