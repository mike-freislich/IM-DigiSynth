#ifndef DS_PARAMETERBOX_H
#define DS_PARAMETERBOX_H

#include <vector>
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

        DSPanel *nameBox = new DSPanel(lcd, "", Dimensions(boundsRect.width * 0.70 + 1, boundsRect.height - 2), this);
        nameBox->color.background = COLOR_PARAM_BG;
        nameBox->color.backgroundHighlight = CL(50, 50, 70);
        nameBox->color.border = COLOR_PARAM_BG;
        nameBox->color.borderHighlight = ILI9341_YELLOW;
        nameBox->borderEnabled = false;
        nameBox->dock(left, middle);
        nameBox->show();
        addChild(nameBox);

        txtName = new DSLabel(lcd, (param != nullptr) ? param->name : "-");
        txtName->setFont(FONT_UI_PARAMEDIT);
        txtName->color.textHighLight = ILI9341_YELLOW;
        txtName->dock(left, middle, 5);
        txtName->show();
        nameBox->addChild(txtName);

        DSPanel *valueBox = new DSPanel(lcd, "", Dimensions(boundsRect.width * 0.30, boundsRect.height - 2), this);
        valueBox->color.background = COLOR_PARAMVALUE_BG;
        nameBox->color.backgroundHighlight = CL(30, 30, 50);
        valueBox->color.border = COLOR_PARAM_BG;
        // valueBox->color.borderHighlight = COLOR_PARAM_BG_SELECTED;
        valueBox->borderEnabled = false;
        valueBox->dock(right, middle, 1);
        valueBox->show();
        addChild(valueBox);

        if (param == nullptr)
            txtValue = new DSLabel(lcd, "-");
        else
            txtValue = new DSLabel(lcd, param->displayValue());
        txtValue->setFont(FONT_UI_PARAMEDIT);
        txtValue->color.textHighLight = ILI9341_YELLOW;
        txtValue->dock(centre, middle);
        txtValue->show();
        valueBox->addChild(txtValue);

        if (param != nullptr)
            attachParameter(param);
    }

    void attachParameter(PSParameter *param)
    {
        _param = param;
        if (param != nullptr)
        {
            txtName->setName(param->name);
            txtValue->setName(param->displayValue());                          
        }
    }

    void attachController(InputBase *controller)
    {
        if (_param != nullptr)
            _param->attachController(controller);
    }

    void render() override
    {
        if (visible)
        {
            bool render = didChange;           
            DSElement::render();

            if (render)
                drawBorder();
        }
    }

    PSParameter *parameter() { return _param; }

    void updateValue()
    {
        if (_param != nullptr)
        {            
            txtValue->setName(_param->displayValue());
            setDidChange();
        }
    }

protected:
    DSLabel *txtName;
    DSLabel *txtValue;
    PSParameter *_param;
};

typedef std::vector<DSParameterBox *> DSParameterBoxVector;

#endif
