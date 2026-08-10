
#include "bsp_hardware.h"
#include <stdio.h>
#include <string.h>

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA

ADC_HandleTypeDef hadc1;
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;
SPI_HandleTypeDef hspi1;
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;
//RTC_HandleTypeDef hrtc;
#if defined(STM32F429xx)
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart3_rx;
#endif
#if defined(STM32F401xE)
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
#endif
UART_HandleTypeDef huart1;
static bsp_usart_rx_t uart1_rx;
DMA_HandleTypeDef hdma_usart1_rx;


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
#if defined(STM32F401xE)
static void MX_USART2_UART_Init(void);
#endif
#if defined(STM32F429xx)
static void MX_USART3_UART_Init(void);
#endif
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(uint32_t Period);
#if defined(HAL_SPI_MODULE_ENABLED)
void MX_SPI1_Init(int mode);
#endif

static bsp_tim_func_t bsp_tim;
static bsp_irq_func_t bsp_irq;
#if defined(STM32F401xE)
static bsp_usart_rx_t uart2_rx;
#endif
#if defined(STM32F429xx)
static bsp_usart_rx_t uart3_rx;
#endif
static bsp_key_func_t bsp_key;

static evb_port_t bsp_port = {INTERFACE_USER_SEL, INTERFACE_USER_SEL, INTERFACE_USER_SEL, EVB_SPI_MODE0};
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
#if 0
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
#else
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV8;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
#endif
}


/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}


/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
void MX_I2C1_Init(uint32_t clk)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = clk;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

void MX_I2C1_DeInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	HAL_I2C_DeInit(&hi2c1);
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed= GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
}

void MX_I2C2_Init(uint32_t clk)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = clk;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

void MX_I2C2_DeInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	HAL_I2C_DeInit(&hi2c2);
	GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed= GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
}


/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
#if 0
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}
#endif

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
void MX_SPI1_Init(int mode)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	if(mode == 3)
	{
		hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
		hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
	}
	else
	{
		hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
		hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	}
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{
#if 0

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = (SystemCoreClock/10000)-1;		//72*100-1	
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = Period*10 - 1;		// max 65535
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
#else
	  /* USER CODE BEGIN TIM1_Init 0 */
	
	  /* USER CODE END TIM1_Init 0 */
	
	  TIM_MasterConfigTypeDef sMasterConfig = {0};
	  TIM_OC_InitTypeDef sConfigOC = {0};
	  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};
	
	  /* USER CODE BEGIN TIM1_Init 1 */
	
	  /* USER CODE END TIM1_Init 1 */
	  htim1.Instance = TIM1;
	  htim1.Init.Prescaler = 719;
	  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim1.Init.Period = 999;	// 100Hz
	  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  htim1.Init.RepetitionCounter = 0;
	  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
	  {
		Error_Handler();
	  }
	  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
	  {
		Error_Handler();
	  }
	  sConfigOC.OCMode = TIM_OCMODE_PWM1;
	  sConfigOC.Pulse = 200;
	  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	  {
		Error_Handler();
	  }
	  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	  sBreakDeadTimeConfig.DeadTime = 0;
	  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
	  {
		Error_Handler();
	  }
	  /* USER CODE BEGIN TIM1_Init 2 */
	
	  /* USER CODE END TIM1_Init 2 */
	  HAL_TIM_MspPostInit(&htim1);
#endif
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(uint32_t Period)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = (SystemCoreClock/10000)-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = (10*Period)-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
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
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(uint32_t Period)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = (SystemCoreClock/10000) - 1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = (10*Period)-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}


#if defined(STM32F401xE)
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

  /* DMA1 clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* Configure DMA for USART2 RX: DMA1 Stream5, Channel4 */
  hdma_usart2_rx.Instance = DMA1_Stream5;
  hdma_usart2_rx.Init.Channel = DMA_CHANNEL_4;
  hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_usart2_rx.Init.Mode = DMA_NORMAL;
  hdma_usart2_rx.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_usart2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
  {
    Error_Handler();
  }

  /* Link DMA to UART handle */
  __HAL_LINKDMA(&huart2, hdmarx, hdma_usart2_rx);

  /* DMA RX interrupt */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

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

void MX_USART2_UART_DeInit(void)
{
	HAL_NVIC_DisableIRQ(USART2_IRQn);
	HAL_NVIC_DisableIRQ(DMA1_Stream5_IRQn);
	HAL_UART_DeInit(&huart2);
}
#endif

