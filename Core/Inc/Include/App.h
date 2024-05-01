#pragma once

#include "Wireless/HC12Module.h"

class App {
 public:
  static void setup();
  static void mainLoop();

  static void updateTimeBaseUs();
  static uint64_t getTimeBaseUs();
  static uint64_t getTimeBaseMs();

  static HC12Module& getRadioModule();

 private:
  static uint64_t appTimeMs;
  static uint64_t appTimeUs;
  static HC12Module radioModule;
};