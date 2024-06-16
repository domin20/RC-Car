#include "App.h"

#include <main.h>
#include <time.h>

#include "AdcManager.h"
#include "Config/StaticConfig.h"
#include "LED/LedRGB.h"
#include "Service/MotorService.h"
#include "Service/ServoService.h"
#include "adc.h"
#include "dma.h"
#include "rtc.h"
#include "tim.h"

uint64_t App::timeBaseUs10 = 0;
uint64_t App::timeBaseMs = 0;
Timer App::testTimer;
TestFlags App::testFlags = {false, false};

SecurityLayerType App::securityLayerType = SecurityLayerType::NONE;

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
GPIOPortPin buttonPin = {USER_BUTTON_GPIO_Port, USER_BUTTON_Pin};
GPIOPortPin setPin = {HC12_SET_GPIO_Port, HC12_SET_Pin};

struct STM32F401EnvironmentContext : public AppEnvironmentContext {
  uint64_t timeBaseMs() override { return App::getTimeBaseMs(); }
  uint64_t timeBaseUs10() override { return App::getTimeBaseUs10(); }
  void assertFailed() override { App::assertFailed(); }
  time_t getUnixTimeFromRTC() override { return App::getUnixTime(); }
} appContext;

void App::setup() {
  AppEnvironment::setAppEnvironmentContext(&appContext);
  SecurityLayerRegistry::init();

  App::initTimers();
  App::initButtons();
  App::initMotorContext();
  App::initLedInstances();
  App::initWireless();

  ServoService::init([](uint16_t duty) { App::setPwmDutyCycle(duty); },
                     []() { return App::getTimeBaseMs(); });
  MotorService::init([]() { return App::getTimeBaseMs(); });
  AdcManager::init(&hadc1, &hdma_adc1);

  HAL_Delay(500);
  TEST_LED_OFF;
  LedRGB::setBlink(LedColor::WHITE, 200, 200, 2000);
}

void App::mainLoop() {
  wirelessController.onService();
  button.update();
  radioModule.update();
  LedRGB::update();
  ServoService::update();
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

void App::assertFailed() {
  while (1) {
    LedRGB::ledOn(LedColor::RED);
    HAL_Delay(300);
    LedRGB::ledOff(LedColor::RED);
    HAL_Delay(300);
  }
}

void App::updateTimeBaseUs10() {
  timeBaseUs10++;
  if (timeBaseUs10 % 100 == 0) timeBaseMs++;
}

void App::initTimers() {
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
}

void App::initButtons() {
  button.setPin(&buttonPin);
  button.addOnClick([]() { App::onButtonClick(); });
  button.addOnHold(SERVO_TEST_HOLD_TIME, []() { App::enableServoTest(); });
  button.addOnHold(MOTOR_TEST_HOLD_TIME, []() { App::enableMotorTest(); });
}

void App::initMotorContext() {
  motorContext.goForwardPin = enableForwardDirectionPin;
  motorContext.goReversePin = enableReverseDirectionPin;
  motorContext.setMotorPowerUsingPwmValue = App::setMotorPowerUsingPwm;
  motor.init(&motorContext);
}

void App::initLedInstances() { LedRGB::setRgbPins(&rgbPinSet); }

void App::initWireless() {
  wirelessController.init(&radioModule);
  radioModule.init(&huart1, &setPin, []() { return App::getTimeBaseUs10(); });
}

void App::setMotorPowerUsingPwm(uint8_t percentageValue) {
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, percentageValue);
}

void App::setPwmDutyCycle(uint16_t duty) { __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, duty); }

void App::synchronizeRtcDateTime(time_t time) {
  tm timeStruct;
  localtime_r(&time, &timeStruct);

  RTC_TimeTypeDef rtcTime = {0};
  RTC_DateTypeDef rtcDate = {0};

  rtcTime.Seconds = timeStruct.tm_sec;
  rtcTime.Minutes = timeStruct.tm_min;
  rtcTime.Hours = timeStruct.tm_hour;

  rtcDate.Year = timeStruct.tm_year;
  rtcDate.Month = timeStruct.tm_mon + 1;
  rtcDate.Date = timeStruct.tm_mday;

  rtcDate.WeekDay = timeStruct.tm_wday == 0 ? RTC_WEEKDAY_SUNDAY : timeStruct.tm_wday;

  if (HAL_RTC_SetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN) != HAL_OK) {
    Error_Handler();
  }

  if (HAL_RTC_SetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN) != HAL_OK) {
    Error_Handler();
  }
}

