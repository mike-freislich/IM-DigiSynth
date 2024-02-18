#ifndef SIMPLETIMER_H
#define SIMPLETIMER_H
#include <Arduino.h>

class SimpleCallbackTimer
{
public:
  SimpleCallbackTimer() {}
  SimpleCallbackTimer(uint32_t duration, void (*callback)()) : duration(duration), onTimerTickHandler(callback){};
  bool update()
  {
    uint32_t now = millis();
    if (now > last + duration)
    {
      last = now;      
      onTimerTickHandler();
      return true;
    }
    return false;
  }
  void reset() { last = millis(); }
  void setDuration(uint32_t duration) { this->duration = duration; }

private:
  uint32_t duration = 0;
  uint32_t last = 0;
  void (*onTimerTickHandler)();
};

class SimpleTimer
{
public:
  SimpleTimer() {}
  SimpleTimer(uint32_t duration) : _duration(duration){};

  void start() { reset(); }
  bool update()
  {
    uint32_t now = millis();
    if (now > last + _duration)
    {
      last = now;
      return true;
    }
    return false;
  }
  void reset() { last = millis(); }
  void duration(uint32_t duration) { this->_duration = duration; }

private:
  uint32_t _duration = 0;
  uint32_t last = 0;
};

#endif