#if defined(STM32F429xx)
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  HAL_UART_DeInit(&huart3);

  /* DMA1 clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* Configure DMA for USART3 RX: DMA1 Stream1, Channel4 */
  hdma_usart3_rx.Instance = DMA1_Stream1;
  hdma_usart3_rx.Init.Channel = DMA_CHANNEL_4;
  hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_usart3_rx.Init.Mode = DMA_NORMAL;
  hdma_usart3_rx.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_usart3_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  if (HAL_DMA_Init(&hdma_usart3_rx) != HAL_OK)
  {
    Error_Handler();
  }

  /* Link DMA to UART handle */
  __HAL_LINKDMA(&huart3, hdmarx, hdma_usart3_rx);

  /* DMA RX interrupt */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;	//921600 115200
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */
	HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
	HAL_NVIC_DisableIRQ(USART3_IRQn);
}

void MX_USART3_UART_DeInit(void)
{
	HAL_NVIC_DisableIRQ(USART3_IRQn);
	HAL_NVIC_DisableIRQ(DMA1_Stream1_IRQn);
	HAL_UART_DeInit(&huart3);
}
#endif

static void MX_USART1_UART_Init(void)
{
  /* DMA1 clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* Configure DMA for USART2 RX: DMA1 Stream5, Channel4 */
  hdma_usart1_rx.Instance = DMA1_Channel1;
  //hdma_usart1_rx.Init.Channel = DMA1_Channel1;
  hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_usart1_rx.Init.Mode = DMA_NORMAL;
  hdma_usart1_rx.Init.Priority = DMA_PRIORITY_HIGH;
  //hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
  {
    Error_Handler();
  }

  /* Link DMA to UART handle */
  __HAL_LINKDMA(&huart1, hdmarx, hdma_usart1_rx);

  /* DMA RX interrupt */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);


  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  //GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  //__HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin : B1_Pin */
//  GPIO_InitStruct.Pin = GPIO_PIN_13;
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
//  GPIO_InitStruct.Pull = GPIO_PULLUP;
//  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

static void MX_IRQ_Init(void)
{	
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI1_IRQn, 1, 2);
	HAL_NVIC_DisableIRQ(EXTI1_IRQn);
	// KEY INT
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 2);
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
}


void evb_tim_handle(void)
{
	if(bsp_tim.tim1_flag)
	{
		bsp_tim.tim1_flag = 0;
		if(bsp_tim.tim1_func)
			bsp_tim.tim1_func();
	}
	
	if(bsp_tim.tim2_flag)
	{
		bsp_tim.tim2_flag = 0;
		if(bsp_tim.tim2_func)
			bsp_tim.tim2_func();
	}
	
	if(bsp_tim.tim3_flag)
	{
		bsp_tim.tim3_flag = 0;
		if(bsp_tim.tim3_func)
			bsp_tim.tim3_func();
	}
	if(bsp_tim.tim4_flag)
	{
		bsp_tim.tim4_flag = 0;
		if(bsp_tim.tim4_func)
			bsp_tim.tim4_func();
	}
	if(bsp_tim.tim5_flag)
	{
		bsp_tim.tim5_flag = 0;
		if(bsp_tim.tim5_func)
			bsp_tim.tim5_func();
	}
}

void evb_setup_timer(TIM_TypeDef * tim_id, int_callback func, uint16_t ms, FunctionalState enable)
{
	TIM_HandleTypeDef *tim_ptr = NULL;

	if(tim_id == TIM1)
	{
//		tim_ptr = &htim1;
//		bsp_tim.tim1_flag = 0;
//		bsp_tim.tim1_func = func;
//		MX_TIM1_Init();
		if(enable)
			HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
		else
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
		return;
	}
	else if(tim_id == TIM2)
	{
		tim_ptr = &htim2;
		bsp_tim.tim2_flag = 0;
		bsp_tim.tim2_func = func;
		MX_TIM2_Init(ms);
	}
	else if(tim_id == TIM3)
	{
		tim_ptr = &htim3;
		bsp_tim.tim3_flag = 0;
		bsp_tim.tim3_func = func;
		MX_TIM3_Init(ms);
	}

	if(tim_ptr)
	{
		if(enable != DISABLE)
		{
			HAL_TIM_Base_Start_IT(tim_ptr);
		}
		else
		{
			HAL_TIM_Base_Stop_IT(tim_ptr);
		}
	}
}

void evb_set_pwm_duty(TIM_TypeDef * tim_id, uint16_t duty)
{
	if(tim_id == TIM1)
	{
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, duty);
	}
}


void evb_setup_irq(int int_type, int_callback func, FunctionalState enable)
{
//	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if(int_type == 1)
	{
//		__HAL_RCC_GPIOB_CLK_ENABLE();
//		/*Configure GPIO pins*/
//		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//		GPIO_InitStruct.Pin = GPIO_PIN_1;
//		GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
//		GPIO_InitStruct.Pull = GPIO_PULLUP;
//		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//		bsp_irq.irq1_func = func;
//		
//		if(enable)
//			HAL_NVIC_EnableIRQ(EXTI1_IRQn);
//		else
//			HAL_NVIC_DisableIRQ(EXTI1_IRQn);
	}
}

