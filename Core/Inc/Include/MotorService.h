#pragma once
#include <stdint.h>

#include "Types.h"

class MotorService {
 public:
  static void init(Function<uint64_t()> timeBase);
  static void update();

  static void setSpeed(uint8_t powerPercentageLevel, uint8_t driveDirection);

 private:
  static uint64_t timestampLastDataUpdate;
  static uint8_t powerPercentageLevel;
  static uint8_t driveDirection;
  static Function<uint64_t()> timeBase;
};
