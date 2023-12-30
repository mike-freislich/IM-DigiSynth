#ifndef DS_PARAMETER_STRIP_H
#define DS_PARAMETER_STRIP_H

#include "DSElements.h"
#include "DSParameterDial.h"



class DSParameterStrip : public DSElement
{
public:
    DSParameterStrip(ILI9341_t3n *lcd, String name) : DSElement(lcd, name)
    {
        this->setBounds(0, 0, screenBounds->boundsRect.width, 50);
    }

    void addParameterWithController(PSParameter *parameter, InputBase *controller = nullptr)
    {
        parameters.push_back(parameter);
        if (controller != nullptr)
        {
            int i = parameters.size() % controls.pots.size();
            parameter->attachController(&controls.pots[i]);
        }
        else
            parameter->attachController(controller);
        
        if (visible)
            parameter->activate();
    }

    void initLayout()
    {
        uint8_t i = 0;
        const int usableWidth = 280;
        const int xmargin = (320 - usableWidth) / 2;
        int n = parameters.size(); // Number of objects
        const int w = 50;          // Width of each object
        const int x = usableWidth; // Horizontal distance
        int spacing = (x - w) / (n - 1);

        while (parameters[i] != nullptr && i < n)
        {
            int xpos = xmargin + i * spacing;
            DSElement *e = addChild(new DSParameterDial(lcd, parameters[i], panel));
            e->attachToSpace(this);
            e->setBoundsPosition(xpos, 0);
            e->dock(noneH, bottom);
            i++;
        }
    }

    void render() override
    {
        updateControls();
        DSElement::render();
    }

    void updateControls()
    {
        if (visible)
        {
            // TODO update the parameter's target object with the value
        }
    }

    void show() override
    {
        DSElement::show();
        for (auto p : parameters)
            p->activate();
    }
    void hide() override
    {
        DSElement::hide();
        for (auto p : parameters)
            p->deactivate();
    }

private:
    DSPanel *panel;
    DSParameterDial **dials;
    PSParameterVector parameters;
};

#endif
