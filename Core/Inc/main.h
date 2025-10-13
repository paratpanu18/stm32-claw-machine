/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f7xx_hal.h"

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
#define MotorY2_Pin GPIO_PIN_3
#define MotorY2_GPIO_Port GPIOF
#define MotorZ1_Pin GPIO_PIN_5
#define MotorZ1_GPIO_Port GPIOF
#define SCK_Pin GPIO_PIN_7
#define SCK_GPIO_Port GPIOF
#define SDO_Pin GPIO_PIN_8
#define SDO_GPIO_Port GPIOF
#define SDI_Pin GPIO_PIN_9
#define SDI_GPIO_Port GPIOF
#define MotorZ2_Pin GPIO_PIN_10
#define MotorZ2_GPIO_Port GPIOF
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOH
#define MotorX2_Pin GPIO_PIN_0
#define MotorX2_GPIO_Port GPIOC
#define MotorY1_Pin GPIO_PIN_3
#define MotorY1_GPIO_Port GPIOC
#define MotorX1_Pin GPIO_PIN_3
#define MotorX1_GPIO_Port GPIOA
#define Btn_Pin GPIO_PIN_5
#define Btn_GPIO_Port GPIOA
#define Claw_Pin GPIO_PIN_7
#define Claw_GPIO_Port GPIOA
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define JoystickUp_Pin GPIO_PIN_13
#define JoystickUp_GPIO_Port GPIOF
#define JoystickRight_Pin GPIO_PIN_14
#define JoystickRight_GPIO_Port GPIOF
#define JoystickDown_Pin GPIO_PIN_9
#define JoystickDown_GPIO_Port GPIOE
#define JoystickLeft_Pin GPIO_PIN_11
#define JoystickLeft_GPIO_Port GPIOE
#define Buzzer_Pin GPIO_PIN_11
#define Buzzer_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define STLK_RX_Pin GPIO_PIN_8
#define STLK_RX_GPIO_Port GPIOD
#define STLK_TX_Pin GPIO_PIN_9
#define STLK_TX_GPIO_Port GPIOD
#define CS_Pin GPIO_PIN_8
#define CS_GPIO_Port GPIOC
#define DC_Pin GPIO_PIN_9
#define DC_GPIO_Port GPIOC
#define RESET_Pin GPIO_PIN_10
#define RESET_GPIO_Port GPIOC
#define LD2_Pin GPIO_PIN_7
#define LD2_GPIO_Port GPIOB
#define Coin_Pin GPIO_PIN_8
#define Coin_GPIO_Port GPIOB
#define Coin_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
