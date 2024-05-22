#include "LED/LedRGB.h"

Led LedRGB::red;
Led LedRGB::green;
Led LedRGB::blue;

void LedRGB::setRgbPins(RgbPinSet* rgbPinSet) {
  red.setPin(&rgbPinSet->red);
  green.setPin(&rgbPinSet->green);
  blue.setPin(&rgbPinSet->blue);
}

void LedRGB::update() {
  red.update();
  green.update();
  blue.update();
}

void LedRGB::selfTest() {
  green.ledOff();
  blue.ledOff();
  red.ledOn();
  HAL_Delay(500);
  red.ledOff();
  green.ledOn();
  HAL_Delay(500);
  green.ledOff();
  blue.ledOn();
  HAL_Delay(500);
  blue.ledOff();
}

void LedRGB::ledOn(LedColor color) {
  if (color == LedColor::RED) red.ledOn();
  if (color == LedColor::GREEN) green.ledOn();
  if (color == LedColor::BLUE) blue.ledOn();
  if (color == LedColor::WHITE) {
    red.ledOn();
    green.ledOn();
    blue.ledOn();
  }
}

void LedRGB::ledOff(LedColor color) {
  if (color == LedColor::RED) red.ledOff();
  if (color == LedColor::GREEN) green.ledOff();
  if (color == LedColor::BLUE) blue.ledOff();
  if (color == LedColor::WHITE) {
    red.ledOff();
    green.ledOff();
    blue.ledOff();
  }
}

void LedRGB::ledToggle(LedColor color) {
  if (color == LedColor::RED) red.ledToggle();
  if (color == LedColor::GREEN) green.ledToggle();
  if (color == LedColor::BLUE) blue.ledToggle();
  if (color == LedColor::WHITE) {
    red.ledToggle();
    green.ledToggle();
    blue.ledToggle();
  }
}

void LedRGB::setBlink(LedColor color, uint16_t timeOnMs, uint16_t timeOffMs, uint32_t durationMs) {
  if (color == LedColor::RED) red.setBlink(timeOnMs, timeOffMs, durationMs);
  if (color == LedColor::GREEN) green.setBlink(timeOnMs, timeOffMs, durationMs);
  if (color == LedColor::BLUE) blue.setBlink(timeOnMs, timeOffMs, durationMs);
  if (color == LedColor::WHITE) {
    red.setBlink(timeOnMs, timeOffMs, durationMs);
    green.setBlink(timeOnMs, timeOffMs, durationMs);
    blue.setBlink(timeOnMs, timeOffMs, durationMs);
  }
}

void LedRGB::stopBlink(LedColor color) {
  if (color == LedColor::RED) red.stopBlink();
  if (color == LedColor::GREEN) green.stopBlink();
  if (color == LedColor::BLUE) blue.stopBlink();
  if (color == LedColor::WHITE) {
    red.stopBlink();
    green.stopBlink();
    blue.stopBlink();
  }
}