time_t App::getUnixTime() {
  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;

  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

  return rtcToUnixTime(&sTime, &sDate);
}

time_t App::rtcToUnixTime(RTC_TimeTypeDef* rtcTime, RTC_DateTypeDef* rtcDate) {
  struct tm timeinfo;
  timeinfo.tm_year = rtcDate->Year + 100;
  timeinfo.tm_mon = rtcDate->Month - 1;
  timeinfo.tm_mday = rtcDate->Date;
  timeinfo.tm_hour = rtcTime->Hours;
  timeinfo.tm_min = rtcTime->Minutes;
  timeinfo.tm_sec = rtcTime->Seconds;

  return mktime(&timeinfo);
}

void App::onButtonClick() { LedRGB::selfTest(); }

void App::enableServoTest() {
  if (!testFlags.isMotorTestEnabled && !testFlags.isServoTestEnabled) {
    testFlags.isServoTestEnabled = true;
    TEST_LED_ON;
    button.block();
  }
}

void App::enableMotorTest() {
  if (!testFlags.isMotorTestEnabled && !testFlags.isServoTestEnabled) {
    testFlags.isMotorTestEnabled = true;
    TEST_LED_ON;
    button.block();
  }
}

void App::servoTest() {
  if (!testTimer.isElapsed()) {
    return;
  }
  static uint8_t servoStep = 0;
  static uint8_t servoPos = 0;
  switch (servoStep) {
    case 0:
      if (servoPos < 100) {
        ServoService::setPosition(servoPos, TurnDirection::LEFT);
        testTimer.setUpFor(10);
        servoPos++;
      } else {
        servoStep = 1;
      }
      break;

    case 1:
      if (servoPos > 0) {
        ServoService::setPosition(servoPos, TurnDirection::LEFT);
        testTimer.setUpFor(10);
        servoPos--;
      } else {
        servoStep = 2;
      }
      break;
    case 2:
      if (servoPos < 100) {
        ServoService::setPosition(servoPos, TurnDirection::RIGHT);
        testTimer.setUpFor(10);
        servoPos++;
      } else {
        servoStep = 3;
      }
      break;

    case 3:
      if (servoPos > 0) {
        ServoService::setPosition(servoPos, TurnDirection::RIGHT);
        testTimer.setUpFor(10);
        servoPos--;
      } else {
        servoStep = 4;
      }
      break;
    case 4:
      servoStep = 0;
      testFlags.isServoTestEnabled = false;
      button.unblock();
      TEST_LED_OFF;
      break;
  }
}

void App::motorTest() {
  if (!testTimer.isElapsed()) {
    return;
  }
  static uint8_t motorValue = 0;
  static uint8_t motorTestStep = 0;
  switch (motorTestStep) {
    case 0:
      if (motorValue < 100) {
        motor.setForwardMotorSpeed(motorValue);
        motorValue++;
        testTimer.setUpFor(10);
      } else {
        motorTestStep = 1;
      }
      break;

    case 1:
      if (motorValue > 0) {
        motor.setForwardMotorSpeed(motorValue);
        motorValue--;
        testTimer.setUpFor(10);
      } else {
        motorTestStep = 2;
      }
      break;

    case 2:
      if (motorValue < 100) {
        motor.setReverseMotorSpeed(motorValue);
        motorValue++;
        testTimer.setUpFor(10);
      } else {
        motorTestStep = 3;
      }
      break;

    case 3:
      if (motorValue > 0) {
        motor.setReverseMotorSpeed(motorValue);
        motorValue--;
        testTimer.setUpFor(10);
      } else {
        motorTestStep = 0;
        testFlags.isMotorTestEnabled = false;
        button.unblock();
        TEST_LED_OFF;
      }
      break;
  }
}

uint64_t App::getTimeBaseUs10() { return timeBaseUs10; }

uint64_t App::getTimeBaseMs() { return timeBaseMs; }

void App::performSteeringData(const SteeringData& steeringData) {
  MotorService::setSpeed(steeringData.speedJoystickDeflection, steeringData.speedJoystickDirection);
  ServoService::setPosition(steeringData.turnJoystickDeflection,
                            steeringData.turnJoystickDirection);
}
