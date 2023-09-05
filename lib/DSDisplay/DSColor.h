#ifndef DS_COLOR_H
#define DS_COLOR_H

#define COLOR_BORDER CL(50, 50, 200)
#define COLOR_HEADING_FILL CL(50, 50, 200)
#define COLOR_BACKGROUND 0
#define COLOR_TOP_PANEL CL(80, 150, 255)
#define COLOR_TEXT CL(255,255,255)
#define COLOR_TEXT_HIGHLIGHT CL(255, 255, 64)
#define COLOR_CONTROL CL(100,100,100)
#define COLOR_CONTROL_VALUE CL(150,150,150)

#define COLOR_PARAM_BG CL(50,100,200)
#define COLOR_PARAMVALUE_BG CL(40,70,150)
#define COLOR_PARAM_BG_SELECTED CL(100,150,200)

struct ElementColor
{
    uint16_t
        background = COLOR_BACKGROUND,
        border = COLOR_BORDER,
        text = COLOR_TEXT,
        textHighLight = COLOR_TEXT_HIGHLIGHT,
        control = COLOR_CONTROL,
        controlValue = COLOR_CONTROL_VALUE;
};


#endif