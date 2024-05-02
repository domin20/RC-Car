#include "ServoService.h"

#include "Config/StaticConfig.h"
#include "Utils/Utils.h"

void ServoService::init(Function<void(uint16_t duty)> setPwmDutyCycle) { setPwm = setPwmDutyCycle; }

void ServoService::setPosition(uint8_t deflectionPercentage, Direction direction) {
  uint16_t value = SERVO_ZERO_POSITION;
  if (direction == Direction::LEFT) {
    value = map(deflectionPercentage, 0, 100, SERVO_ZERO_POSITION, SERVO_MAX_POSITION);
  } else if (direction == Direction::RIGHT) {
    value = map(deflectionPercentage, 0, 100, SERVO_ZERO_POSITION, SERVO_MIN_POSITION);
  }
  setPwm(value);
}
