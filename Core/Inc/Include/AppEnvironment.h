#pragma once
#include <stdint.h>

struct AppEnvironmentContext {
  virtual uint64_t timeBase() = 0;
};

class AppEnvironment {
 public:
  static void setAppContext(AppEnvironmentContext* appContext) { context = appContext; }
  static AppEnvironmentContext* getEnvironmentContext() { return context; }

 private:
  static AppEnvironmentContext* context;
};
