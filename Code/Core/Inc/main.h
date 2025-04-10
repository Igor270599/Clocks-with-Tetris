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
#include "stm32f1xx_hal.h"

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
#define JoeStick_Button_Pin GPIO_PIN_3
#define JoeStick_Button_GPIO_Port GPIOA
#define CLK_MAX7219_Pin GPIO_PIN_13
#define CLK_MAX7219_GPIO_Port GPIOB
#define CS_MAX7219_Pin GPIO_PIN_14
#define CS_MAX7219_GPIO_Port GPIOB
#define Din_MAX7219_Pin GPIO_PIN_15
#define Din_MAX7219_GPIO_Port GPIOB
#define LEFT_Button_Pin GPIO_PIN_11
#define LEFT_Button_GPIO_Port GPIOA
#define UP_Button_Pin GPIO_PIN_12
#define UP_Button_GPIO_Port GPIOA
#define RIGHT_Button_Pin GPIO_PIN_15
#define RIGHT_Button_GPIO_Port GPIOA
#define DOWN_Button_Pin GPIO_PIN_3
#define DOWN_Button_GPIO_Port GPIOB
#define MENU_Button_Pin GPIO_PIN_4
#define MENU_Button_GPIO_Port GPIOB
#define MP3_Busy_Pin GPIO_PIN_9
#define MP3_Busy_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
