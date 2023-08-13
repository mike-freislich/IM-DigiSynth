#ifndef DS_VERTICALMETER_H
#define DS_VERTICALMETER_H

#include <DSElement.h>

class DSVerticalMeter : public DSElement, public Docker
{
public:
    DSVerticalMeter(
        ILI9341_t3n *lcd, String(label), DSElement *parent = nullptr)
        : DSElement(lcd, label, Rect(0, 0, 26, 23), parent), Docker(DockPositionX::right, DockPositionY::yCentre)
    {
        // setColors(Color(ILI9341_BLACK, ILI9341_WHITE, ILI9341_BLACK));
        color.border = _lcd->color565(80, 80, 255);
        color.fill = _lcd->color565(0, 0, 0);
        _anchor.horizontal = HorizontalPosition::left;
        setDockOptions(DockPositionX::right, DockPositionY::yCentre);
    }

    ~DSVerticalMeter()
    {
    }

    void render()
    {
        if (getShouldRedraw())
        {
            if (!docked)
            {
                dock(this, _parent);
                docked = true;
            }
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
            setShouldRedraw(true);
        }
    }

protected:
    float min = 0;
    float max = 1;
    float levelLeft, levelRight;
    const uint8_t padding = 2;

    void drawMeter()
    {
        float scaledLevels[] = {
            (levelLeft * (box.height - 1)),
            (levelRight * (box.height - 1))};

        int columnWidth = (box.width - padding * 3) / 2;

        int x = box.x + padding;
        for (int columnHeight : scaledLevels)
        {
            _lcd->fillRect(
                x, box.y + box.height - columnHeight, columnWidth, columnHeight, ILI9341_GREEN);
            x += columnWidth + padding;
        }
    }
};

#endif