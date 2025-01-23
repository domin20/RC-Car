#include "Motor.h"

void Motor::init(HBridgeContext* context) { this->context = context; }

void Motor::setForwardMotorSpeed(uint8_t percentageValue) {
  if (this->context) {
    this->context->enableForward();
    if (this->context->setMotorPowerUsingPwmValue) {
      this->context->setMotorPowerUsingPwmValue(percentageValue);
    }
  }
}

void Motor::setReverseMotorSpeed(uint8_t percentageValue) {
  if (this->context) {
    this->context->enableReverse();
    if (this->context->setMotorPowerUsingPwmValue) {
      this->context->setMotorPowerUsingPwmValue(percentageValue);
    }
  }
}
