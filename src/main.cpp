
#define DEBUG
#include "DebugLog.h"

#include "ArduinoStub.h"
#include "Audio.h"
#include "LXSynth.h"



void setup()
{
#ifdef BUILD_FOR_TEENSY
    Serial.begin(115200);
    delay(2000);
#endif

    LOG("starting synth");
    synth.init();
}

void loop()
{
    synth.update();
    delay(100);
    LOG("loop");
}   

void exiting()
{
}

