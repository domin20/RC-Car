#pragma once

#include <stdint.h>

#include "AppHelper/Env/AppEnvironment.h"

class Timer {
 public:
  inline void setUpFor(uint32_t interval) {
    this->interval = interval;
    this->previousTime = AppEnvironment::getEnvironmentContext()->timeBaseMs();
    this->_isElapsed = false;
  }

  inline void update() {
    uint32_t currentTime = AppEnvironment::getEnvironmentContext()->timeBaseMs();
    if (this->_isElapsed == false && currentTime - this->previousTime > this->interval) {
      this->_isElapsed = true;
    }
  }
  inline void stop() { this->_isElapsed = true; }

  inline bool isElapsed() const { return this->_isElapsed; }

 private:
  uint32_t previousTime;
  uint32_t interval = 0;
  bool _isElapsed = true;
};
