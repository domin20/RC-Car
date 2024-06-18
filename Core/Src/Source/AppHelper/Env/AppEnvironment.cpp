#include "AppHelper/Env/AppEnvironment.h"

AppEnvironmentContext* AppEnvironment::context = nullptr;

void AppEnvironment::setAppEnvironmentContext(AppEnvironmentContext* appContext) {
  context = appContext;
}

AppEnvironmentContext* AppEnvironment::getEnvironmentContext() { return context; }