extern void evb_irq_handle(void)
{
//	if(bsp_irq.debounce > 0)
//	{
//		bsp_irq.debounce--;
//	}
	if(bsp_irq.irq1_flag)
	{
		bsp_irq.irq1_flag = 0;
//		bsp_irq.debounce = SystemCoreClock/50;
		if(bsp_irq.irq1_func)
			bsp_irq.irq1_func();
	}
	if(bsp_irq.irq2_flag)
	{
		bsp_irq.irq2_flag = 0;
		if(bsp_irq.irq2_func)
			bsp_irq.irq2_func();
	}
}


void qst_delay_ms(unsigned int delay)
{
	//HAL_ResumeTick();
	HAL_Delay(delay);
	//HAL_SuspendTick();
}

void qst_delay_us(unsigned int delay)
{	
	while(delay--)
	{
		__NOP(); __NOP(); __NOP(); __NOP();
		__NOP(); __NOP(); __NOP(); __NOP();
		__NOP(); __NOP(); __NOP(); __NOP();
		__NOP(); __NOP(); __NOP(); __NOP();
		
		__NOP(); __NOP(); __NOP(); __NOP();
		__NOP(); __NOP(); __NOP(); __NOP();
		__NOP(); __NOP(); __NOP(); __NOP();
		__NOP(); __NOP(); __NOP(); __NOP();
	}
}

void SysTick_Enable(unsigned char enable)
{
	if(enable)
		HAL_ResumeTick();
	else
		HAL_SuspendTick();
}

void bsp_event_clear(void)
{
	evb_setup_irq(QST_EVB_INT1, NULL, DISABLE);
	evb_setup_irq(QST_EVB_INT2, NULL, DISABLE);
	//evb_setup_user_key1(NULL, DISABLE, 0);

	// HAL_TIM_Base_Stop_IT(&htim1);
	// HAL_TIM_Base_Stop_IT(&htim2);
	// HAL_TIM_Base_Stop_IT(&htim3);
	// HAL_TIM_Base_Stop_IT(&htim4);
	// HAL_TIM_Base_Stop_IT(&htim5);
}

void bsp_power_pin_set(int on)
{
	if(on)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
	}
	qst_delay_ms(200);
}

void bsp_led_toggle(void)
{
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
}

void bsp_port_i2c_init(evb_interface_e type)
{
	if((bsp_port.i2c_type < INTERFACE_I2C_SW) || (bsp_port.i2c_type > INTERFACE_I2C_HW_1M))
	{
		bsp_port.i2c_type = type;
		if(type == INTERFACE_I2C_SW)
		{
			qst_logi("\r\nuser select I2C-SW\r\n");
			i2c_sw_gpio_config(0);
		}
		else if(type == INTERFACE_I2C_HW)
		{
			qst_logi("\r\nuser select I2C-HW400K\r\n");
			MX_I2C1_Init(400*1000);
			MX_I2C2_Init(400*1000);
		}
		else if(type == INTERFACE_I2C_HW_1M)
		{
			qst_logi("\r\nuser select I2C-HW1000K\r\n");
			MX_I2C1_Init(800*1000);
			MX_I2C2_Init(800*1000);
		}
		else
		{
			bsp_port.i2c_type = INTERFACE_USER_SEL;
			qst_logi("\r\nI2C init error!\r\n");
			return;
		}
	}
}

void bsp_port_i2c_deinit(void)
{
	HAL_I2C_DeInit(&hi2c1);
	HAL_I2C_DeInit(&hi2c2);
	bsp_port.i2c_type = INTERFACE_USER_SEL;
}

