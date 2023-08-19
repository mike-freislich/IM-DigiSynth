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

#define ARDUINO_SERIAL_DELAY 10000
#define SCENE_DELAY 5000

void nextScene();
void refreshDisplay();
void onDisplayUpdateTouch();
void changeEnvelope();
void onSerialLogTimer();
void monitorPeakOutput();
void playStep(SeqStep *_steps[SEQ_TRACKS]);

PolySynth polySynth;
Sequencer seq(playStep);
DSDisplay display(onDisplayUpdateTouch, &polySynth);

SimpleTimer sceneRotationTimer(SCENE_DELAY, nextScene);
SimpleTimer displayRefreshTimer(1000 / SCREEN_REFRESH_RATE, refreshDisplay);
SimpleTimer audioOutPollTimer(1000 / SCREEN_REFRESH_RATE, monitorPeakOutput);
// SimpleTimer changeEnvelopeTimer(1000 / SCREEN_REFRESH_RATE, changeEnvelope);

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
  display.clearScreen(ILI9341_BLACK);
#ifdef USB_MIDI_AUDIO_SERIAL
  delayForSerial();
#endif
  seq.setTempo(SEQ_TEMPO, 8);
  seq.play();
  display.nextScene();
  threads.addThread(synthLoop);

  #ifdef RUNTESTS
  runTests();
  #endif
}

void nextScene()
{
  display.nextScene();
}

void refreshDisplay() { display.render(); }

void monitorPeakOutput()
{
  StereoLevels level = polySynth.getPeakLevel();
  display.getCurrentScene()->getAudioMeter()->setValue(level.left, level.right);
}

FLASHMEM void loop()
{
  // sceneRotationTimer.update();
  // changeEnvelopeTimer.update();
  displayRefreshTimer.update();
  audioOutPollTimer.update();
  controls.update();
  serialLogTimer.update();
}

void onSerialLogTimer()
{
  /*
  simpleMemInfo();

  float cpu = AudioProcessorUsage();
  float maxCpu = AudioProcessorUsageMax();
  printf("cpu %2f\tmax %2f\t", cpu, maxCpu);

  for (int i = 0; i < NUMPOTS; i++)
  {
    Potentiometer *p = &controls.pots[i];
    printf("pot %d = %d\t", p->getPin(), p->getValue());
  }
  for (int i = 0; i < NUMBUTTONS; i++)
  {
    Button *b = &controls.buttons[i];
    printf("button %d = %d\t", b->getPin(), b->getValue());
  }

  Serial.print("\n");
  */
}

void onDisplayUpdateTouch() { display.update(); }

FLASHMEM void playStep(SeqStep *steps[SEQ_TRACKS])
{
  for (int i = 0; i < SEQ_TRACKS; i++)
  {
    if (i == 0)
      polySynth.playNote(steps[i]->midiNote, steps[i]->velocity);
  }
}

DSSceneEnvelope *es = display.env;
FLASHMEM void changeEnvelope()
{
  // polySynth.vcfL->frequency(vcf_EGL.read()*2500 + polySynth.filterLFO->valuePostGain());
  // polySynth.vcfR->frequency(vcf_EGR.read()*2500 + polySynth.filterLFO->valuePostGain());
  // polySynth.amp->attack(es->attack->getValue());
  // polySynth.amp->attack(es->attack->sinInc(1));
  //  polySynth.amp->hold(es->hold->sinInc(2));
  // polySynth.amp->decay(es->decay->sinInc(1));
  // polySynth.amp->sustain(es->sustain->sinInc(3));
  // polySynth.amp->release(es->release->sinInc(6));

  es->graph->setShouldRedraw(true);
}
