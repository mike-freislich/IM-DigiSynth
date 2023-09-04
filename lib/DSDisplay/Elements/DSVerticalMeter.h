#ifndef DS_VERTICALMETER_H
#define DS_VERTICALMETER_H

#include <DSElement.h>

class DSVerticalMeter : public DSElement
{
public:
    DSVerticalMeter(ILI9341_t3n *lcd, String name) : DSElement(lcd, name)
    {
        color.border = lcd->color565(80, 80, 255);
        color.background = lcd->color565(0, 0, 0);
    }

    void render() override
    {
        if (visible && didChange)
        {
            drawBackground();
            drawMeter();
            drawBorder();
        }
        DSElement::render();
    }

    void setRange(int min, int max)
    {
        this->min = min;
        this->max = max;
    }

    void setValue(float left, float right)
    {
        if (levelLeft != left || levelRight != right)
        {
            levelLeft = left;
            levelRight = right;
            setDidChange();
        }
    }

protected:
    float min = 0;
    float max = 1;
    float levelLeft, levelRight;
    const uint8_t padding = 2;

    void drawMeter()
    {
        Rect box = dockedBounds();
        float scaledLevels[] = {
            (levelLeft * (box.height - 1)),
            (levelRight * (box.height - 1))};

        int columnWidth = (box.width - padding * 3) / 2;

        int x = box.x + padding;
        for (int columnHeight : scaledLevels)
        {
            lcd->fillRect(
                x, box.y + box.height - columnHeight, columnWidth, columnHeight, ILI9341_GREEN);
            x += columnWidth + padding;
        }
    }
};

#endif