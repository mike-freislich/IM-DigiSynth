#ifndef SIMPLETIMER_H
#define SIMPLETIMER_H
#include <Arduino.h>

struct SimpleTimer
{
  uint32_t last;
  uint32_t freq;
  void (*onTimerTickHandler)();
  SimpleTimer(uint32_t timerFreq, void (*callback)()) : freq(timerFreq), onTimerTickHandler(callback) {}

  void update()
  {
    uint32_t now = millis();
    if (now - last >= freq)
    {
      onTimerTickHandler();
      last = now;
    }
  }
};

#endif