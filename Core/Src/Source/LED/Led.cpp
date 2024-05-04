#include "Led/Led.h"

#include "GPIOPortPin.h"

void Led::setPin(GPIOPortPin* portPin) { pin = portPin; }

void Led::update() {
  timer.update();
  if (timer.isElapsed() && ledBlinkDurationMs) {
    if (isLedOn) {
      ledOff();
      if (ledOffMs > ledBlinkDurationMs) {
        timer.setUpFor(ledBlinkDurationMs);
        ledBlinkDurationMs = 0;
      } else {
        timer.setUpFor(ledOffMs);
        ledBlinkDurationMs -= ledOffMs;
      }
    } else {
      ledOn();
      if (ledOnMs > ledBlinkDurationMs) {
        timer.setUpFor(ledBlinkDurationMs);
        ledBlinkDurationMs = 0;
      } else {
        timer.setUpFor(ledOnMs);
        ledBlinkDurationMs -= ledOnMs;
      }
    }
  } else if (timer.isElapsed() && !ledBlinkDurationMs && isLedOn) {
    ledOff();
  }
}

void Led::ledOn() {
  if (pin != nullptr) {
    HAL_GPIO_WritePin(pin->port, pin->pin, GPIO_PIN_RESET);
    isLedOn = true;
  }
}

void Led::ledOff() {
  if (pin != nullptr) {
    HAL_GPIO_WritePin(pin->port, pin->pin, GPIO_PIN_SET);
    isLedOn = false;
  }
}

void Led::ledToggle() {
  if (pin != nullptr) {
    HAL_GPIO_TogglePin(pin->port, pin->pin);
    isLedOn = !isLedOn;
  }
}

void Led::setBlink(uint16_t timeOnMs, uint16_t timeOffMs, uint32_t durationMs) {
  stopBlink();
  ledOnMs = timeOnMs;
  ledOffMs = timeOffMs;
  ledBlinkDurationMs = durationMs;
}

void Led::stopBlink() {
  ledOnMs = 0;
  ledOffMs = 0;
  ledBlinkDurationMs = 0;
  timer.stop();
  ledOff();
}
