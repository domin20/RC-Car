#include "App.h"

#include <main.h>

#include "AdcManager.h"
#include "Config/StaticConfig.h"
#include "LED/LedRGB.h"
#include "Service/MotorService.h"
#include "Service/ServoService.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"

uint64_t App::appTimeUs = 0;
uint64_t App::appTimeMs = 0;
Timer App::testTimer;
TestFlags App::testFlags = {false, false};

WirelessController App::wirelessController;
Motor App::motor;
HC12Module App::radioModule;

Button App::button([]() { return App::getTimeBaseMs(); });

HBridgeContext motorContext;
GPIOPortPin enableForwardDirectionPin = {FORWARD_ENABLE_GPIO_Port, FORWARD_ENABLE_Pin};
GPIOPortPin enableReverseDirectionPin = {REVERSE_ENABLE_GPIO_Port, REVERSE_ENABLE_Pin};

RgbPinSet rgbPinSet = {{LED_RGB_R_GPIO_Port, LED_RGB_R_Pin},
                       {LED_RGB_G_GPIO_Port, LED_RGB_G_Pin},
                       {LED_RGB_B_GPIO_Port, LED_RGB_B_Pin}};
GPIOPortPin serviceLedPin = {LED_RGB_B_GPIO_Port, LED_RGB_B_Pin};

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

  button.addOnClick([]() { App::onButtonClick(); });
  button.addOnHold(SERVO_TEST_HOLD_TIME, []() { App::servoTest(); });
  button.addOnHold(MOTOR_TEST_HOLD_TIME, []() { App::motorTest(); });

  wirelessController.init(&radioModule);
  radioModule.init(&huart1, App::getTimeBaseUs);

  LedRGB::setBlink(LedColor::WHITE, 200, 200, 1200);
}

void App::mainLoop() {
  wirelessController.onService();
  button.update();
  LedRGB::update();
  MotorService::update();
  testLoop();
}

void App::testLoop() {
  testTimer.update();
  if (testFlags.isServoTestEnabled) {
    servoTest();
  } else if (testFlags.isMotorTestEnabled) {
    motorTest();
  }
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

void App::initLedInstances() { LedRGB::setRgbPins(&rgbPinSet); }

void App::setMotorPowerUsingPwm(uint8_t percentageValue) {
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, percentageValue);
}

void App::onButtonClick() { LedRGB::selfTest(); }

void App::enableServoTest() {
  if (!testFlags.isMotorTestEnabled && !testFlags.isServoTestEnabled) {
    testFlags.isServoTestEnabled = true;
    button.block();
  }
}

void App::enableMotorTest() {
  if (!testFlags.isMotorTestEnabled && !testFlags.isServoTestEnabled) {
    testFlags.isMotorTestEnabled = true;
    button.block();
  }
}

void App::servoTest() {
  if (!testTimer.isElapsed()) {
    return;
  }
  static uint8_t step = 0;

  switch (step) {
    case 0:
      ServoService::setPosition(100, TurnDirection::LEFT);
      testTimer.setUpFor(500);
      step = 1;
      break;

    case 1:
      ServoService::setPosition(100, TurnDirection::RIGHT);
      testTimer.setUpFor(500);
      step = 2;
      break;
    case 2:
      ServoService::setPosition(0, TurnDirection::LEFT);
      testTimer.setUpFor(500);
      step = 3;
      break;
    case 3:
      step = 0;
      testFlags.isServoTestEnabled = false;
      break;
  }
}

void App::motorTest() {
  if (!testTimer.isElapsed()) {
    return;
  }
  static uint8_t value = 0;
  static uint8_t step = 0;
  switch (step) {
    case 0:
      if (value < 100) {
        motor.setForwardMotorSpeed(value);
        value++;
        testTimer.setUpFor(10);
      } else {
        step = 1;
      }
      break;

    case 1:
      if (value > 0) {
        motor.setForwardMotorSpeed(value);
        value--;
        testTimer.setUpFor(10);
      } else {
        step = 2;
      }
      break;

    case 2:
      if (value < 100) {
        motor.setReverseMotorSpeed(value);
        value++;
        testTimer.setUpFor(10);
      } else {
        step = 3;
      }
      break;

    case 3:
      if (value > 0) {
        motor.setReverseMotorSpeed(value);
        value--;
        testTimer.setUpFor(10);
      } else {
        step = 0;
        testFlags.isMotorTestEnabled = false;
        LedRGB::ledOff(LedColor::GREEN);
      }
      break;
  }
}

uint64_t App::getTimeBaseUs() { return appTimeUs; }

uint64_t App::getTimeBaseMs() { return appTimeMs; }

void App::performSteeringData(const SteeringData& steeringData) {
  MotorService::setSpeed(steeringData.speedJoystickDeflection, steeringData.speedJoystickDirection);
  ServoService::setPosition(steeringData.turnJoystickDeflection,
                            steeringData.turnJoystickDirection);
}
