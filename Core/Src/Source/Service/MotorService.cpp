#include "Service/MotorService.h"

#include "App.h"
#include "Config/StaticConfig.h"

uint64_t MotorService::timestampLastDataUpdate = 0;
uint8_t MotorService::powerPercentageLevel = 0;
uint8_t MotorService::driveDirection = 0;
Function<uint64_t()> MotorService::timeBase = nullptr;

void MotorService::init(Function<uint64_t()> timeBase) { MotorService::timeBase = timeBase; }

void MotorService::update() {
  if (timeBase() - timestampLastDataUpdate > MOTOR_DATA_MAX_DELAY) {
    setSpeed(0, 0);
    timestampLastDataUpdate = timeBase();
  }
}

void MotorService::setSpeed(uint8_t powerPercentageLevel, uint8_t driveDirection) {
  MotorService::powerPercentageLevel = powerPercentageLevel;
  MotorService::driveDirection = driveDirection;
  if (MotorService::driveDirection == DriveDirection::FORWARD) {
    App::getMotorInstance().setForwardMotorSpeed(MotorService::powerPercentageLevel);
  } else if (MotorService::driveDirection == DriveDirection::REVERSE) {
    App::getMotorInstance().setReverseMotorSpeed(MotorService::powerPercentageLevel);
  }

  timestampLastDataUpdate = timeBase();
}
