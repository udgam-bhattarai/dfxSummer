



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
#include "math.h"
#include <stdbool.h>
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
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;
/* USER CODE BEGIN PV */
/* USER CODE END PV */
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */
/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t X_size[2];

volatile bool data_ready = false;


float Weights[] = {6.279351664958193524e+00,
		-7.035501848239908984e-01,
		-1.065726312446290569e+00,
		-7.588902593505301997e-02,
		2.030713190912808996e-01,
		4.131207085809989188e-01,
		-8.371054147988320204e-02,
		1.369555102658815526e+00};
float Bias = -3.0625375469792884;
float dotProduct(float* a, float* b, uint32_t size);
void regressionPredict(float* buffer, uint32_t* Y, uint32_t rows, uint32_t cols);
void rxbufferUnpack(float* buffer, float** X, uint32_t rows, uint32_t cols);
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
 MX_USART2_UART_Init();
 /* USER CODE BEGIN 2 */
 /* USER CODE END 2 */
 /* Infinite loop */
 /* USER CODE BEGIN WHILE */
 while (1)
 {
   /* USER CODE END WHILE */
   /* USER CODE BEGIN 3 */
	 if (data_ready){
		 uint32_t rows = X_size[0];
		 uint32_t cols = X_size[1];

		 float rx_buffer[rows * cols];
		 float  X_test[rows][cols];
		 uint32_t Y_test[rows];

		 HAL_UART_Receive(&huart2, rx_buffer, sizeof(rx_buffer), HAL_MAX_DELAY);
		 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

		 //rxbufferUnpack(rx_buffer, X_test, rows, cols);
		 regressionPredict(rx_buffer, Y_test, rows, cols);
		 HAL_UART_Transmit(&huart2, Y_test, (rows*4), HAL_MAX_DELAY);
		 data_ready = false;
	 }
	 else{
		 HAL_UART_Receive_DMA(&huart2, X_size, 8);
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
 RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
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
 PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
 PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
 if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
 {
   Error_Handler();
 }
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
 huart2.Init.BaudRate = 9600;
 huart2.Init.WordLength = UART_WORDLENGTH_8B;
 huart2.Init.StopBits = UART_STOPBITS_1;
 huart2.Init.Parity = UART_PARITY_NONE;
 huart2.Init.Mode = UART_MODE_TX_RX;
 huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
 huart2.Init.OverSampling = UART_OVERSAMPLING_16;
 huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
 huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
 if (HAL_UART_Init(&huart2) != HAL_OK)
 {
   Error_Handler();
 }
 /* USER CODE BEGIN USART2_Init 2 */
 /* USER CODE END USART2_Init 2 */
}
/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void)
{
 /* DMA controller clock enable */
 __HAL_RCC_DMA1_CLK_ENABLE();
 /* DMA interrupt init */
 /* DMA1_Channel6_IRQn interrupt configuration */
 HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
 HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
 /* DMA1_Channel7_IRQn interrupt configuration */
 HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);
 HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);
}
/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
 GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */
 /* GPIO Ports Clock Enable */
 __HAL_RCC_GPIOA_CLK_ENABLE();
 /*Configure GPIO pin Output Level */
 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
 /*Configure GPIO pin : PA5 */
 GPIO_InitStruct.Pin = GPIO_PIN_5;
 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
 HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}
/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (huart->Instance == USART2)
	  {
		data_ready = true;
	  }
}
void regressionPredict(float* buffer, uint32_t* Y, uint32_t rows, uint32_t cols) {
    for (uint32_t i = 0; i < rows; i++) {
        // Calculate the dot product of features and weights
        float features[cols];
        for (uint32_t j = 0; j < cols; j++) {
            features[j] = buffer[i * cols + j];
        }
        double dot_product = dotProduct(features, Weights, cols);

        // Calculate the logistic regression output
        double Z = 1 / (1 + exp(-(dot_product + Bias)));

        // Store the prediction result in the output array
        if (Z < 0.5) {
            Y[i] = 0;
        } else {
            Y[i] = 1;
        }
    }
}


float dotProduct(float* a, float* b, uint32_t size) {
    float result = 0.0;
    for (uint32_t i = 0; i < size; i++) {
        result += a[i] * b[i];
    }
    return result;
}


void rxbufferUnpack(float* buffer, float** X, uint32_t rows, uint32_t cols) {
    int index = 0;
    for (uint32_t i = 0; i < rows; i++) {
        for (uint32_t j = 0; j < cols; j++) {
            X[i][j] = buffer[index];
            index++;
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
