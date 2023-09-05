#ifndef DSELEMENT_H
#define DSELEMENT_H

#include <vector>
#include <ILI9341_t3n.h>
#include <DSFonts.h>
#include <DSGraphics.h>
#include <DSColor.h>
#include <DSScene.h>
#include <DSBounds.h>

class DSElement;
typedef std::vector<DSElement *> DSElementVector;

class DSElement : public DSBounds
{
public:
    ElementColor color = ElementColor();
    DSBounds *screenBounds;

    DSElement(ILI9341_t3n *lcd, String name, DSBounds *parent = nullptr) : DSBounds()
    {
        screenBounds = new DSBounds(Rect(2, 0, lcd->width() - 2, lcd->height()), nullptr);
        setBounds(0, 0, 5, 5, screenBounds);
        this->lcd = lcd;
        this->name = name;
        this->didChange = true;
        this->visible = false;
    }

    virtual void didSetParent(DSElement *parent) {}
    void setParent(DSElement *parent)
    {
        if (parent != nullptr)
        {
            this->parent = parent;
            this->attachToSpace((DSBounds *)parent);
            didSetParent(parent);
        }
    }

    virtual void render()
    {
        renderChildren();
        didChange = false;
    }

    virtual void hide() { this->visible = false; }
    virtual void show()
    {
        for (auto e : children)
        {
            e->show();
            e->setDidChange();
        }

        this->visible = true;
        setDidChange();
    }

    DSElement *addChild(DSElement *element)
    {
        element->setParent(this);
        children.push_back(element);
        return element;
    }

    String getName() { return name; }

    void setFont(const ILI9341_t3_font_t f)
    {
        this->font = f;
        setDidChange();
    }

    void setName(String s)
    {
        name = s;
        setDidChange();
    }

    void setDidChange()
    {
        didChange = true;
        onDidChange();
    }

protected:
    String name;
    ILI9341_t3_font_t font = FONT_UI_LABEL;
    ILI9341_t3n *lcd;
    DSElementVector children;
    Dimensions textSize = Dimensions();
    bool didChange = true;
    bool visible = false;

    virtual void onDidChange()
    {
        didChange = true;
        // Serial.printf("BaseClass [%s]: onDidChange() called!\n", name.c_str());
    }

    void drawBorder()
    {
        Rect r = dockedBounds();
        lcd->drawRect(r.x, r.y, r.width, r.height, color.border);
    }

    void drawBackground()
    {
        Rect r = dockedBounds();
        lcd->fillRect(r.x, r.y, r.width, r.height, color.background);
    }

private:
    DSElement *parent = nullptr;

    void renderChildren()
    {
        for (auto e : children)
            e->render();
    }
};

#endif