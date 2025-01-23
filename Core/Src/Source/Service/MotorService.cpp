#include "Service/MotorService.h"

#include "App.h"
#include "Config/StaticConfig.h"

uint64_t MotorService::timestampLastDataUpdate = 0;
uint8_t MotorService::powerPercentageLevel = 0;
uint8_t MotorService::driveDirection = 0;
etl::delegate<uint64_t()> MotorService::timeBase;

void MotorService::init(etl::delegate<uint64_t()> timeBase) { MotorService::timeBase = timeBase; }

void MotorService::update() {
  assert(timeBase);
  if (timeBase() - timestampLastDataUpdate > MOTOR_DATA_MAX_DELAY) {
    setSpeed(0, 0);
    timestampLastDataUpdate = timeBase();
  }
}

void MotorService::setSpeed(uint8_t powerPercentageLevel, uint8_t driveDirection) {
  assert(timeBase);
  MotorService::powerPercentageLevel = powerPercentageLevel;
  MotorService::driveDirection = driveDirection;
  timestampLastDataUpdate = timeBase();
  if (MotorService::driveDirection == DriveDirection::FORWARD) {
    App::getMotorInstance().setForwardMotorSpeed(MotorService::powerPercentageLevel);
  } else if (MotorService::driveDirection == DriveDirection::REVERSE) {
    App::getMotorInstance().setReverseMotorSpeed(MotorService::powerPercentageLevel);
  }

  timestampLastDataUpdate = timeBase();
}
