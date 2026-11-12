/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static void UART_SendText(const char *text)
{
    uint16_t length = 0U;

    while (text[length] != '\0')
    {
        ++length;
    }

    if (length > 0U)
    {
        (void)HAL_UART_Transmit(&huart2, (uint8_t *)text, length, 100U);

    }
}


/* Demo variables can be watched in the CubeIDE debugger. */
volatile uint32_t callback10ms_count = 0U;
volatile uint32_t callback10ms_timestamp_ms = 0U;
volatile uint32_t task500ms_count = 0U;
volatile uint32_t task1000ms_count = 0U;
volatile uint32_t task1500ms_count = 0U;
volatile uint32_t task2000ms_count = 0U;
volatile uint32_t task2500ms_count = 0U;
volatile uint32_t oneShot1_timestamp_ms = 0U;
volatile uint32_t oneShot2_timestamp_ms = 0U;

void callback10ms(void)
{
    callback10ms_count++;
    callback10ms_timestamp_ms = SCH_Get_Time();
}

void Task_500ms(void)
{
    task500ms_count++;
    HAL_GPIO_TogglePin(Led_Test_System_GPIO_Port, Led_Test_System_Pin);
    char buffer[64];

            snprintf(buffer, sizeof(buffer),
                     "Task_500ms at %lu ms\r\n",
                     (unsigned long)SCH_Get_Time());

            UART_SendText(buffer);
}

void Task_1000ms(void)
{
    task1000ms_count++;
    HAL_GPIO_TogglePin(Led_Test_Timer_GPIO_Port, Led_Test_Timer_Pin);
    char buffer[64];

        snprintf(buffer, sizeof(buffer),
                 "Task_10ms done at %lu ms\r\n",
                 (unsigned long)SCH_Get_Time());

        UART_SendText(buffer);
}

void Task_1500ms(void)
{
    task1500ms_count++;
    HAL_GPIO_TogglePin(Led_5_GPIO_Port, Led_5_Pin);
    char buffer[64];

            snprintf(buffer, sizeof(buffer),
                     "Task_1500ms done at %lu ms\r\n",
                     (unsigned long)SCH_Get_Time());

            UART_SendText(buffer);
}

void Task_2000ms(void)
{
    task2000ms_count++;
}

void Task_2500ms(void)
{
    task2500ms_count++;
}

void OneShot_1(void)
{
    oneShot1_timestamp_ms = SCH_Get_Time();
    char buffer[64];

                snprintf(buffer, sizeof(buffer),
                         "Task_OneShot1 done at %lu ms\r\n",
                         (unsigned long)SCH_Get_Time());

                UART_SendText(buffer);
}

void OneShot_2(void)
{
    oneShot2_timestamp_ms = SCH_Get_Time();
}


static const uint8_t SEGMENT_TABLE[10] =
{
    0x3F,  // 0: abcdef
    0x06,  // 1: bc
    0x5B,  // 2: abdeg
    0x4F,  // 3: abcdg
    0x66,  // 4: bcfg
    0x6D,  // 5: acdfg
    0x7D,  // 6: acdefg
    0x07,  // 7: abc
    0x7F,  // 8: abcdefg
    0x6F   // 9: abcdfg
};

static uint8_t display_digits[4] = {0, 0, 0, 0};
static uint8_t current_digit = 0;

void Display_UpdateNumber(uint32_t number)
{
    number %= 10000U;

    display_digits[0] = number / 1000U;
    display_digits[1] = (number / 100U) % 10U;
    display_digits[2] = (number / 10U) % 10U;
    display_digits[3] = number % 10U;
}

void Display_DisableAll(void)
{
    HAL_GPIO_WritePin(GPIOA,
        En0_Pin | En1_Pin | En2_Pin | En3_Pin,
        GPIO_PIN_SET);
}

