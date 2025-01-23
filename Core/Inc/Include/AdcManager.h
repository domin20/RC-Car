#pragma once

#include <main.h>
#include <stdint.h>

constexpr uint8_t RAW_VALUE_BUFFER_SIZE = 3;

#pragma pack(push, 1)

struct AdcBuffer {
  uint16_t mainSupplyVoltageRawValue;
  uint16_t dcMotorVoltageRawValue;
};

struct AdcChannel {
  uint8_t bufferIdx = 0;
  uint16_t rawValueBuffer[RAW_VALUE_BUFFER_SIZE];
  uint16_t adcValue;
};

#pragma pack(pop)

class AdcManager {
 public:
  static void init(ADC_HandleTypeDef* hadc, DMA_HandleTypeDef* hdma);

  static AdcBuffer getBuffer();

  static void endOfConversionCallback();

  static float getMainSupplyVoltage();
  static float getDcMotorVoltage();

 private:
  static void calculateAdcChannelValues(AdcChannel* channel, uint16_t rawAdcBufferValue);

 private:
  static DMA_HandleTypeDef* hdma;
  static AdcChannel mainSupply;
  static AdcChannel dcMotor;

  static AdcBuffer adcBuffer;
};
