#pragma once
#include <etl/map.h>

#include "Config/StaticConfig.h"
#include "GPIOPortPin.h"
#include "Types.h"
#include "Utils/Timer.h"

class Button {
 public:
  Button(Function<uint64_t()> timeBase);

  void update();
  inline void setPin(GPIOPortPin* pin) { this->pin = pin; }
  inline void addOnClick(Function<void()> onClick) { this->onClick = onClick; }
  inline void addOnHold(uint32_t holdTimeMs, Function<void()> onHold) {
    onHoldFunctions[holdTimeMs] = onHold;
  }

 private:
  enum class State : uint8_t {
    IDLE,
    DEBOUNCE,
    PRESSED,
    WAIT_FOR_HOLD,
    HOLD,
  };

 private:
  GPIOPortPin* pin;
  Function<void()> onClick;
  etl::map<uint32_t, Function<void()>, MAX_ON_HOLD_FUNCTIONS> onHoldFunctions;
  Function<uint64_t()> timeBase;
  State state;
  Timer timer;
  Timer buttonReadTimer;
};
