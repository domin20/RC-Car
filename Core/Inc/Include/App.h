#pragma once

#include "LED/Led.h"
#include "Motor.h"
#include "Wireless/HC12Module.h"
class App {
 public:
  static void setup();
  static void mainLoop();

  // init methods
  static void initTimers();
  static void initMotorContext();
  static void initLedInstances();

  static void setMotorPowerUsingPwm(uint8_t percentageValue);

  static void updateTimeBaseUs();
  static uint64_t getTimeBaseUs();
  static uint64_t getTimeBaseMs();

  static HC12Module& getRadioModule();

 private:
  static uint64_t appTimeMs;
  static uint64_t appTimeUs;

  static Motor dcMotor;
  static HC12Module radioModule;
  static Led serviceLed;
};