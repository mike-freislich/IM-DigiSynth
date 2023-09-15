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
        String name = F("Edit Params (") + component->name + ")";
        setSceneName(name);
        _component = component;

        addParameterBoxes();
        parameterIndex = -1;
        //selectParameter(true);
        scrollPot = &controls.pots[0]; // left pot
        valuePot = &controls.pots[3];  // right pot
    }

    void render() override
    {
        if (visible)
        {
            selectParameter();
            updateParameterValue();
        }
        DSScene::render();
    }

protected:
    PSComponent *_component;
    Potentiometer *scrollPot;
    Potentiometer *valuePot;
    DSParameterBoxVector parameterBoxes;
    DSParameterBox *selected = nullptr;
    uint8_t parameterIndex = 0;

    // get the selectedParameterIndex from the scroll potentiometer
    // "force" selection even if the index hasn't changed (off by default)
    void selectParameter(bool force = false)
    {
        int newParameterIndex = clamp((int)scrollPot->getValue(), 10, 980); // get potentiometer value
        newParameterIndex = map(newParameterIndex, 10, 980, 0, 7);          // convert to an index 0..7

        if (newParameterIndex != parameterIndex || force) // only change if index changed OR forced
        {
            if (selected != nullptr)
            {                
                selected->parameter()->deactivate();
                selected->parameter()->detachController();
                selected->unselect();
            }

            parameterIndex = newParameterIndex;
            selected = parameterBoxes[parameterIndex];
            if (selected != nullptr)
            {                           
                if (selected->parameter() != nullptr)
                {
                    PSParameter *p = selected->parameter();
                    p->attachController(valuePot);                    
                    p->activate();                    
                    
                    //pname = p->name;
                    //pval = p->getValue();
                    Serial.print("here: ");
                    Serial.print(p->name);
                    Serial.print(" ");
                    Serial.print(p->getValue());
                    Serial.println();
                    
                }
                selected->select();              
                //Serial.printf("Selected : box: %s, parm: %s, value:%f \n", "test", pname.c_str(), pval);
            }
        }
    }

    void updateParameterValue()
    {
        if (!validParameter())
        {
            Serial.println(F("invalid parameter"));
            return;
        }

        PSParameter *p = selected->parameter();
        if (p->updateFromControl(true))
        {
            Serial.printf("----------[%s] %2.2f\n", p->name.c_str(), p->getValue());
            selected->updateValue();
            _component->updateFromControl();
            p->didChange(true);
        }
        // Serial.printf("[%s] %2.2f\n", p->name.c_str(), p->getValue());
    }

    bool validParameter() { return (selected && selected->parameter()); }

    void addParameterBoxes()
    {
        PSParameter *emptyParm = new PSParameter("-");
        for (int i = 0; i < 8; i++)
        {
            PSParameter *p = this->getParameter(i);
            if (p == nullptr)
                p = emptyParm;

            DSParameterBox *pbox = new DSParameterBox(_lcd, p, this);
            if (i < 5)
                pbox->dock(left, noneV, 2);
            else
                pbox->dock(right, noneV, 4);
            pbox->setBoundsPosition(0, (i < 5) ? (30) + i * (pbox->boundsRect.height + 4) : (30) + (i - 3) * (pbox->boundsRect.height + 4));
            parameterBoxes.push_back(pbox);
            addElement(pbox);
        }
    }

    PSParameter *getParameter(int i)
    {
        if (i < (int)_component->params.size())
            return _component->params[i];
        return nullptr;
    }
};

#endif