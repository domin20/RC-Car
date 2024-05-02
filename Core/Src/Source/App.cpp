#include "App.h"

#include <main.h>

#include "Config/StaticConfig.h"
#include "tim.h"

uint64_t App::appTimeUs = 0;
uint64_t App::appTimeMs = 0;
HC12Module App::radioModule;

void App::setup() {
  HAL_TIM_Base_Start_IT(&htim2);
  radioModule.init(&huart1, App::getTimeBaseUs);
}

void App::mainLoop() {}

void App::updateTimeBaseUs() {
  appTimeUs++;
  if (appTimeUs % 1000 == 0) appTimeMs++;
}

uint64_t App::getTimeBaseUs() { return appTimeUs; }

uint64_t App::getTimeBaseMs() { return appTimeMs; }

HC12Module& App::getRadioModule() { return radioModule; }
