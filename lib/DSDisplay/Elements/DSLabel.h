#ifndef DSLABEL_H
#define DSLABEL_H

#include <DSElement.h>

class DSLabel : public DSElement
{
public:
    DSLabel(ILI9341_t3n *lcd, String name) : DSElement(lcd, name)
    {
        setBounds(Rect(0, 0, textSize.width, textSize.height));
        color.border = ILI9341_WHITE;
        setDidChange();
    }

    void render() override
    {
        if (didChange)
        {
            DSElement::render();
            drawText();            
        }
    }
    void setAnchorPosition(uint8_t datum)
    {
        this->datum = datum;
        setDidChange();
    }

    void onDidChange() override
    {
        DSElement::onDidChange();
        lcd->setFont(font);
        setTextDimensions(name);
        Rect b = getBounds();
        setBounds(b.x, b.y, textSize.width, textSize.height);
        Serial.printf("SuperClass [%s]: onDidChange() called!\n", name.c_str());
    }

    void didSetParent(DSElement *parent) override
    {
        color.background = parent->color.background;
    }

protected:
    uint8_t datum = TL_DATUM;
    String lastValue;
    Rect lastBounds;

    void drawText()
    {
        Rect r = this->dockedBounds();
        // Serial.printf("BOUNDS %s, x=%d, y=%d, w=%d, h=%d\n", this->name.c_str(), r.x, r.y, r.width, r.height);

        lcd->setFont(font);
        lcd->setTextDatum(datum);

        if (lastValue.length() > 0 && lastValue != name)
            lcd->fillRect(lastBounds.x, lastBounds.y, lastBounds.width, lastBounds.height, color.background);                    
        
        lcd->setCursor(r.x, r.y);
        lcd->setTextColor(color.text);
        lcd->print(name.c_str());
        lastValue = name;  
        lastBounds = r;
    }

    Dimensions setTextDimensions(String s)
    {
        int16_t x1 = 0, y1 = 0;
        uint16_t w = 0, h = 0;
        lcd->setFont(font);
        lcd->getTextBounds(s, 0, 0, &x1, &y1, &w, &h);
        textSize = Dimensions(w, h);
        // Serial.printf("getTextDimensions : '%s', w=%d, h=%d\n", s.c_str(), w, h);
        return textSize;
    }
};

#endif