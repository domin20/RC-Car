#pragma once
#include <stdint.h>

#include "Types.h"

class ServoService {
 public:
  enum Direction { LEFT = 0, RIGHT = 1 };

  static void init(Function<void(uint16_t duty)> setPwmDutyCycle);

  static void setPosition(uint8_t deflectionPercentage, Direction direction);

 private:
  static Function<void(uint16_t duty)> setPwm;
};
