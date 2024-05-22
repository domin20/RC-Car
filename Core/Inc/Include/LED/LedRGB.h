#pragma once
#include "Led.h"

struct RgbPinSet {
  GPIOPortPin red;
  GPIOPortPin green;
  GPIOPortPin blue;
};

enum LedColor : uint8_t { RED = 0, GREEN = 1, BLUE = 2, WHITE = 3 };

class LedRGB {
 public:
  static void setRgbPins(RgbPinSet* rgbPinSet);
  static void update();

  static void ledOn(LedColor color);
  static void ledOff(LedColor color);
  static void ledToggle(LedColor color);

  static void setBlink(LedColor color, uint16_t timeOnMs, uint16_t timeOffMs,
                       uint32_t durationMs = UINT32_MAX);
  static void stopBlink(LedColor color);

 private:
  static Led red;
  static Led green;
  static Led blue;
};
