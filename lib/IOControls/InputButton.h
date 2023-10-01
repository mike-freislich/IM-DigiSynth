#ifndef INPUT_BUTTON_H
#define INPUT_BUTTON_H

#include <InputBase.h>

class Button : public InputBase
{
public:
    Button() : InputBase()
    {
        controllerType = ControllerType::CT_BTN;
        _value = 1;
    }
    Button(int pin, uint16_t debounceMillis = 10) : InputBase(pin, debounceMillis)
    {
        pinMode(pin, INPUT_PULLUP);
        controllerType = ControllerType::CT_BTN;
        _value = 1;
    }

    bool pressed() { return getValue(); }
    float getValue() override { return (_value == LOW); }

    FLASHMEM bool didLongPress(uint16_t duration = 1000)
    {
        // Serial.printf("didLongPress %d\n", duration);
        if (_didRelease)
        {
            _didRelease = false;
            Serial.println("didRelease");
            return (releaseDuration >= duration);
        }
        return false;
    }

    FLASHMEM bool setValue(int value)
    {
        if (value != _value && debounced())
        {
            _value = value;
            if (pressed())
            {
                pressedTime = millis();
                releaseDuration = 0;
                _didRelease = false;
            }
            else
            {
                releaseDuration = millis() - pressedTime;
                _didRelease = true;
            }

            return true;
        }
        return false;
    }

    void update() override { setValue(digitalRead(_pin)); }

protected:
    uint32_t pressedTime = 0;
    uint32_t releaseDuration = 0;
    bool _didRelease = false;
};


#endif