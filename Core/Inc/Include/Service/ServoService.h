#pragma once
#include <etl/delegate.h>
#include <stdint.h>

#include "Types.h"

class ServoService {
 public:
  static void init(etl::delegate<void(uint16_t duty)> setPwmDutyCycle,
                   etl::delegate<uint64_t()> timeBase);
  static void update();
  static void setPosition(uint8_t deflectionPercentage, uint8_t direction);

 private:
  static uint64_t timestampLastDataUpdate;
  static etl::delegate<void(uint16_t duty)> setPwmDutyCycle;
  static etl::delegate<uint64_t()> timeBase;
};
