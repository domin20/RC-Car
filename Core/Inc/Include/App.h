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

  // tests
  static void servoTest();
  static void motorTest();

  static void updateTimeBaseUs();

  // getters
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