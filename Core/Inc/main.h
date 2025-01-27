/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TEST_LED_Pin GPIO_PIN_13
#define TEST_LED_GPIO_Port GPIOC
#define USER_BUTTON_Pin GPIO_PIN_0
#define USER_BUTTON_GPIO_Port GPIOA
#define MAIN_SUPPLY_VOLTAGE_Pin GPIO_PIN_3
#define MAIN_SUPPLY_VOLTAGE_GPIO_Port GPIOA
#define DC_ENGINE_VOLTAGE_Pin GPIO_PIN_4
#define DC_ENGINE_VOLTAGE_GPIO_Port GPIOA
#define MOTOR_PWM_Pin GPIO_PIN_7
#define MOTOR_PWM_GPIO_Port GPIOA
#define FORWARD_ENABLE_Pin GPIO_PIN_0
#define FORWARD_ENABLE_GPIO_Port GPIOB
#define REVERSE_ENABLE_Pin GPIO_PIN_1
#define REVERSE_ENABLE_GPIO_Port GPIOB
#define ENCODER_Pin GPIO_PIN_10
#define ENCODER_GPIO_Port GPIOB
#define ENCODER_EXTI_IRQn EXTI15_10_IRQn
#define HC12_SET_Pin GPIO_PIN_11
#define HC12_SET_GPIO_Port GPIOA
#define SERVO_PWM_Pin GPIO_PIN_6
#define SERVO_PWM_GPIO_Port GPIOB
#define LED_RGB_R_Pin GPIO_PIN_7
#define LED_RGB_R_GPIO_Port GPIOB
#define LED_RGB_G_Pin GPIO_PIN_8
#define LED_RGB_G_GPIO_Port GPIOB
#define LED_RGB_B_Pin GPIO_PIN_9
#define LED_RGB_B_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
