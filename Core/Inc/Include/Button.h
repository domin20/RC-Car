#pragma once
#include <etl/delegate.h>
#include <etl/map.h>

#include "Config/StaticConfig.h"
#include "GPIOPortPin.h"
#include "Types.h"
#include "Utils/Timer.h"

class Button {
 public:
  Button(etl::delegate<uint64_t()> timeBase);

  void update();
  void block() { isBlocked = true; }
  void unblock() { isBlocked = false; }
  inline void setPin(GPIOPortPin* pin) { this->pin = pin; }
  inline void addOnClick(etl::delegate<void()> onClick) { this->onClick = onClick; }
  inline void addOnHold(uint32_t holdTimeMs, etl::delegate<void()> onHold) {
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
  bool isBlocked = false;
  GPIOPortPin* pin;
  etl::delegate<void()> onClick;
  etl::map<uint32_t, etl::delegate<void()>, MAX_ON_HOLD_FUNCTIONS> onHoldFunctions;
  etl::delegate<uint64_t()> timeBase;
  State state;
  Timer timer;
  Timer buttonReadTimer;
};
