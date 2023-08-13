#ifndef DSELEMENT_H
#define DSELEMENT_H


#include <vector>
#include <ILI9341_t3n.h>
#include <XPT2046_Touchscreen.h>
#include <DSFonts.h>
#include <DSGraphics.h>
#include "DSScene.h"

using namespace std;

class DSScene;

class DSElement;
typedef vector<DSElement *> DSElementVector;

enum HorizontalPosition
{
    left,
    centre,
    right
};

enum VerticalPosition
{
    top,
    middle,
    bottom
};

struct AnchorPosition
{
    HorizontalPosition horizontal = HorizontalPosition::centre;
    VerticalPosition vertical = VerticalPosition::middle;

    AnchorPosition() : horizontal(HorizontalPosition::centre), vertical(VerticalPosition::middle) {}
    AnchorPosition(HorizontalPosition h, VerticalPosition v) : horizontal(h), vertical(v) {}
};

class DSElement
{
public:
    struct Color
    {
        uint16_t border, fill, text, textHighlighted, control, controlValue;
        Color(){};
        Color(uint16_t border, uint16_t fill, uint16_t text) : border(border), fill(fill), text(text) {}
    };

    DSElement(ILI9341_t3n *lcd, String label, Rect boundingBox, DSElement *parent = nullptr)
        : _lcd(lcd), label(label), box(boundingBox), _parent(parent)
    {
        setColors();
        setFont(FONT_UI_LABEL);
        _textBox = getTextBox(label);
    }
    ~DSElement() {}

    void setColors(Color elementColor = Color(ILI9341_DARKGREY, ILI9341_WHITE, ILI9341_BLACK))
    {
        color = elementColor;
        setShouldRedraw(true);
    }

    void setAnchors(AnchorPosition a)
    {
        this->_anchor = a;
    }

    void setParent(DSElement *parent)
    {
        _parent = parent;
    }

    void setScene(DSScene *scene) { _scene = scene; }
    DSScene *getScene() { return _scene; }

    virtual void render()
    {
        for (auto e : childElements)
            e->render();
        setShouldRedraw(false);
    }

    void addElement(DSElement *element, bool useParentOrigin = true)
    {
        element->setParent(this);
        if (useParentOrigin)
        {
            element->box.x += this->box.x;
            element->box.y += this->box.y;
        }

        childElements.push_back(element);
        // element->setShouldRedraw(true);
    }

    void setShouldRedraw(bool state)
    {
        for (auto e : childElements)
            e->setShouldRedraw(state);
        _needsRedraw = state;
    }

    virtual void clear()
    {
        _lcd->fillRect(box.x, box.y, box.width, box.height, 0);
        // for(auto e : childElements) {
        //     e->clear();
        //     e->setShouldRedraw(true);
        // }
    }

    bool getShouldRedraw() { return _needsRedraw; }

    bool isPointInElement(TS_Point point)
    {
        return ((point.x >= box.x) && (point.x <= box.x + box.width) && (point.y >= box.y) && (point.y <= box.y + box.height));
    }

    void setLabel(String s)
    {
        label = s;
        _textBox = getTextBox(s);
    }
    void setFont(ILI9341_t3_font_t font) { this->font = font; }

    Rect getBoundingBox() { return box; }
    void setBoundingBox(Rect bb) { box = bb; }

    void MoveTo(int x, int y)
    {
        box.x = x;
        box.y = y;
    }

    void logToScreen(String msg)
    {
        Point c(320 / 2, 240 / 2);
        _lcd->setCursor(c.x, c.y);
        drawText(c.x, c.y, msg, FONT_UI_LABEL);
        // printCenteredText(c.x, c.y, msg, FONT_UI_LABEL, ILI9341_WHITE);
    }

    Rect getTextBox(String s)
    {
        Rect bounds(0, 0, 0, 0);
        int16_t x1, y1;
        uint16_t w, h;
        _lcd->getTextBounds(s, 0, 0, &x1, &y1, &w, &h);
        bounds.width = w;
        bounds.height = h;
        return bounds;
    }

    String getLabel() { return label; }

