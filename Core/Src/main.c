/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "state.h"
#include "ili9341.h"
#include "ili9341_fonts.h"
#include "LCD_pages.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
const uint8_t TARGET_COIN_AMOUNT = 10;
const uint8_t COIN_VALUE = 5;
const uint8_t WAIT_COIN_TIME_LIMIT = 10;
const uint8_t WAIT_GAME_TIME_STATE = 30;
uint8_t currentCoinAmount = 0;
uint8_t STATE = IDLE;


static float posX = 0;
static float posY = 0;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/*
 === Joystick ===
 Right - Green
 Up - Yellow
 Left - Red
 Down - Blue

*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void transmitStringUART(const char* format, ...);

void motorStop() {
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, 0);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_15, 0);
}

void home() {
	posX *= 1.0;
	posY *= 1.0;
	while (posX > 0) {
		  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, 1);
		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_15, 0);
		  posX -= 1;
		  transmitStringUART("Position (%.1f, %.1f)\r\n", posX, posY);
	}
	 HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, 0);
	 HAL_GPIO_WritePin(GPIOF, GPIO_PIN_15, 0);

	while (posY > 0) {
		  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 1);
		  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, 0);
		  posY -= 1;
		  transmitStringUART("Position (%.1f, %.1f)\r\n", posX, posY);
	}
	  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 0);
	  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, 0);

	  posX = 0.0f;
	  posY = 0.0f;
}



/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
ILI9341_HandleTypeDef ili9341;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_TIM2_Init();
  MX_SPI5_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  ili9341 = ILI9341_Init(
  	&hspi5,
  	CS_GPIO_Port,
  	CS_Pin,
  	DC_GPIO_Port,
  	DC_Pin,
  	RESET_GPIO_Port,
  	RESET_Pin,
  	ILI9341_ROTATION_HORIZONTAL_2,
  	320,
  	240
  );
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  transmitStringUART("Program started. Current State: %s\r\n", stateNames[STATE]);
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//	  ILI9341_FillScreen(&ili9341, ILI9341_COLOR_RED);



//	  transmitStringUART("Button: %d\r\n", );

	  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_13) == 0) {
		  if (STATE == WAIT_CONFIRM) {
			  STATE = GAME;
			  transmitStringUART("State changed to %s\r\n", stateNames[STATE]);
		  }
		  else if (STATE == GAME) {
			  STATE = DEPOSIT;
			  transmitStringUART("State changed to %s\r\n", stateNames[STATE]);
		  }
	  }


	  renderPage(STATE);

	  if (STATE == IDLE) {
		  if (currentCoinAmount != 0) {

			  HAL_TIM_Base_Start_IT(&htim2);

			  STATE = WAIT_COIN;
			  transmitStringUART("State changed to %s\r\n", stateNames[STATE]);

		  }
	  }
	  else if (STATE == WAIT_COIN) {
		  if (currentCoinAmount >= TARGET_COIN_AMOUNT) {

			  HAL_TIM_Base_Stop_IT(&htim2);
			  currentCoinAmount = 0;
			  __HAL_TIM_SET_COUNTER(&htim2, 0);

			  STATE = GAME;
			  transmitStringUART("State changed to %s\r\n", stateNames[STATE]);

		  }
	  }
	  else if (STATE == GAME) {
		  HAL_TIM_Base_Start_IT(&htim2);
		  const uint8_t joystickUp = HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_13);
		  const uint8_t joystickDown = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9);
		  const uint8_t joystickLeft = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_11);
		  const uint8_t joystickRight = HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_14);

	//	  transmitStringUART("Up = %d | Down = %d | Left = %d | Right = %d \r\n", joystickUp, joystickDown, joystickLeft, joystickRight);


		  if (!joystickLeft) {
			  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, 0);
			  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_15, 1);
			  posX += 1;
			  transmitStringUART("Position (%.1f, %.1f)\r\n", posX, posY);
		  }
		  else if (!joystickRight) {
			  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, 1);
			  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_15, 0);
			  posX -= 1;
			  transmitStringUART("Position (%.1f, %.1f)\r\n", posX, posY);
		  }
		  else {
			  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, 0);
			  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_15, 0);
		  }

		  if (!joystickUp) {
			  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 1);
			  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, 0);
			  posY -= 1;
			  transmitStringUART("Position (%.1f, %.1f)\r\n", posX, posY);
		  }
		  else if (!joystickDown) {
			  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 0);
			  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, 1);
			  posY += 1;
			  transmitStringUART("Position (%.1f, %.1f)\r\n", posX, posY);
		  }
		  else {
			  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 0);
			  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, 0);
		  }
//		  transmitStringUART("Position (%d, %d)\r\n", posX, posY);
		  continue;
	  }
	  else if (STATE == DEPOSIT){
		  transmitStringUART("Hello from Deposit state: STOP`");

		  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, 0);
		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_15, 0);
		  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 0);
		  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, 0);


		  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);

		  HAL_Delay(4500);

		  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);

		  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);

		  HAL_Delay(6500);

		  //Positioning
		  home();

		  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);


		  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);
		  STATE = IDLE;
		  transmitStringUART("State changed to %s\r\n", stateNames[STATE]);
	  }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void transmitStringUART(const char* format, ...){
	char buf[128];

	va_list args;
	va_start(args, format);
	vsnprintf(buf, sizeof(buf), format, args);
	va_end(args);

	HAL_UART_Transmit(&huart3, (uint8_t*)buf, strlen(buf), 50);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	transmitStringUART("GPIO: %d\r\n", GPIO_Pin);
	if (GPIO_Pin == GPIO_PIN_8)
	{
		transmitStringUART("It's fucking interupt blahblah\r\n");
		if (STATE == IDLE || STATE == WAIT_COIN){
			currentCoinAmount += COIN_VALUE;
			transmitStringUART("Coin Detected | Current Coin Amount: %d\r\n", currentCoinAmount);
		}
	}
	else if (GPIO_Pin == GPIO_PIN_5)
	{
		transmitStringUART("It's fucking interupt\r\n");
		if (STATE == WAIT_CONFIRM){
			transmitStringUART("Print from here\r\n");
			STATE = GAME;
			transmitStringUART("State changed to %s\r\n", stateNames[STATE]);
		}

//		if (STATE == GAME){
//			STATE = DEPOSIT;
//			transmitStringUART("State changed to %s from EXTI_9\r\n", stateNames[STATE]);
//		}
	}
	else if (GPIO_Pin == GPIO_PIN_13) {
		transmitStringUART("Interupt from EXTI13\r\n");
	}
//	else if (GPIO_Pin == GPIO_PIN_10){
//		if (STATE == GAME){
//			STATE = DEPOSIT;
//			transmitStringUART("State changed to %s from EXTI_10\r\n", stateNames[STATE]);
//		}
//	}
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	static int timeCount = 0;
	if (htim -> Instance == TIM2){
		if (STATE == WAIT_COIN) {
			timeCount++;
			timeCount%=11;
			if (timeCount >= WAIT_COIN_TIME_LIMIT) {
				currentCoinAmount = 0;
				STATE = IDLE;
				transmitStringUART("WAIT_COIN exceeded %d seconds | State changed to %s\r\n", WAIT_COIN_TIME_LIMIT, stateNames[STATE]);
			}
		}


		if (STATE == GAME) {
			timeCount++;
			timeCount%=31;
			if(timeCount >= WAIT_GAME_TIME_STATE){
				STATE = DEPOSIT;
				transmitStringUART("State changed to %s\r\n", stateNames[STATE]);
			}

		}

	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
