#pragma once
#include <etl/delegate.h>
#include <stdint.h>

#include "Types.h"

class MotorService {
 public:
  static void init(etl::delegate<uint64_t()> timeBase);
  static void update();

  static void setSpeed(uint8_t powerPercentageLevel, uint8_t driveDirection);

 private:
  static uint64_t timestampLastDataUpdate;
  static uint8_t powerPercentageLevel;
  static uint8_t driveDirection;
  static etl::delegate<uint64_t()> timeBase;
};
