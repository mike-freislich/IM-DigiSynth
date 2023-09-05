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
        PSParameter *fakeParm = new PSParameter("-", 0, 0, 0);

        for (int i = 0; i < 8; i++)
        {
            PSParameter *p = this->getParameter(i);
            if (p == nullptr)
                p = fakeParm;

            DSParameterBox *pbox = new DSParameterBox(_lcd, p, this);
            if (i < 5)
                pbox->dock(left, noneV, 2);
            else
                pbox->dock(right, noneV, 4);

            pbox->setBoundsPosition(0, (i < 5) ? (30) + i * (pbox->boundsRect.height + 4) : (30) + (i - 3) * (pbox->boundsRect.height + 4));
            addElement(pbox);
        }
    }

    PSParameter *getParameter(int i)
    {
        if (i < _component->params.size())        
            return _component->params[i];
                
        return nullptr;
    }
};

#endif