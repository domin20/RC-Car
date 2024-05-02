#pragma once
#include "GPIOPortPin.h"
#include "Types.h"

struct HBridgeContext {
  GPIOPortPin rightEnabled;
  GPIOPortPin leftEnabled;

  Function<void(uint8_t)> setForwardMotorPower;
  Function<void(uint8_t)> setReverseMotorPower;
};

class Motor {
 public:
  void init(HBridgeContext& context);

 private:
  HBridgeContext& context;
};