void bsp_port_i3c_init(evb_interface_e type)
{
#ifdef HAL_I3C_MODULE_ENABLED
	int ret = 0;

	if((bsp_port.i3c_type < INTERFACE_I3C_4M) || (bsp_port.i3c_type > INTERFACE_I3C_12_5M))
	{	
I3C_INIT:
		MX_I3C1_Init(0x1e, 0x1e);	// 0x13
		qst_delay_ms(100);
		HAL_I3C_DeInit(&hi3c1);
		qst_delay_ms(100);
		bsp_port.i3c_type = type;
		if(type == INTERFACE_I3C_4M)
		{
			qst_logi("\r\nuser select I3C-4.0M\r\n");
			MX_I3C1_Init(0x1e, 0x1e);
		}
		else if(type == INTERFACE_I3C_6_25M)
		{
			qst_logi("\r\nuser select I3C-6.25M\r\n");
			MX_I3C1_Init(0x13, 0x13);
		}
		else if(type == INTERFACE_I3C_10M)
		{
			qst_logi("\r\nuser select I3C-10.0M\r\n");
			MX_I3C1_Init(0x0c, 0x0c);
		}
		else if(type == INTERFACE_I3C_12_5M)
		{
			qst_logi("\r\nuser select I3C-12.5M\r\n");
	//		MX_I3C1_Init(0x09, 0x0b);	// FAIL
	//		MX_I3C1_Init(0x0a, 0x0b);	// OK
	//		MX_I3C1_Init(0x0b, 0x0b);	// OK
	//		MX_I3C1_Init(0x0c, 0x0c);	// OK
	//		MX_I3C1_Init(0x0d, 0x0d);	// OK
	//		MX_I3C1_Init(0x0e, 0x0e);	// OK
			MX_I3C1_Init(0x09, 0x09);	// 12.5M
		}
		else
		{
			bsp_port.i3c_type = INTERFACE_USER_SEL;
			qst_logi("\r\nI3C init error!\r\n");
			return;
		}
		qst_delay_ms(100);
		ret = qst_evb_enry_i3c();
		if(ret == 0)
			goto I3C_INIT;
	}
//	qst_delay_ms(300);
#endif
}

void bsp_port_i3c_deinit(void)
{
#ifdef HAL_I3C_MODULE_ENABLED
	HAL_I3C_DeInit(&hi3c1);
	bsp_port.i3c_type = INTERFACE_USER_SEL;
#endif
}

void bsp_port_spi_init(evb_interface_e type, int mode)
{
#ifdef HAL_SPI_MODULE_ENABLED
	if((bsp_port.spi_type < INTERFACE_SPI_HW4) || (bsp_port.spi_type > INTERFACE_SPI_SW3))
	{
		bsp_port.spi_type = type;
		bsp_port.spi_mode = (evb_spi_mode_e)mode;

		if(type == INTERFACE_SPI_HW4)
		{
			qst_logi("\r\nuser select HW-4WIRE SPI mode[%d]\r\n", mode);
			MX_SPI1_Init(mode);
		}
		else if(type == INTERFACE_SPI_HW3)
		{
			qst_loge("bsp_port_init EVB_INTERFACE_SPI_HW_3 not support!!!\r\n");
		}
		else if(type == INTERFACE_SPI_SW4)
		{
			qst_logi("\r\nuser select SW-4WIRE SPI mode[%d]\r\n", mode);
			spi_sw_init(4, mode);
		}
		else if(type == INTERFACE_SPI_SW3)
		{
			qst_logi("\r\nuser select SW-3WIRE SPI mode[%d]\r\n", mode);
			spi_sw_init(3, mode);
		}
		else
		{
			bsp_port.spi_type = INTERFACE_USER_SEL;
			qst_loge("bsp_port_spi_init no function!!!\r\n");
		}
	}
#endif
}

void bsp_port_spi_deinit(evb_interface_e type)
{
#ifdef HAL_SPI_MODULE_ENABLED
	if(type == INTERFACE_SPI_HW4)
	{
		qst_logi("HW-4WIRE SPI deinit\r\n");
	}
#endif
}

int bsp_i2c_write_reg(unsigned char slave, unsigned char reg, unsigned char value)
{
	int ret = 0;

	if(bsp_port.i2c_type == INTERFACE_I2C_SW)
	{
		ret = qst_sw_writereg(slave<<1, reg, value);
	}
	else if((bsp_port.i2c_type == INTERFACE_I2C_HW)||(bsp_port.i2c_type == INTERFACE_I2C_HW_1M))
	{
		ret = bsp_i2c1_write(slave<<1, reg, value);
	}
	else
	{
		qst_loge("bsp_i2c_write_reg no function!!!\r\n");
	}

	return ret;
}

int bsp_i2c_write_regs(unsigned char slave, unsigned char reg, unsigned char *values, unsigned short len)
{
	int ret = 0;

	if(bsp_port.i2c_type == INTERFACE_I2C_SW)
	{
		ret = qst_sw_writeregs(slave<<1, reg, values, len);
	}
	else if((bsp_port.i2c_type == INTERFACE_I2C_HW)||(bsp_port.i2c_type == INTERFACE_I2C_HW_1M))
	{
		ret = bsp_i2c1_writes(slave<<1, reg, values, len);
	}
	else
	{
		qst_loge("bsp_i2c_write_reg no function!!!\r\n");
	}

	return ret;
}


