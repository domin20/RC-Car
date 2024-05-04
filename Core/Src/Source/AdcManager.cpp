#include "AdcManager.h"

#include "Config/StaticConfig.h"

DMA_HandleTypeDef* AdcManager::hdma = nullptr;
AdcChannel AdcManager::mainSupply = {};
AdcChannel AdcManager::dcMotor = {};
AdcBuffer AdcManager::adcBuffer = {};

void AdcManager::init(ADC_HandleTypeDef* hadc, DMA_HandleTypeDef* hdma) {
  HAL_ADC_Start_DMA(hadc, (uint32_t*)&adcBuffer, ADC_CONVERSION_COUNT);
  HAL_DMA_RegisterCallback(hdma, HAL_DMA_CallbackIDTypeDef::HAL_DMA_XFER_CPLT_CB_ID,
                           endOfConversionCallback);
  AdcManager::hdma = hdma;
}

AdcBuffer AdcManager::getBuffer() { return adcBuffer; }

void AdcManager::endOfConversionCallback(DMA_HandleTypeDef* hdma) {
  if (hdma == AdcManager::hdma) {
    calculateAdcChannelValues(&mainSupply, adcBuffer.mainSupplyVoltageRawValue);
    calculateAdcChannelValues(&dcMotor, adcBuffer.dcMotorVoltageRawValue);
  }
}

float AdcManager::getMainSupplyVoltage() {
  return mainSupply.adcValue * MICROCONTROLLER_VOLTAGE * (R1_MAIN_SUPPLY + R2_MAIN_SUPPLY) /
         (MAX_ADC_RAW_VALUE * R2_MAIN_SUPPLY);
}

float AdcManager::getDcMotorVoltage() {
  return dcMotor.adcValue * MICROCONTROLLER_VOLTAGE * (R1_DC_MOTOR + R2_DC_MOTOR) /
         (MAX_ADC_RAW_VALUE * R2_DC_MOTOR);
}

void AdcManager::calculateAdcChannelValues(AdcChannel* channel, uint16_t rawAdcBufferValue) {
  channel->rawValueBuffer[channel->bufferIdx] = rawAdcBufferValue;
  channel->bufferIdx += 1;
  channel->bufferIdx %= RAW_VALUE_BUFFER_SIZE;

  uint32_t sum = 0;
  for (uint8_t i = 0; i < RAW_VALUE_BUFFER_SIZE; i++) {
    sum += channel->rawValueBuffer[i];
  }

  channel->adcValue = sum / RAW_VALUE_BUFFER_SIZE;
}
