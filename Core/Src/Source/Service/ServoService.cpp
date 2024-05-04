#include "Service/ServoService.h"

#include "Config/StaticConfig.h"
#include "Utils/Utils.h"

Function<void(uint16_t duty)> ServoService::setPwmDutyCycle = nullptr;

void ServoService::init(Function<void(uint16_t duty)> setPwmDutyCycle) {
  ServoService::setPwmDutyCycle = setPwmDutyCycle;
}

void ServoService::setPosition(uint8_t deflectionPercentage, uint8_t direction) {
  uint16_t value = SERVO_ZERO_POSITION;
  if (direction == TurnDirection::LEFT) {
    value = map(deflectionPercentage, 0, 100, SERVO_ZERO_POSITION, SERVO_MAX_POSITION);
  } else if (direction == TurnDirection::RIGHT) {
    value = map(deflectionPercentage, 0, 100, SERVO_ZERO_POSITION, SERVO_MIN_POSITION);
  }
  setPwmDutyCycle(value);
}
