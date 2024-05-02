#include "AdcManager.h"

#include "Config/StaticConfig.h"

AdcBuffer AdcManager::buffer = {};

void AdcManager::init(ADC_HandleTypeDef* hadc) {
  HAL_ADC_Start_DMA(hadc, (uint32_t*)&buffer, ADC_CONVERSION_COUNT);
}

AdcBuffer AdcManager::getBuffer() { return buffer; }

float AdcManager::getMainSupplyVoltage() {
  return buffer.mainSupplyVoltageRawValue * MICROCONTROLLER_VOLTAGE *
         (R1_MAIN_SUPPLY + R2_MAIN_SUPPLY) / (MAX_ADC_RAW_VALUE * R2_MAIN_SUPPLY);
}

float AdcManager::getDcMotorVoltage() {
  return buffer.dcMotorVoltageRawValue * MICROCONTROLLER_VOLTAGE * (R1_DC_MOTOR + R2_DC_MOTOR) /
         (MAX_ADC_RAW_VALUE * R2_DC_MOTOR);
}
