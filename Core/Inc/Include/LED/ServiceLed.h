#pragma once
#include "Led.h"

class ServiceLed : public Led {
 public:
  static void setPin(GPIOPortPin* portPin);
  static void update();

  static void ledOn();
  static void ledOff();
  static void ledToggle();
  static void setBlink(uint16_t timeOnMs, uint16_t timeOffMs, uint32_t durationMs = UINT32_MAX);
  static void stopBlink();

 private:
  static bool isLedOn;

  static GPIOPortPin* pin;
  static Timer timer;

  static uint32_t ledBlinkDurationMs;
  static uint16_t ledOnMs;
  static uint16_t ledOffMs;
};
