/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ir.h"
#include "ws2812b.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LAST_MODE		4
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
const uint8_t gamma8[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

bool is_button_play_pressed;
short led_modes[7] = {0};
short brightness = 1;
short led_animation_mode = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static short set_led_mode(short led_mode) {

	led_mode++;

	if(led_mode == LAST_MODE)
	  led_mode = 0;

	return led_mode;
}

static void led_on(short led, short mode, short brightness) {

	if(mode == 1) {

		ws2812b_set_color(led, brightness, 0, 0);
		ws2812b_update();

	}
	else if(mode == 2) {

		ws2812b_set_color(led, 0, brightness, 0);
		ws2812b_update();

	}
	else if(mode == 3) {

		ws2812b_set_color(led, 0, 0, brightness);
		ws2812b_update();

	}
	else {

		ws2812b_set_color(led, 0, 0, 0);
		ws2812b_update();

	}

}

int __io_putchar(int ch) {

  if (ch == '\n') {

    __io_putchar('\r');

  }

  HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);

  return 1;

}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

  if (htim == &htim2) {

    switch (HAL_TIM_GetActiveChannel(&htim2)) {
      case HAL_TIM_ACTIVE_CHANNEL_1:
    	  ir_tim_interrupt();
        break;
      default:
        break;
    }

  }

}

void control_ws2812b(int button_code) {

	  switch (button_code) {
	  case IR_CODE_1:
		led_modes[LED_1] = set_led_mode(led_modes[LED_1]);
		led_on(LED_1, led_modes[LED_1], brightness);
		break;
	  case IR_CODE_2:
		led_modes[LED_2] = set_led_mode(led_modes[LED_2]);
		led_on(LED_2, led_modes[LED_2], brightness);
		break;
	  case IR_CODE_3:
		led_modes[LED_3] = set_led_mode(led_modes[LED_3]);
		led_on(LED_3, led_modes[LED_3], brightness);
	    break;
	  case IR_CODE_4:
		led_modes[LED_4] = set_led_mode(led_modes[LED_4]);
		led_on(LED_4, led_modes[LED_4], brightness);
	    break;
	  case IR_CODE_5:
		led_modes[LED_5] = set_led_mode(led_modes[LED_5]);
		led_on(LED_5, led_modes[LED_5], brightness);
	    break;
	  case IR_CODE_6:
		led_modes[LED_6] = set_led_mode(led_modes[LED_6]);
		led_on(LED_6, led_modes[LED_6], brightness);
	    break;
	  case IR_CODE_7:
		led_modes[LED_7] = set_led_mode(led_modes[LED_7]);
		led_on(LED_7, led_modes[LED_7], brightness);
	    break;
	  case IR_CODE_PLAY:
		if(!is_button_play_pressed)
		  is_button_play_pressed = true;
		else
		  is_button_play_pressed = false;
		break;
	  case IR_CODE_ONOFF:
		is_button_play_pressed = false;
		for(int led = 0; led < 7; led++) {
		  ws2812b_set_color(led, 0, 0, 0);
		  ws2812b_update();
		}
		break;
	  case IR_CODE_PLUS:
		if(brightness >= 0 && brightness < 1)
			brightness += 1;
		else if(brightness >= 1 && brightness < 10)
		  brightness += 9;
		else if(brightness >= 10 && brightness < 90)
		  brightness += 80;
		else if(brightness >= 90 && brightness < 250)
		  brightness += 160;
		break;
	  case IR_CODE_MINUS:
		if(brightness > 0 && brightness <= 1)
		  brightness -= 1;
		else if(brightness > 1 && brightness <= 10)
		  brightness -= 9;
		else if(brightness > 10 && brightness <= 90)
		  brightness -= 80;
		else if(brightness > 110 && brightness <= 250)
		  brightness -= 160;
		break;
	  case IR_CODE_FORWARD:
		if(is_button_play_pressed)
		  if(led_animation_mode < 2)
			led_animation_mode += 1;
		break;
	  case IR_CODE_REWIND:
		if(is_button_play_pressed)
		  if(led_animation_mode > 0)
			led_animation_mode -= 1;
		break;
	  }

}

void led_animation() {

	if(is_button_play_pressed) {

		uint32_t delay_time;

		if(led_animation_mode == 0) {


			uint8_t r = gamma8[rand() % 256];
			uint8_t g = gamma8[rand() % 256];
			uint8_t b = gamma8[rand() % 256];

			for(int led = 0; led < 7; led++) {

				delay_time = HAL_GetTick();
				ws2812b_set_color(led, r, g, b);
				ws2812b_update();
				//HAL_Delay(100);
				while(HAL_GetTick() - delay_time <= 100){}

			}

		}
		else if(led_animation_mode == 1) {

			for(int led = 0; led < 7; led++) {

				delay_time = HAL_GetTick();

				uint8_t r = gamma8[rand() % 256];
				uint8_t g = gamma8[rand() % 256];
				uint8_t b = gamma8[rand() % 256];

				ws2812b_set_color(led, r, g, b);
				ws2812b_update();
				while(HAL_GetTick() - delay_time < 100){}

			}

		}
		else if(led_animation_mode == 2) {

			uint8_t r1;
			uint8_t g1;
			uint8_t b1;
			uint8_t r2;
			uint8_t g2;
			uint8_t b2;
			uint8_t last_led = 6;
			for(int led = 0; led < 7; led++) {

				delay_time = HAL_GetTick();

				r1 = gamma8[rand() % 256];
				g1 = gamma8[rand() % 256];
				b1 = gamma8[rand() % 256];
				r2 = gamma8[rand() % 256];
				g2 = gamma8[rand() % 256];
				b2 = gamma8[rand() % 256];

				ws2812b_set_color(led, r1, g1, b1);
				ws2812b_update();
				while(HAL_GetTick() - delay_time < 150){}
				ws2812b_set_color(led, 0, 0, 0);
				ws2812b_update();
				ws2812b_set_color(last_led, r2, g2, b2);
				ws2812b_update();

			}

		}

	}


}

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
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  ir_init();
  ws2812b_init();


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  int value = ir_read();
	  control_ws2812b(value);
	  led_animation();




    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/* USER CODE BEGIN 4 */

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

#ifdef  USE_FULL_ASSERT
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
