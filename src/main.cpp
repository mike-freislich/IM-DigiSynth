
// #define DEBUG
#include "DebugLog.h"

#include "ArduinoStub.h"
#include "Audio.h"
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include "LXSynth.h"
#include "mem.h"

void onSerialLogTimer();
SimpleTimer serialLogTimer;
SimpleTimer noteTimer;

void setup()
{
#ifdef BUILD_FOR_TEENSY
    Serial.begin(115200);
    delay(2000);
#endif

    LOG("starting synth");
    synth.init();
    Modules.module<LXOscillator>(ItemType::TLXOscillator, ModKeys::OscillatorA)->begin();
    Modules.module<LXOscillator>(ItemType::TLXOscillator, ModKeys::OscillatorB)->begin();
    serialLogTimer.start(10000);
    noteTimer.start(150);    
}

int note = 0;
bool didSet = false;
void loop()
{    
    // if (noteTimer.update())
    // {   
    //     synth.noteOff(0, 0, 0);
    //     synth.noteOn(0, note+36, 127);
    //     note = (note + 1) % 24;        
    // }

    synth.update();
    if (serialLogTimer.update())
        onSerialLogTimer();
    delay(1);
}

void exiting()
{
}

extern float tempmonGetTemp(void);

void onSerialLogTimer()
{
#ifdef BUILD_FOR_TEENSY
    simpleMemInfo();
    float cpu = AudioProcessorUsage();
    float maxCpu = AudioProcessorUsageMax();
    printf(", cpu %2.2f, max %2.2f, temp %2.1f°C\n", cpu, maxCpu, tempmonGetTemp());
#endif
}
