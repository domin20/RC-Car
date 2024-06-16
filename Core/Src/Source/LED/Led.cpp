#include "Led/Led.h"

#include "GPIOPortPin.h"

void Led::setPin(GPIOPortPin* portPin) { pin = portPin; }

void Led::update() {
  this->timer.update();
  if (!this->isBlinkEnabled) {
    return;
  }
  if (this->timer.isElapsed() && this->ledBlinkDurationMs) {
    if (this->isLedOn) {
      this->ledOff();
      if (this->ledOffMs > this->ledBlinkDurationMs) {
        this->timer.setUpFor(this->ledBlinkDurationMs);
        this->ledBlinkDurationMs = 0;
      } else {
        this->timer.setUpFor(this->ledOffMs);
        this->ledBlinkDurationMs -= this->ledOffMs;
      }
    } else {
      this->ledOn();
      if (this->ledOnMs > this->ledBlinkDurationMs) {
        this->timer.setUpFor(this->ledBlinkDurationMs);
        this->ledBlinkDurationMs = 0;
      } else {
        this->timer.setUpFor(this->ledOnMs);
        this->ledBlinkDurationMs -= this->ledOnMs;
      }
    }
  } else if (this->timer.isElapsed() && !this->ledBlinkDurationMs && this->isLedOn) {
    this->ledOff();
    this->isBlinkEnabled = false;
  }
}

void Led::ledOn() {
  if (this->pin != nullptr) {
    HAL_GPIO_WritePin(pin->port, pin->pin, GPIO_PIN_SET);
    this->isLedOn = true;
  }
}

void Led::ledOff() {
  if (this->pin != nullptr) {
    HAL_GPIO_WritePin(pin->port, pin->pin, GPIO_PIN_RESET);
    this->isLedOn = false;
  }
}

void Led::ledToggle() {
  if (this->pin != nullptr) {
    HAL_GPIO_TogglePin(pin->port, pin->pin);
    this->isLedOn = !isLedOn;
  }
}

void Led::setBlink(uint16_t timeOnMs, uint16_t timeOffMs, uint32_t durationMs) {
  this->stopBlink();
  this->isBlinkEnabled = true;
  this->ledOnMs = timeOnMs;
  this->ledOffMs = timeOffMs;
  this->ledBlinkDurationMs = durationMs;
}

void Led::stopBlink() {
  this->ledOnMs = 0;
  this->ledOffMs = 0;
  this->ledBlinkDurationMs = 0;
  this->isBlinkEnabled = false;
  this->timer.stop();
  this->ledOff();
}
