#pragma once
#include <stdint.h>

#include "AppHelper/Utils/Timer.h"
#include "GPIOPortPin.h"

class Timer;

class Led {
 public:
  void setPin(GPIOPortPin* portPin);
  void update();

  void ledOn();
  void ledOff();
  void ledToggle();
  void setBlink(uint16_t timeOnMs, uint16_t timeOffMs, uint32_t durationMs = UINT32_MAX);
  void stopBlink();

 private:
  bool isLedOn;

  GPIOPortPin* pin;
  Timer timer;

  uint32_t ledBlinkDurationMs;
  bool isBlinkEnabled = false;
  uint16_t ledOnMs;
  uint16_t ledOffMs;
};