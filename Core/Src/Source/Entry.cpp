#include "App.h"
#include "main.h"

extern "C" void appStart() { App::setup(); }

extern "C" void appMainLoop() { App::mainLoop(); }