// #include "TeensyDebug.h"
// #pragma GCC optimize ("O0")
#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>

#include <DSDisplay.h>
#include <SimpleTimer.h>
#include "DSSequencer.h"
#include "TeensyThreads.h"
#include "PolySynth.h"
#include "FastTrig.h"
#include "mem.h"
#include "controls.h"
#include "MIDIUSB.h"
#include "tests.h"

#define ARDUINO_SERIAL_DELAY 5000
#define SCENE_DELAY 5000

//void nextScene();
void refreshDisplay();
void onDisplayUpdateTouch();
void onSerialLogTimer();
void monitorPeakOutput();
void playStep(SeqStep *_steps[SEQ_TRACKS]);


PolySynth polySynth = PolySynth();
Sequencer seq = Sequencer(playStep);
DSDisplay display = DSDisplay(onDisplayUpdateTouch, &polySynth, &controls);

SimpleTimer displayRefreshTimer(1000 / SCREEN_REFRESH_RATE, refreshDisplay);
SimpleTimer audioOutPollTimer(1000 / SCREEN_REFRESH_RATE, monitorPeakOutput);

#ifdef USB_MIDI_AUDIO_SERIAL
SimpleTimer serialLogTimer(10000, onSerialLogTimer);

void delayForSerial()
{
  display.lcd().println(F("waiting for arduino serial monitor"));
  uint32_t lastTime = millis();
  while (!Serial && ((millis() - lastTime) < ARDUINO_SERIAL_DELAY))
    delay(1);
}
#endif

void synthLoop()
{
  while (1)
  {
    seq.update();
  }
}

FLASHMEM void setup()
{
#ifdef USB_MIDI_AUDIO_SERIAL
  Serial.begin(115200);
#endif
  display.begin();
  display.setupScenes();
#ifdef USB_MIDI_AUDIO_SERIAL
  delayForSerial();
#endif
#ifdef RUNTESTS
  runTests();
#endif
  polySynth.init();
  polySynth.loadPatch(0);
  //Serial.println(polySynth.voice1.toString());
  seq.setTempo(SEQ_TEMPO, 8);
  seq.play();
  display.nextScene();
  threads.addThread(synthLoop);
}

// FLASHMEM void nextScene()
// {
//   display.nextScene();
// }

void refreshDisplay() { display.render(); }

void monitorPeakOutput()
{
  StereoLevels level = polySynth.getPeakLevel();
  display.getCurrentScene()->getAudioMeter()->setValue(level.left, level.right);
}

FLASHMEM void loop()
{
  displayRefreshTimer.update();
  audioOutPollTimer.update();
  controls.update();

  if (controls.buttons[0].didLongPress())
    polySynth.savePatch(0);

  serialLogTimer.update();  

  delay(1);
}

extern float tempmonGetTemp(void);
void onSerialLogTimer()
{
  simpleMemInfo();

  float cpu = AudioProcessorUsage();
  float maxCpu = AudioProcessorUsageMax();

  printf(", cpu %2.2f, max %2.2f, temp %2.1f°C", cpu, maxCpu, tempmonGetTemp());

  for (int i = 0; i < NUMPOTS; i++)
  {
    Potentiometer *p = &controls.pots[i];
    printf(", pot[%2d] %3d", p->getPin(), p->getValue());
  }
  for (int i = 0; i < NUMBUTTONS; i++)
  {
    Button *b = &controls.buttons[i];
    printf(", button[%2d] %d", b->getPin(), b->getValue());
  }

  Serial.print("\n");
}

void onDisplayUpdateTouch()
{
  display.updateTouch();
}

FLASHMEM void playStep(SeqStep *steps[SEQ_TRACKS])
{
  for (int i = 0; i < SEQ_TRACKS; i++)
  {
    if (i == 0)
      polySynth.playNote(steps[i]->midiNote, steps[i]->velocity);
  }
}