int bsp_i2c_read_reg(unsigned char slave, unsigned char reg, uint8_t* buff, unsigned short len)
{
	int ret = 0;

	//qst_logi("interface : %d--%d	", bsp_port.interface, bsp_port.spi_mode);
	if(bsp_port.i2c_type == INTERFACE_I2C_SW)
	{
		ret = qst_sw_readreg(slave<<1, reg, buff, len);
	}
	else if((bsp_port.i2c_type == INTERFACE_I2C_HW)||(bsp_port.i2c_type == INTERFACE_I2C_HW_1M))
	{
		ret = bsp_i2c1_read(slave<<1, reg, buff, len);
	}
	else
	{
		qst_loge("bsp_i2c_read_reg no function!!!\r\n");
	}

	return ret;
}

int bsp_i2c2_write_reg(unsigned char slave, unsigned char reg, unsigned char value)
{
	int ret = 0;

	if(bsp_port.i2c_type == INTERFACE_I2C_SW)
	{
		//ret = qst_sw_writereg(slave<<1, reg, value);
	}
	else if((bsp_port.i2c_type == INTERFACE_I2C_HW)||(bsp_port.i2c_type == INTERFACE_I2C_HW_1M))
	{
		ret = bsp_i2c2_write(slave<<1, reg, value);
	}
	else
	{
		qst_loge("bsp_i2c2_write_reg no function!!!\r\n");
	}

	return ret;
}

int bsp_i2c2_write_regs(unsigned char slave, unsigned char reg, unsigned char *values, unsigned short len)
{
	int ret = 0;

	if(bsp_port.i2c_type == INTERFACE_I2C_SW)
	{
		// ret = qst_sw_writeregs(slave<<1, reg, values, len);
	}
	else if((bsp_port.i2c_type == INTERFACE_I2C_HW)||(bsp_port.i2c_type == INTERFACE_I2C_HW_1M))
	{
		ret = bsp_i2c2_writes(slave<<1, reg, values, len);
	}
	else
	{
		qst_loge("bsp_i2c2_write_regs no function!!!\r\n");
	}

	return ret;
}


int bsp_i2c2_read_reg(unsigned char slave, unsigned char reg, uint8_t* buff, unsigned short len)
{
	int ret = 0;

	//qst_logi("interface : %d--%d	", bsp_port.interface, bsp_port.spi_mode);
	if(bsp_port.i2c_type == INTERFACE_I2C_SW)
	{
		// ret = qst_sw_readreg(slave<<1, reg, buff, len);
	}
	else if((bsp_port.i2c_type == INTERFACE_I2C_HW)||(bsp_port.i2c_type == INTERFACE_I2C_HW_1M))
	{
		ret = bsp_i2c2_read(slave<<1, reg, buff, len);
	}
	else
	{
		qst_loge("bsp_i2c2_read_reg no function!!!\r\n");
	}

	return ret;
}

int bsp_i3c_write_reg(unsigned char reg, unsigned char value)
{
	int ret = 0;
#ifdef HAL_I3C_MODULE_ENABLED
	ret = bsp_i3c_write(reg, value);
#endif
	return ret;
}

int bsp_i3c_read_reg(unsigned char reg, uint8_t* buff, unsigned short len)
{
	int ret = 0;
#ifdef HAL_I3C_MODULE_ENABLED
	ret = bsp_i3c_read(reg, buff, len);
#endif
	return ret;
}

int bsp_spi_write_reg(unsigned char reg, unsigned char value)
{
	int ret = 0;
#ifdef HAL_SPI_MODULE_ENABLED

	if(bsp_port.spi_type == INTERFACE_SPI_HW4)
	{
		ret = qst_hw_spi_write(reg, value);
	}
	else if(bsp_port.spi_type == INTERFACE_SPI_SW4)
	{
		if(bsp_port.spi_mode == EVB_SPI_MODE0)
		{
			ret = qst_sw_spi4_mode0_write(reg, value);
		}
		else if(bsp_port.spi_mode == EVB_SPI_MODE3)
		{
			ret = qst_sw_spi4_mode3_write(reg, value);
		}
	}
	else if(bsp_port.spi_type == INTERFACE_SPI_SW3)
	{
		if(bsp_port.spi_mode == EVB_SPI_MODE0)
		{
			ret = qst_sw_spi3_mode0_write(reg, value);
		}
		else if(bsp_port.spi_mode == EVB_SPI_MODE3)
		{
			ret = qst_sw_spi3_mode3_write(reg, value);
		}
	}
#endif
	return ret;
}

