#ifndef DS_PARAMEDIT_H
#define DS_PARAMEDIT_H

#include <ILI9341_t3n.h>
#include <DSParameterBox.h>
#include "DSScene.h"

using namespace std;

class DSParamEdit : public DSScene
{
public:
    DSParamEdit(ILI9341_t3n *lcd, PSComponent *component) : DSScene(lcd)
    {
        setSceneName(F("PARAM Editor"));
        _component = component;
        addParameterBoxes();
    }

protected:
    PSComponent *_component;

    void addParameterBoxes()
    {
        PSParameter *fakeParm = new PSParameter("fakeParm", 50, 0, 100);
        for (int i = 0; i < 8; i++)
        {
            DSParameterBox *pbox = new DSParameterBox(_lcd, fakeParm, this);
            if (i < 5)
                pbox->dock(left, noneV, 2);
            else
                pbox->dock(right, noneV, 4);

            pbox->setBoundsPosition(0, (i < 5) ? (30) + i * (pbox->boundsRect.height + 4) : (30) + (i - 3) * (pbox->boundsRect.height + 4));
            addElement(pbox);
        }
    }
};

#endif