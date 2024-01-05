// #include "TeensyDebug.h"
// #pragma GCC optimize ("O0")
#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include "PSMidi.h"
#include <DSDisplay.h>
#include <SimpleTimer.h>
#include "DSSequencer.h"
#include "TeensyThreads.h"

#include "PolySynth.h"

#include "FastTrig.h"
#include "mem.h"
#include "controls.h"
#include "tests.h"
#include "lights.h"

#define ARDUINO_SERIAL_DELAY 5000
#define SCENE_DELAY 5000

void refreshDisplay();
void onSerialLogTimer();
void monitorPeakOutput();
void playStep(SeqStep *_steps[SEQ_TRACKS]);

Sequencer seq = Sequencer(playStep);
DSDisplay display = DSDisplay(&polySynth, &controls);

SimpleCallbackTimer displayRefreshTimer(1000 / SCREEN_REFRESH_RATE, refreshDisplay);
SimpleCallbackTimer audioOutPollTimer(1000 / SCREEN_REFRESH_RATE, monitorPeakOutput);

#ifdef USB_MIDI_AUDIO_SERIAL
SimpleCallbackTimer serialLogTimer(10000, onSerialLogTimer);

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

void midiLoop()
{
  while (1)
  {
    psMidi.update();
  }
}

FLASHMEM void setup()
{
#ifdef USB_MIDI_AUDIO_SERIAL
  Serial.begin(115200);
#endif
  display.begin();
#ifdef USB_MIDI_AUDIO_SERIAL
  delayForSerial();
#endif
#ifdef RUNTESTS
  runTests();
#endif

  digitalIO.setup();
  digitalIO.begin();

  controls.setup();

  polySynth.init();
  display.setupScenes();

  polySynth.loadPatch(0);
  seq.setTempo(SEQ_TEMPO, 8);
  // seq.play();
  polySynth.voice1.setupControllers();
  display.nextScene();
  threads.addThread(synthLoop);

  psMidi.begin(&polySynth);
  // threads.addThread(midiLoop);
  //  Serial.println(polySynth.voice1.toString());


}

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
  digitalIO.update();
  controls.update();
  lights.update();
  serialLogTimer.update();
  psMidi.update();
  polySynth.updateVoices(psMidi.pitchBend);

  if (controls.buttons[0].didLongPress())
  {
    polySynth.savePatch(0);
  }

  delay(1);
}

extern float tempmonGetTemp(void);
void onSerialLogTimer()
{
  simpleMemInfo();

  float cpu = AudioProcessorUsage();
  float maxCpu = AudioProcessorUsageMax();

  printf(", cpu %2.2f, max %2.2f, temp %2.1f°C", cpu, maxCpu, tempmonGetTemp());

  for (uint8_t i = 0; i < sizeof(buttonPinsActive); i++)
  {
    Button *b = &controls.buttons[i];
    printf(", button[%02d] %d", b->getPin(), b->getValue());
  }

  Serial.print("\n");
}

void playStep(SeqStep *steps[SEQ_TRACKS])
{
  for (int i = 0; i < SEQ_TRACKS; i++)
  {
    if (i == 0)
      polySynth.playNote(steps[i]->midiNote, steps[i]->velocity);
  }
}