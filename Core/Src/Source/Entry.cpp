#include "App.h"
#include "main.h"
#include "stm32f4xx_it.h"

extern "C" void appStart() { App::setup(); }

extern "C" void appMainLoop() { App::mainLoop(); }
