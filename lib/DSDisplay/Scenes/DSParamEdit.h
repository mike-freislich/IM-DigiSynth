#ifndef DS_PARAMEDIT_H
#define DS_PARAMEDIT_H

#include <ILI9341_t3n.h>
#include "DSScene.h"

class DSParamEdit : public DSScene
{
public:
    DSParamEdit(ILI9341_t3n *lcd) : DSScene(lcd)
    {
        setSceneName(F("PARAM Editor"));
        
    }

protected:
};

#endif