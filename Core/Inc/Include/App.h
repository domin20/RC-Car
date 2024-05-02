#pragma once

#include "Motor.h"
#include "Wireless/HC12Module.h"

class App {
 public:
  static void setup();
  static void mainLoop();

  // setting methods
  static void setTimers();
  static void setMotorContext();

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
};