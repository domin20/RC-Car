#include "App.h"
#include "usart.h"

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart) { App::getRadioModule().setTxCpltFlag(); }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
  if (htim->Instance == TIM2) {
    App::updateTimeBaseUs();
  }
}