int bsp_spi_read_reg(unsigned char reg, uint8_t* buff, uint16_t len)
{
	int ret = 0;
#ifdef HAL_SPI_MODULE_ENABLED
	if(bsp_port.spi_type == INTERFACE_SPI_HW4)
	{
		ret = qst_hw_spi_read(reg, buff, len);
	}
	else if(bsp_port.spi_type == INTERFACE_SPI_SW4)
	{
		if(bsp_port.spi_mode == EVB_SPI_MODE0)
		{
			ret = qst_sw_spi4_mode0_read(reg, buff, len);
		}
		else if(bsp_port.spi_mode == EVB_SPI_MODE3)
		{
			ret = qst_sw_spi4_mode3_read(reg, buff, len);
		}
	}
	else if(bsp_port.spi_type == INTERFACE_SPI_SW3)
	{
		if(bsp_port.spi_mode == EVB_SPI_MODE0)
		{
			ret = qst_sw_spi3_mode0_read(reg, buff, len);
		}
		else if(bsp_port.spi_mode == EVB_SPI_MODE3)
		{
			ret = qst_sw_spi3_mode3_read(reg, buff, len);
		}
	}
#endif
	return ret;
}


int bsp_write_reg(unsigned char slave, unsigned char reg, unsigned char value)
{
	if(slave)
	{
		if((bsp_port.i2c_type >= INTERFACE_I2C_SW)&&(bsp_port.i2c_type <= INTERFACE_I2C_HW_1M))
		{
			return bsp_i2c_write_reg(slave, reg, value);
		}
		else if((bsp_port.i3c_type >= INTERFACE_I3C_4M)&&(bsp_port.i3c_type <= INTERFACE_I3C_12_5M))
		{
			return 0;	//bsp_i3c_write_reg(reg, value);
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return bsp_spi_write_reg(reg, value);
	}
}

int bsp_read_reg(unsigned char slave, unsigned char reg, uint8_t* buff, uint16_t len)
{
	if(slave)
	{
		if((bsp_port.i2c_type >= INTERFACE_I2C_SW)&&(bsp_port.i2c_type <= INTERFACE_I2C_HW_1M))
		{
			return bsp_i2c_read_reg(slave, reg, buff, len);
		}
		else if((bsp_port.i3c_type >= INTERFACE_I3C_4M)&&(bsp_port.i3c_type <= INTERFACE_I3C_12_5M))
		{
			return 0;	//bsp_i3c_read_reg(reg, buff, len);
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return bsp_spi_read_reg(reg, buff, len);
	}
}


typedef struct
{
	evb_interface_e 	index;
	char				support;
	char *				info;
}bsp_interface_info;

const bsp_interface_info interface_array[INTERFACE_TOTAL+1] = 
{
	{INTERFACE_I2C_SW,		1,	"I2C-SW200K"},
	{INTERFACE_I2C_HW,		1,	"I2C-400K"},
	{INTERFACE_I2C_HW_1M,	1,	"I2C-1.0M"},
#ifdef HAL_I3C_MODULE_ENABLED
	{INTERFACE_I3C_4M,		1,	"I3C-4.0M"},
	{INTERFACE_I3C_6_25M,	1,	"I3C-6.25M"},
	{INTERFACE_I3C_10M, 	1,	"I3C-10.0M"},
	{INTERFACE_I3C_12_5M,	1,	"I3C-12.5M"},
#else
	{INTERFACE_I3C_4M,		0,	"I3C-4.0M"},
	{INTERFACE_I3C_6_25M,	0,	"I3C-6.25M"},
	{INTERFACE_I3C_10M, 	0,	"I3C-10.0M"},
	{INTERFACE_I3C_12_5M,	0,	"I3C-12.5M"},
#endif
	{INTERFACE_SPI_HW4, 	1,	"SPI-HW4W"},
	{INTERFACE_SPI_HW3, 	1,	"SPI-HW3W"},
	{INTERFACE_SPI_SW4, 	1,	"SPI-SW4W"},
	{INTERFACE_SPI_SW3, 	1,	"SPI-SW3W"},
	{INTERFACE_TOTAL,		0,	"UNKNOW"}
};

void bsp_port_init(int *intf, int spi_sel)
{
	if(*intf < 0)
	{
		while(1)
		{
			int items = 0;
			qst_logi("Select communication procotol:\r\n");
			
			for(items=0; items<(sizeof(interface_array)/sizeof(interface_array[0])); items++)
			{
				if(interface_array[items].support)
				{
					if((interface_array[items].index>=INTERFACE_SPI_HW4) && (interface_array[items].index<=INTERFACE_SPI_SW3))
					{
						if(spi_sel)
						{
							qst_logi("[%d]: %s\r\n", interface_array[items].index, interface_array[items].info);
						}
					}
					else
					{
						qst_logi("[%d]: %s\r\n", interface_array[items].index, interface_array[items].info);
					}
				}
			}

			scanf("%d", intf);
			if((*intf>=INTERFACE_I2C_SW)&&(*intf<INTERFACE_TOTAL))
			{
				if(interface_array[*intf].support)
				{
					break;
				}
			}
			else
			{
				NVIC_SystemReset();
				qst_logi("Select communication procotol:%d error!!!\r\n", *intf);
			}
		}
	}

	if((*intf >= INTERFACE_I2C_SW)&&(*intf <= INTERFACE_I2C_HW_1M))
	{
		qst_logi("init i2c %d\r\n", *intf);
		bsp_port_i2c_init((evb_interface_e)(*intf));
	}
	else if((*intf >= INTERFACE_I3C_4M)&&(*intf <= INTERFACE_I3C_12_5M))
	{
		qst_logi("init i3c %d\r\n", *intf);
		bsp_port_i3c_init((evb_interface_e)(*intf));
	}
	else if((*intf >= INTERFACE_SPI_HW4)&&(*intf <= INTERFACE_SPI_SW3))
	{
		qst_logi("init spi %d-%d\r\n", *intf, bsp_port.spi_mode);
		bsp_port_spi_init((evb_interface_e)(*intf), bsp_port.spi_mode);
	}
	else
	{
		qst_logi("Evb can not support this communication procotol!!!\r\n");
	}
}

void bsp_port_deinit(int sel)
{
	if((sel >= INTERFACE_I2C_SW)&&(sel <= INTERFACE_I2C_HW_1M))
	{
		bsp_port_i2c_deinit();
	}
	else if((sel >= INTERFACE_I3C_4M)&&(sel <= INTERFACE_I3C_12_5M))
	{
		bsp_port_i3c_deinit();
	}
}

char * bsp_get_interface_info(int sel)
{
	if((sel>=INTERFACE_I2C_SW)&&(sel<INTERFACE_TOTAL))
	{
		return interface_array[sel].info;
	}
	else
	{	
		return interface_array[INTERFACE_TOTAL].info;
	}
}

void bsp_hardware_init(void)
{
	HAL_Init();

	SystemClock_Config();
	/* Initialize all configured peripherals */
	//MX_RTC_Init();
	MX_GPIO_Init();	
	MX_ADC1_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */
	MX_IRQ_Init();
	MX_TIM1_Init();	// pwm

	//setvbuf(stdin, NULL, _IONBF, 0);
	//setvbuf(stdout, NULL, _IONBF, 0);
}

//timer1 5ms interrupt
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == htim1.Instance)
	{
		bsp_tim.tim1_flag = 1;
	}
	else if(htim->Instance == htim2.Instance)
	{
		bsp_tim.tim2_flag = 1;
	}
	else if(htim->Instance == htim3.Instance)
	{
		bsp_tim.tim3_flag = 1;
	}
	// else if(htim->Instance == htim4.Instance)
	// {
	// 	bsp_tim.tim4_flag = 1;
	// }
	// else if(htim->Instance == htim5.Instance)
	// {
	// 	bsp_tim.tim5_flag = 1;
	// }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
//	if(GPIO_Pin == GPIO_PIN_9)
//		bsp_irq.irq2_flag = 1;
//	else if(GPIO_Pin == GPIO_PIN_7)
//		bsp_irq.irq1_flag = 1;
	if(GPIO_Pin == GPIO_PIN_1)
		bsp_key.key2_flag = 1;
	else if(GPIO_Pin == GPIO_PIN_13)
		bsp_key.key1_flag = 1;
}

