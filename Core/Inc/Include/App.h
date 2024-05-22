#pragma once

#include "Button.h"
#include "LED/Led.h"
#include "Motor.h"
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

  // init methods
  static void initTimers();
  static void initMotorContext();
  static void initLedInstances();

  static void setMotorPowerUsingPwm(uint8_t percentageValue);

  // button methods
  static void onButtonClick();
  static void enableServoTest();
  static void enableMotorTest();

  static void servoTest();
  static void motorTest();

  static void updateTimeBaseUs();
  static uint64_t getTimeBaseUs();
  static uint64_t getTimeBaseMs();

  static HC12Module& getRadioModule() { return radioModule; }
  static Motor& getMotorInstance() { return motor; }

  static void performSteeringData(const SteeringData& steeringData);

 private:
  static uint64_t appTimeMs;
  static uint64_t appTimeUs;
  static Timer testTimer;
  static TestFlags testFlags;

  static WirelessController wirelessController;
  static Motor motor;
  static HC12Module radioModule;

  static Button button;
};