void Display_SetSegments(uint8_t pattern)
{
    HAL_GPIO_WritePin(GPIOB,
        Led7_a_Pin | Led7_b_Pin | Led7_c_Pin |
        Led7_d_Pin | Led7_e_Pin | Led7_f_Pin |
        Led7_g_Pin,
        GPIO_PIN_RESET);

    HAL_GPIO_WritePin(Led7_a_GPIO_Port, Led7_a_Pin,
        (pattern & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(Led7_b_GPIO_Port, Led7_b_Pin,
        (pattern & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(Led7_c_GPIO_Port, Led7_c_Pin,
        (pattern & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(Led7_d_GPIO_Port, Led7_d_Pin,
        (pattern & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(Led7_e_GPIO_Port, Led7_e_Pin,
        (pattern & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(Led7_f_GPIO_Port, Led7_f_Pin,
        (pattern & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(Led7_g_GPIO_Port, Led7_g_Pin,
        (pattern & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void Display_Scan(void)
{
    Display_DisableAll();

    Display_SetSegments(SEGMENT_TABLE[display_digits[current_digit]]);

    switch (current_digit)
    {
        case 0:
            HAL_GPIO_WritePin(En0_GPIO_Port, En0_Pin, GPIO_PIN_RESET);
            break;

        case 1:
            HAL_GPIO_WritePin(En1_GPIO_Port, En1_Pin, GPIO_PIN_RESET);
            break;

        case 2:
            HAL_GPIO_WritePin(En2_GPIO_Port, En2_Pin, GPIO_PIN_RESET);
            break;

        case 3:
            HAL_GPIO_WritePin(En3_GPIO_Port, En3_Pin, GPIO_PIN_RESET);
            break;
    }

    current_digit++;

    if (current_digit >= 4)
    {
        current_digit = 0;
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
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  SCH_Init();

  /* Add scheduler tasks. TIM2 tick is 10 ms (one scheduler tick). */
  (void)SCH_Add_Task(callback10ms, 0U, 1U);       /* Every 10 ms */
  (void)SCH_Add_Task(Task_500ms, 50U, 50U);       /* Every 500 ms */
  (void)SCH_Add_Task(Task_1000ms, 1U, 1U);    /* Every 1 s */
  (void)SCH_Add_Task(Task_1500ms, 150U, 150U);    /* Every 1.5 s */
  (void)SCH_Add_Task(Task_2000ms, 200U, 200U);    /* Every 2 s */
  (void)SCH_Add_Task(Task_2500ms, 250U, 250U);    /* Every 2.5 s */

  /* One-shot callbacks after 1 s and 2.5 s */
  (void)SCH_Add_Task(OneShot_1, 100U, 0U);
  (void)SCH_Add_Task(OneShot_2, 250U, 0U);

  UART_SendText("Lab4 scheduler started\r\n");

  /* Start the 10 ms TIM2 scheduler tick */
  if (HAL_TIM_Base_Start_IT(&htim2) != HAL_OK)
  {
      Error_Handler();
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    SCH_Dispatch_Tasks();

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, Led_Test_System_Pin|Dot_Pin|Led_5_Pin|En0_Pin
                          |En1_Pin|En2_Pin|En3_Pin|Led_Test_Timer_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, Led7_a_Pin|Led7_b_Pin|Led7_c_Pin|Led7_d_Pin
                          |Led7_e_Pin|Led7_f_Pin|Led7_g_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : Led_Test_System_Pin Dot_Pin Led_5_Pin En0_Pin
                           En1_Pin En2_Pin En3_Pin Led_Test_Timer_Pin */
  GPIO_InitStruct.Pin = Led_Test_System_Pin|Dot_Pin|Led_5_Pin|En0_Pin
                          |En1_Pin|En2_Pin|En3_Pin|Led_Test_Timer_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Led7_a_Pin Led7_b_Pin Led7_c_Pin Led7_d_Pin
                           Led7_e_Pin Led7_f_Pin Led7_g_Pin */
  GPIO_InitStruct.Pin = Led7_a_Pin|Led7_b_Pin|Led7_c_Pin|Led7_d_Pin
                          |Led7_e_Pin|Led7_f_Pin|Led7_g_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
