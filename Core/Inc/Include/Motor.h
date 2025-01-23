#pragma once

#include "GPIOPortPin.h"
#include "Types.h"

struct HBridgeContext {
  GPIOPortPin goForwardPin;
  GPIOPortPin goReversePin;

  void enableForward() {
    goReversePin.reset();
    goForwardPin.set();
  }

  void enableReverse() {
    goForwardPin.reset();
    goReversePin.set();
  }

  Function<void(uint8_t percentageValue)> setMotorPowerUsingPwmValue = nullptr;
};

class Motor {
 public:
  void init(HBridgeContext* context);

  void setForwardMotorSpeed(uint8_t percentageValue);
  void setReverseMotorSpeed(uint8_t percentageValue);

 private:
  HBridgeContext* context;
};