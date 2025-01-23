#include "Button.h"

#define IS_PRESSED 0
#define IS_RELEASED 1

Button::Button(etl::delegate<uint64_t()> timeBase)
    : onClick(), timeBase(timeBase), state(State::IDLE) {}

void Button::update() {
  if (this->isBlocked) {
    return;
  }
  this->timer.update();
  this->buttonReadTimer.update();
  static uint32_t pressTimestamp = 0;
  static bool buttonState = IS_RELEASED;
  if (pin != nullptr && this->buttonReadTimer.isElapsed()) {
    buttonState = this->pin->read();
    this->buttonReadTimer.setUpFor(20);
  }

  switch (this->state) {
    case State::IDLE: {
      if (buttonState == IS_PRESSED) {
        pressTimestamp = timeBase();
        this->state = State::DEBOUNCE;
        this->timer.setUpFor(BUTTON_DEBOUNCE_TIME);
      }
      break;
    }
    case State::DEBOUNCE: {
      if (this->timer.isElapsed()) {
        if (buttonState == IS_PRESSED) {
          this->state = State::PRESSED;
          this->timer.setUpFor(BUTTON_DEBOUNCE_TIME);
        } else {
          state = State::IDLE;
        }
      }
      break;
    }
    case State::PRESSED:
      if (this->timer.isElapsed()) {
        if (buttonState == IS_RELEASED) {
          if (this->onClick) {
            this->onClick();
          }
          this->state = State::IDLE;
        } else {
          this->timer.setUpFor(BUTTON_WAIT_FOR_HOLD_TIME);
          this->state = State::WAIT_FOR_HOLD;
        }
        break;
      }

    case State::WAIT_FOR_HOLD: {
      if (buttonState == IS_RELEASED) {
        if (this->onClick) {
          this->onClick();
        }
        this->state = State::IDLE;
      } else if (this->timer.isElapsed()) {
        this->state = State::HOLD;
      }
      break;
    }

    case State::HOLD: {
      if (buttonState == IS_RELEASED) {
        uint32_t pressDurationMs = timeBase() - pressTimestamp;
        auto iterator = onHoldFunctions.lower_bound(pressDurationMs);
        if (iterator == onHoldFunctions.end() ||
            (iterator != onHoldFunctions.begin() && iterator->first > pressDurationMs)) {
          --iterator;
        }
        if (iterator->first <= pressDurationMs) {
          (iterator->second)();
        }
        this->state = State::IDLE;
      }
      break;
    }
  }
}
