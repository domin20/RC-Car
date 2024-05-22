#pragma once
#include <stdint.h>

struct AppEnvironmentContext {
  virtual uint64_t timeBaseMs() = 0;
  virtual uint64_t timeBaseUs() = 0;
};

class AppEnvironment {
 public:
  static void setAppEnvironmentContext(AppEnvironmentContext* appContext) { context = appContext; }
  static AppEnvironmentContext* getEnvironmentContext() { return context; }

 private:
  static AppEnvironmentContext* context;
};
