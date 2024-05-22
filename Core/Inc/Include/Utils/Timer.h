#pragma once

#include <stdint.h>

#include "../Types.h"
#include "App.h"

class Timer {
 public:
  inline void setUpFor(uint32_t interval) {
    this->interval = interval;
    this->previousTime = App::getTimeBaseMs();
    this->_isElapsed = false;
  }

  inline void update() {
    uint32_t currentTime = App::getTimeBaseMs();
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