void evb_setup_uart_rx(USART_TypeDef* USARTx, usart_callback func)
{
	if(USARTx == USART1)
	{
		if(func && uart1_rx.rx_cbk == NULL)
		{
			memset(&uart1_rx, 0, sizeof(uart1_rx));
			uart1_rx.rx_cbk = func;
			HAL_NVIC_SetPriority(USART1_IRQn, 2, 0);
			HAL_NVIC_EnableIRQ(USART1_IRQn);
			HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart1_rx.rx_buf, sizeof(uart1_rx.rx_buf));
		}
		else if(func==NULL)
		{
			uart1_rx.rx_cbk = NULL;
			HAL_UART_AbortReceive(&huart1);
		}
		uart1_rx.rx_cplt_count = 1000;
	}
}

void evb_usart_rx_handle(void)
{
	if(uart1_rx.rx_cplt_count)
	{
		uart1_rx.rx_cplt_count--;
		if((uart1_rx.rx_cplt_count == 0)&&(uart1_rx.rx_len))
		{
			if(uart1_rx.rx_cbk)
			{
				uart1_rx.rx_cbk(uart1_rx.rx_buf, uart1_rx.rx_len);
			}
			memset(&uart1_rx.rx_buf, 0, sizeof(uart1_rx.rx_buf));
			uart1_rx.rx_len = 0;
			HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart1_rx.rx_buf, sizeof(uart1_rx.rx_buf));
		}
	}
}


#define UART_RX_WAIT_COUNT	10000

void DMA1_Channel1_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_usart1_rx);
}

