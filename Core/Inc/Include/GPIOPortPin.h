#pragma once

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

struct GPIOPortPin {
  GPIO_TypeDef* port;
  uint16_t pin;

  inline void set() { HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET); }
  inline void clear() { HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET); }
  inline void toggle() { HAL_GPIO_TogglePin(port, pin); }
  inline GPIO_PinState read() { return HAL_GPIO_ReadPin(port, pin); }
};
