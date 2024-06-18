#pragma once
#include <stdint.h>
#include <time.h>

struct AppEnvironmentContext {
  virtual uint64_t timeBaseMs() = 0;
  virtual uint64_t timeBaseUs10() = 0;
  virtual void assertFailed() = 0;
  virtual time_t getUnixTimeFromRTC() = 0;
};

class AppEnvironment {
 public:
  static void setAppEnvironmentContext(AppEnvironmentContext* appContext);
  static AppEnvironmentContext* getEnvironmentContext();

 private:
  static AppEnvironmentContext* context;
};
