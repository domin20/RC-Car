#pragma once

#include <main.h>
#include <stdint.h>

#pragma pack(push, 1)

struct AdcBuffer {
  uint16_t mainSupplyVoltageRawValue;
  uint16_t dcMotorVoltageRawValue;
};

#pragma pack(pop)

class AdcManager {
 public:
  static void init(ADC_HandleTypeDef* hadc);

  static AdcBuffer getBuffer();

  static float getMainSupplyVoltage();
  static float getDcMotorVoltage();

 private:
  static AdcBuffer buffer;
};
