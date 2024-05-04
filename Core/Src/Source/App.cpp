#include "App.h"

#include <main.h>

#include "AdcManager.h"
#include "Config/StaticConfig.h"
#include "Service/MotorService.h"
#include "Service/ServoService.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"

uint64_t App::appTimeUs = 0;
uint64_t App::appTimeMs = 0;

WirelessController App::wirelessController;
Motor App::motor;
HC12Module App::radioModule;

HBridgeContext motorContext;
GPIOPortPin enableForwardDirectionPin = {FORWARD_ENABLE_GPIO_Port, FORWARD_ENABLE_Pin};
GPIOPortPin enableReverseDirectionPin = {REVERSE_ENABLE_GPIO_Port, REVERSE_ENABLE_Pin};

GPIOPortPin serviceLedPin = {SERVICE_LED_GPIO_Port, SERVICE_LED_Pin};

struct STM32F401EnvironmentContext : public AppEnvironmentContext {
  uint64_t timeBaseMs() override { return App::getTimeBaseMs(); }
  uint64_t timeBaseUs() override { return App::getTimeBaseUs(); }
} appContext;

void App::setup() {
  AppEnvironment::setAppEnvironmentContext(&appContext);

  App::initTimers();
  App::initMotorContext();
  App::initLedInstances();
  AdcManager::init(&hadc1, &hdma_adc1);

  wirelessController.init(&radioModule);
  radioModule.init(&huart1, App::getTimeBaseUs);
}

void App::mainLoop() {
  wirelessController.onService();
  MotorService::update();
}

void App::updateTimeBaseUs() {
  appTimeUs++;
  if (appTimeUs % 1000 == 0) appTimeMs++;
}

void App::initTimers() { HAL_TIM_Base_Start_IT(&htim2); }

void App::initMotorContext() {
  motorContext.goForwardPin = enableForwardDirectionPin;
  motorContext.goReversePin = enableReverseDirectionPin;
  motorContext.setMotorPowerUsingPwmValue = App::setMotorPowerUsingPwm;
  motor.init(&motorContext);
}

void App::initLedInstances() { serviceLed.setPin(&serviceLedPin); }

void App::setMotorPowerUsingPwm(uint8_t percentageValue) {
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, percentageValue);
}

uint64_t App::getTimeBaseUs() { return appTimeUs; }

uint64_t App::getTimeBaseMs() { return appTimeMs; }

void App::performSteeringData(const SteeringData& steeringData) {
  MotorService::setSpeed(steeringData.speedJoystickDeflection, steeringData.speedJoystickDirection);
  ServoService::setPosition(steeringData.turnJoystickDeflection,
                            steeringData.turnJoystickDirection);
}
