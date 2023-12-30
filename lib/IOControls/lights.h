#ifndef LIGHTS_H
#define LIGHTS_H

#include <Arduino.h>
#include <DigitalIO.h>
#include <SimpleTimer.h>

const uint8_t PROGMEM ledPins[] = {16};

class Lights
{
public:
    Lights() {}

    void update()
    {
        if (flashTimer.update())
        {
            for (uint8_t i = 0; i < sizeof(ledPins); i++)
            {
                bool v = random(2);
                digitalIO.digitalWriteIO(ledPins[i], v);
            }
        }
    }

private:
    SimpleTimer flashTimer = SimpleTimer(100);    
} lights;

#endif