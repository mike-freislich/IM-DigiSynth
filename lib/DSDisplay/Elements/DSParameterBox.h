#ifndef DS_PARAMETERBOX_H
#define DS_PARAMETERBOX_H

#include "DSElement.h"

class DSParameterBox : public DSElement
{
public:
    DSParameterBox(ILI9341_t3n *lcd, String label, DSElement *parent)
        : DSElement(lcd, label, Rect(), parent)
    {
        setBoundingBox(Rect(0, 0, 100, 25));
        
        DSElement::Color colorName(ILI9341_CYAN, ILI9341_BLUE, ILI9341_WHITE);
        _txtName = new DSLabel(lcd, "Param Name", colorName);
        
        DSElement::Color colorValue(ILI9341_CYAN, ILI9341_BLUE, ILI9341_WHITE);
        _txtValue = new DSLabel(lcd, "Param Name", colorValue);
        addElement(_txtName);
        addElement(_txtValue);                     
    }

    void attachParameter(PSParameter *param) { _param = param; } 


protected:
    DSLabel *_txtName;
    DSLabel *_txtValue;
    PSParameter *_param;
};

#endif
