#ifndef DS_PARAMETERBOX_H
#define DS_PARAMETERBOX_H

#include "DSElement.h"

class DSParameterBox : public DSElement
{
public:
    DSParameterBox(ILI9341_t3n *lcd, PSParameter *param, DSBounds *parentBounds = nullptr) : DSElement(lcd, param->name)
    {
        color.background = COLOR_PARAM_BG;
        color.border = COLOR_PARAM_BG;

        setBounds(Rect(0, 0, lcd->width() / 2 - 6, 25));
        if (parentBounds != nullptr)
            this->attachToSpace(parentBounds);

        DSPanel *nameBox = new DSPanel(lcd, "", Dimensions(boundsRect.width * 0.75 + 1, boundsRect.height - 2), this);
        nameBox->color.background = COLOR_PARAM_BG;
        nameBox->color.border = COLOR_PARAM_BG;
        nameBox->dock(left, middle);
        nameBox->show();
        addChild(nameBox);

        txtName = new DSLabel(lcd, (param != nullptr) ? param->name : "-");
        txtName->setFont(FONT_UI_PARAMEDIT);
        txtName->dock(left, middle, 5);
        txtName->show();
        nameBox->addChild(txtName);

        DSPanel *valueBox = new DSPanel(lcd, "", Dimensions(boundsRect.width * 0.25, boundsRect.height - 2), this);
        valueBox->color.background = COLOR_PARAMVALUE_BG;
        valueBox->color.border = COLOR_PARAM_BG;
        valueBox->borderEnabled = true;
        valueBox->dock(right, middle, 1);
        valueBox->show();
        addChild(valueBox);

 
        if (param == nullptr)
            txtValue = new DSLabel(lcd, "-");
        else
            txtValue = new DSLabel(lcd, param->getValue());
        txtValue->setFont(FONT_UI_PARAMEDIT);
        txtValue->dock(centre, middle);
        txtValue->show();
        valueBox->addChild(txtValue);

        // Rect r = txtName->dockedBounds();
        // Serial.printf("BOUNDS %s, x=%d, y=%d, w=%d, h=%d\n", txtName->getName().c_str(), r.x, r.y, r.width, r.height);        
        attachParameter(param);
    }

    void attachParameter(PSParameter *param)
    {
        _param = param;

        if (param != nullptr)
        {
            txtName->setName(param->name);
            txtValue->setName(String((int)param->getValue()));
        }        
    }

    void attachController(InputBase *controller)
    {
        if (_param != nullptr)
            _param->attachController(controller);
    }

    void render() override
    {
        if (visible && didChange)
        {
            drawBorder();
            Rect box = dockedBounds();
            lcd->fillRect(box.x + (box.width - 39), box.y + 1, 38, box.height - 2, CL(0, 0, 100));
        }
        DSElement::render();
    }

protected:
    DSLabel *txtName;
    DSLabel *txtValue;
    PSParameter *_param;
};

#endif