    void setUpdateDelay(uint16_t ms)
    {
        _updateDelay = ms;
    }

protected:
    uint16_t _updateDelay = 0;
    uint32_t _lastUpdate = 0;
    ILI9341_t3n *_lcd;
    String label;
    Rect box;
    DSElement *_parent;
    Color color;
    DSElementVector childElements;
    ILI9341_t3_font_t font;
    Rect _textBox;
    AnchorPosition _anchor;
    DSScene *_scene;

    bool updateTimerReady()
    {
        if (_updateDelay == 0)
            return true;

        uint32_t now = millis();
        if (now - _lastUpdate > _updateDelay)
        {
            _lastUpdate = now;
            return true;
        }
        return false;
    }

    void drawBackground()
    {
        _lcd->fillRect(box.x, box.y, box.width, box.height, color.fill);
    }

    void drawBorder()
    {
        _lcd->drawRect(box.x, box.y, box.width, box.height, color.border);
    }

    Point getTextAnchorPosition(int x, int y, String s) { return getTextAnchorPosition(x, y, s, _anchor); }

    Point getTextAnchorPosition(int x, int y, String s, AnchorPosition a)
    {
        int16_t x1, y1;
        uint16_t w, h;
        _lcd->getTextBounds(s, x, y, &x1, &y1, &w, &h);

        Point p(x, y);

        if (a.vertical == VerticalPosition::top)
            p.y = y;
        else if (a.vertical == VerticalPosition::middle)
            p.y = y - h / 2;
        else if (a.vertical == VerticalPosition::bottom)
            p.y = y - h;

        if (a.horizontal == HorizontalPosition::left)
            p.x = x;
        else if (a.horizontal == HorizontalPosition::centre)
            p.x = x - w / 2;
        else if (a.horizontal == HorizontalPosition::right)
            p.x = x - w;
        return p;
    }

    // Draws a label at point using element alignment settings
    void drawLabel(Point p) { drawLabel(p.x, p.y); }

    // Draws a label at point x and y using element alignment settings
    void drawLabel(int x, int y) { drawText(x, y, label, font, color.text, _anchor); }

    void drawText(int x, int y, String s,
                  ILI9341_t3_font_t f,
                  uint16_t textColor = ILI9341_PINK,
                  AnchorPosition a = AnchorPosition(centre, middle))
    {
        _lcd->setFont(f);
        Point p = getTextAnchorPosition(x, y, s, a);
        _lcd->setCursor(p.x, p.y);
        _lcd->setTextColor(textColor);
        _lcd->print(s);
        //_lcd->fillCircle(x, y, 3, ILI9341_PINK); // draw anchor for debugging
    }

    void printCenteredText(int x, int y, String s, ILI9341_t3_font_t font, uint16_t color)
    {
        _lcd->setFont(font);
        _lcd->setTextColor(color);
        Point cursorPos = getTextAnchorPosition(x, y, s);
        _lcd->setCursor(cursorPos.x, cursorPos.y);
        _lcd->print(s);
    }

private:
    bool _needsRedraw = true;
};

class Docker
{
public:
    bool docked = false;
    enum DockPositionX
    {
        left,
        xCentre,
        right,
        xNone
    } x;
    enum DockPositionY
    {
        top,
        yCentre,
        bottom,
        yNone
    } y;
    Docker()
    {
        x = xNone;
        y = yNone;
    }
    Docker(DockPositionX x, DockPositionY y) : x(x), y(y) {}

    void setDockOptions(DockPositionX x, DockPositionY y)
    {
        this->x = x;
        this->y = y;
        docked = false;
    }

    Rect getDockPosition(Rect child, Rect parent)
    {
        if (this->x == left)
        {
            child.x = parent.x;
        }
        else if (x == xCentre)
        {
            child.x = parent.x + (parent.width - child.width) / 2;
        }
        else if (x == right)
        {
            child.x = parent.width - child.width;
        }

        if (this->y == top)
        {
            child.y = parent.y;
        }
        else if (y == yCentre)
        {
            child.y = parent.y + (parent.height - child.height) / 2;
        }
        else if (y == bottom)
        {
            child.y = parent.height - child.height;
        }

        return child;
    }

    void dock(DSElement *dockable, DSElement *parent)
    {
        if (!docked && parent != nullptr)
        {
            Rect newBox = this->getDockPosition(dockable->getBoundingBox(), parent->getBoundingBox());
            dockable->setBoundingBox(newBox);
            docked = true;
        }
    }
};

#endif