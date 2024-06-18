#include "AdcManager.h"
#include "App.h"
#include "usart.h"

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) { App::getRadioModule().onReceivedData(); }

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart) { App::getRadioModule().setTxCpltFlag(); }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
  if (htim->Instance == TIM2) {
    App::updateTimeBaseUs10();
  }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) { AdcManager::endOfConversionCallback(); }
