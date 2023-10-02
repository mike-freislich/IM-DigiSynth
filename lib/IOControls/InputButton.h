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
    Button(int pin, IOType ioType = IO_EXPANSION, uint16_t debounceMillis = 30) : InputBase(pin, ioType, debounceMillis)
    {
        if (ioType == IO_TEENSY)
            pinMode(pin, INPUT_PULLUP);

        controllerType = ControllerType::CT_BTN;
        _value = 1;
    }

    bool pressed() { return getValue(); }
    float getValue() override
    {
        return ((ioType == IO_EXPANSION && _value == HIGH) || (ioType == IO_TEENSY && _value == LOW));
    }

    bool didLongPress(uint16_t duration = 1000)
    {
        
        if (_didRelease)
        {
            _didRelease = false;            
            bool result = (releaseDuration > duration);
            if (result)
                Serial.printf("Button [%d] didLongPress %d\n", _pin, releaseDuration);
            return result;
        }
        return false;
    }

    bool setValue(int value)
    {
        if (value != _value && debounced())
        {
            Serial.printf("button [%d] value changed to %d\n", _pin, value);
            _value = value;
            if (pressed())
            {
                Serial.printf("Button [%d] pressed\n", _pin);
                pressedTime = millis();
                releaseDuration = 0;
                _didRelease = false;
            }
            else
            {
                Serial.printf("Button [%d] released\n", _pin);
                releaseDuration = millis() - pressedTime;
                _didRelease = true;
            }

            return true;
        }
        return false;
    }

    void update() override
    {
        int v = (ioType == IO_EXPANSION) ? (int)digitalIO.pinState(_pin) : digitalRead(_pin);
        setValue(v);
    }

protected:
    uint32_t pressedTime = 0;
    uint32_t releaseDuration = 0;
    bool _didRelease = false;
};

#endif