#define DEBUG
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
SimpleTimer serialLogTimer(10000);
SimpleTimer noteTimer(150);
SimpleTimer loopDelayTimer(10);

FLASHMEM void setup()
{
#ifdef TEENSY_OPT_SMALLEST_CODE
    asm(".global _printf_float");
#endif

#ifdef BUILD_FOR_TEENSY
    Serial.begin(115200);
    delay(2000);
#endif

    LOG("starting synth");
    synth.init();
    Modules.module<LXOscillator>(ItemType::TLXOscillator, ModKeys::OscillatorA)->begin();
    Modules.module<LXOscillator>(ItemType::TLXOscillator, ModKeys::OscillatorB)->begin();
    serialLogTimer.start();
    noteTimer.start();
    loopDelayTimer.start();
}

void loop()
{
    synth.update();
    if (serialLogTimer.update())
         onSerialLogTimer();
    
    while (!loopDelayTimer.update())
        yield();
}

extern float tempmonGetTemp(void);

FLASHMEM void onSerialLogTimer()
{
#ifdef BUILD_FOR_TEENSY
    simpleMemInfo();
    float cpu = AudioProcessorUsage();
    float maxCpu = AudioProcessorUsageMax();
    printf(", cpu %2.2f, max %2.2f, temp %2.1f°C\n", cpu, maxCpu, tempmonGetTemp());
#endif
}
