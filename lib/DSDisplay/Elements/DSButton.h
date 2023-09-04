#ifndef DSBUTTON_H
#define DSBUTTON_H

#include <DSElement.h>
#include <DSColor.h>
#include <ILI9341_t3n.h>

class DSButton : public DSElement
{
public:
    DSButton(ILI9341_t3n *lcd, String name) : DSElement(lcd, name)             
    {        
        this->setBounds(Rect(0,0,50,25));                
    }

    ~DSButton()
    {
    }

    void render() override
    {        
        DSElement::render();
    }
};

#endif