void USART1_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart1);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart == &huart1)
	{
		uart1_rx.rx_len = Size;
		uart1_rx.rx_cplt_count = 5;
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	huart->ErrorCode = HAL_UART_ERROR_NONE;

	if(huart == &huart1)
	{
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart1_rx.rx_buf, sizeof(uart1_rx.rx_buf));
	}

}

void evb_key_handle(void)
{
	
	if((bsp_key.key1_delay_count > 0) && (bsp_key.debounce_flag & (1<<QST_KEY1)))
	{
		bsp_key.key1_delay_count--;
		if(bsp_key.key1_delay_count == 0)
		{
			bsp_key.key1_flag = 0;
			qst_logi("key1 debounce done\r\n");
		}
		return;
	}

	if((bsp_key.key2_delay_count > 0) && (bsp_key.debounce_flag & (1<<QST_KEY2)))
	{
		bsp_key.key2_delay_count--;
		if(bsp_key.key2_delay_count == 0)
		{
			bsp_key.key2_flag = 0;
			qst_logi("key2 debounce done\r\n");
		}
		return;
	}

	if(bsp_key.key1_flag)
	{
		bsp_key.key1_flag = 0;
		if(bsp_key.key1_func)
		{
			bsp_key.key1_func();
		}
		if(bsp_key.debounce_flag & (1<<QST_KEY1))
		{
			bsp_key.key1_delay_count = SystemCoreClock/1200;
		}
	}
	if(bsp_key.key2_flag)
	{
		bsp_key.key2_flag = 0;
		if(bsp_key.key2_func)
		{
			bsp_key.key2_func();
		}
		if(bsp_key.debounce_flag & (1<<QST_KEY2))
		{
			bsp_key.key2_delay_count = SystemCoreClock/1200;
		}
	}
}

void evb_setup_user_key(int id, int_callback func, int debounce_en)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if(id == QST_KEY1)
	{
		/*Configure GPIO pin */	
		__HAL_RCC_GPIOC_CLK_ENABLE();
		GPIO_InitStruct.Pin = GPIO_PIN_13;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

		bsp_key.key1_func = func;
	}	
	if(id == QST_KEY2)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
		/*Configure GPIO pins*/
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Pin = GPIO_PIN_1;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		HAL_NVIC_EnableIRQ(EXTI1_IRQn);

		bsp_key.key2_func = func;
	}

	if(debounce_en)
		bsp_key.debounce_flag |= 1<<id;
	else
		bsp_key.debounce_flag &= ~(1<<id);
}

void evb_setup_adc(int channel, FunctionalState enable)
{
	ADC_HandleTypeDef *padc = NULL;

	if(channel == 0)
	{
		padc = &hadc1;
	}

	if(padc)
	{
		if(enable)
			HAL_ADC_Start(padc);
		else
			HAL_ADC_Stop(padc);
	}
}

unsigned int evb_get_adc(int channel)
{
	ADC_HandleTypeDef *padc = NULL;
	unsigned int value = 0;

	if(channel == 0)
	{
		padc = &hadc1;
	}
	
	if(padc)
	{
		//if(HAL_ADC_PollForConversion(padc, 2) == HAL_OK)
		//{
			value = HAL_ADC_GetValue(padc);
		//}
	}
	else
	{
		value = 0xffff;
	}
	
	return value;
}


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

#if defined(USE_MAKEFILE)
int __io_putchar(int ch)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);

    return ch;
}

void __io_putchars(char *ptr, int len)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, HAL_MAX_DELAY);
}

int __io_getchar(void)
{
    uint8_t ch = 0;

    /* 直接读DR寄存器，不依赖 HAL_UART_Receive 的 RxState 状态，
       因为 UART 可能已被 evb_setup_uart_rx 切换到 IT 接收模式
       (RxState = HAL_UART_STATE_BUSY_RX)，此时 HAL_UART_Receive
       会直接返回 HAL_BUSY 而不会等待。 */
    while(!(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE)));
	if(HAL_UART_Receive(&huart1, &ch, 1, HAL_MAX_DELAY) == HAL_OK)
    	HAL_UART_Transmit(&huart1, &ch, 1, HAL_MAX_DELAY);

    return ch;
}

#else

int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);

    return ch;
}

int fgetc(FILE *f)
{
    uint8_t ch = 0;

	while(!(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE)));
	//ch = (uint8_t)(huart1.Instance->DR & 0xFF);
	if(HAL_UART_Receive(&huart1, &ch, 1, HAL_MAX_DELAY) == HAL_OK)
		HAL_UART_Transmit(&huart1, &ch, 1, HAL_MAX_DELAY);

	return ch;
}
#endif

void usart_send_ch(uint8_t ch)
{
    HAL_UART_Transmit(&huart1 , (uint8_t *)&ch, 1, 0xFFFF);
}


