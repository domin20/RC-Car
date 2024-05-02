#include "App.h"

#include <main.h>

#include "Config/StaticConfig.h"
#include "tim.h"

uint64_t App::appTimeUs = 0;
uint64_t App::appTimeMs = 0;

Motor App::dcMotor;
HC12Module App::radioModule;

HBridgeContext motorContext;
GPIOPortPin enableForwardDirectionPin = {FORWARD_ENABLE_GPIO_Port, FORWARD_ENABLE_Pin};
GPIOPortPin enableReverseDirectionPin = {REVERSE_ENABLE_GPIO_Port, REVERSE_ENABLE_Pin};

GPIOPortPin serviceLedPin = {SERVICE_LED_GPIO_Port, SERVICE_LED_Pin};

void App::setup() {
  App::initTimers();
  App::initMotorContext();
  App::initLedInstances();

  radioModule.init(&huart1, App::getTimeBaseUs);
}

void App::mainLoop() {}

void App::updateTimeBaseUs() {
  appTimeUs++;
  if (appTimeUs % 1000 == 0) appTimeMs++;
}

void App::initTimers() { HAL_TIM_Base_Start_IT(&htim2); }

void App::initMotorContext() {
  motorContext.goForwardPin = enableForwardDirectionPin;
  motorContext.goReversePin = enableReverseDirectionPin;
  motorContext.setMotorPowerUsingPwmValue = App::setMotorPowerUsingPwm;
  dcMotor.init(&motorContext);
}

void App::initLedInstances() { serviceLed.setPin(&serviceLedPin); }

void App::setMotorPowerUsingPwm(uint8_t percentageValue) {
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, percentageValue);
}

uint64_t App::getTimeBaseUs() { return appTimeUs; }

uint64_t App::getTimeBaseMs() { return appTimeMs; }

HC12Module& App::getRadioModule() { return radioModule; }
