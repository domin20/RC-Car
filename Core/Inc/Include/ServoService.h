#pragma once
#include <stdint.h>

#include "Types.h"

class ServoService {
 public:
  static void init(Function<void(uint16_t duty)> setPwmDutyCycle);

  static void setPosition(uint8_t deflectionPercentage, uint8_t direction);

 private:
  static Function<void(uint16_t duty)> setPwmDutyCycle;
};
