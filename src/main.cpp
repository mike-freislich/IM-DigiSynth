
//#define DEBUG
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
    synth.noteOn(0, 64, 127);
    serialLogTimer.start(10000);
}

void loop()
{
    synth.update();
    if (serialLogTimer.update())
        onSerialLogTimer();
    //delay(1);
}

void exiting()
{
}

extern float tempmonGetTemp(void);

void onSerialLogTimer()
{
  simpleMemInfo();
  float cpu = AudioProcessorUsage();
  float maxCpu = AudioProcessorUsageMax();
  printf(", cpu %2.2f, max %2.2f, temp %2.1f°C\n", cpu, maxCpu, tempmonGetTemp());  
}
