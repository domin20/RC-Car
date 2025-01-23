#include "Service/ServoService.h"

#include "AppHelper/Utils/Utils.h"
#include "Config/StaticConfig.h"

uint64_t ServoService::timestampLastDataUpdate = 0;
etl::delegate<void(uint16_t duty)> ServoService::setPwmDutyCycle;
etl::delegate<uint64_t()> ServoService::timeBase;

void ServoService::init(etl::delegate<void(uint16_t duty)> setPwmDutyCycle,
                        etl::delegate<uint64_t()> timeBase) {
  ServoService::setPwmDutyCycle = setPwmDutyCycle;
  ServoService::timeBase = timeBase;
}

void ServoService::update() {
  if (timeBase() - timestampLastDataUpdate > SERVO_DATA_MAX_DELAY) {
    setPosition(0, 0);
    timestampLastDataUpdate = timeBase();
  }
}

void ServoService::setPosition(uint8_t deflectionPercentage, uint8_t direction) {
  uint16_t value = SERVO_ZERO_POSITION;
  timestampLastDataUpdate = timeBase();
  if (direction == TurnDirection::LEFT) {
    value = map(deflectionPercentage, 0, 100, SERVO_ZERO_POSITION, SERVO_MAX_POSITION);
  } else if (direction == TurnDirection::RIGHT) {
    value = map(deflectionPercentage, 0, 100, SERVO_ZERO_POSITION, SERVO_MIN_POSITION);
  }
  setPwmDutyCycle(value);
}
