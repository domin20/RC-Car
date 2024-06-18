/*
  App builded by Dominik Kusiak in 22.05.2024

  App software will be developed in the future, I want to add new features for example lighting,
  cruise control, distance sensors on front of vehicle, more capacity of battery pack and power
  control using voltage level set with a digital potentiometer.
*/

#pragma once

#include "Button.h"
#include "LED/Led.h"
#include "Motor.h"
#include "SecurityLayerRegistry.h"
#include "Wireless/HC12Module.h"
#include "Wireless/WirelessController.h"

struct TestFlags {
  bool isMotorTestEnabled;
  bool isServoTestEnabled;
};

class App {
 public:
  static void setup();
  static void mainLoop();

  static void testLoop();

  static bool isTestRunning();
  static void assertFailed();

  // init methods
  static void initTimers();
  static void initButtons();
  static void initMotorContext();
  static void initLedInstances();
  static void initWireless();

  static void setMotorPowerUsingPwm(uint8_t percentageValue);
  static void setPwmDutyCycle(uint16_t duty);

  // RTC
  static void synchronizeRtcDateTime(time_t time);
  static time_t getUnixTime();
  static time_t rtcToUnixTime(RTC_TimeTypeDef* rtcTime, RTC_DateTypeDef* rtcDate);

  // button methods
  static void onButtonClick();
  static void enableServoTest();
  static void enableMotorTest();

  static void switchSecurityLayerType();

  // tests
  static void servoTest();
  static void motorTest();

  static void updateTimeBaseUs10();

  // getters
  static SecurityLayerType getSecurityLayerType() { return securityLayerType; }

  static uint64_t getTimeBaseUs10();
  static uint64_t getTimeBaseMs();

  static HC12Module& getRadioModule() { return radioModule; }
  static WirelessController& getWirelessController() { return wirelessController; }
  static Motor& getMotorInstance() { return motor; }

  static void performSteeringData(const SteeringData& steeringData);

 private:
  static uint64_t timeBaseMs;
  static uint64_t timeBaseUs10;
  static Timer testTimer;
  static TestFlags testFlags;

  static SecurityLayerType securityLayerType;

  static WirelessController wirelessController;
  static Motor motor;
  static HC12Module radioModule;

  static Button button;
};