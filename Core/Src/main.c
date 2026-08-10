/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "bsp_hardware.h"
#include "gif_cat.h"
#include "game_dino.h"


//#define GAME_DINO

typedef struct
{
	int					port;
	int					count;
	unsigned char		reset_flag;
} qst_evb_t;

static qst_evb_t g_evb;

static void qst_evb_adc_read(void)
{
	unsigned int adc_v = evb_get_adc(0);
	qst_logi("qst_evb_adc_read %d %f\r\n", adc_v, (adc_v*3.3f/4096));
}

static void qst_evb_key_irq1(void)
{
	qst_logi("qst_evb_key_irq1\r\n");
	
	if(g_evb.count == 0)
	{
		g_evb.count = 1;
// pwm tim1 start
		// evb_setup_timer(TIM1, NULL, 0, ENABLE);
// pwm tim1 start
// adc
		evb_setup_adc(0, ENABLE);
		evb_setup_timer(TIM3, qst_evb_adc_read, 100, ENABLE);
// adc
	}
	else
	{
		g_evb.count = 0;

// pwm tim1 stop
		// evb_setup_timer(TIM1, NULL, 0, DISABLE);
// pwm tim1 stop
// adc
		evb_setup_adc(0, ENABLE);
		evb_setup_timer(TIM3, qst_evb_adc_read, 100, DISABLE);
// adc
	}
	// pwm
}

void qst_evb_key_irq2(void)
{
	static uint16_t duty = 0;
	static int16_t step = 10;

	duty = (duty+step);
	if(duty > 1000)
	{
		duty = 1000;
		step = -step;
	}
	else if(duty < 100)
	{
		duty = 100;
		step = -step;
	}
	evb_set_pwm_duty(TIM1, duty);
	qst_logi("qst_evb_key_irq2 duty=%d\r\n", duty);

}


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	bsp_hardware_init();
	qst_delay_ms(100);

	g_evb.port = INTERFACE_I2C_HW;
	bsp_port_init(&g_evb.port, 0);
// oled
	oled_set_lcm(OLED_LCM_0);
	oled_init();
#if defined(USE_DUAL_OLED)
	oled_set_lcm(OLED_LCM_1);
	oled_init();
#endif
// oled

 // app
 	#if defined(GAME_DINO)
 	game_dino_init();
 	#else
 	git_cat_init(GIF_CAT_A);	// tim2 lcm0(pb6 pb7 i2c1)
	#endif
 #if defined(USE_DUAL_OLED)
 	git_cat_init(GIF_CAT_B);	// tim3 lcm1(pb10 pb11 i2c2)
 #endif
 // app
	
// key irq
	// evb_setup_user_key(QST_KEY2, qst_evb_key_irq1, 1);
// keyirq

	//evb_setup_timer(TIM1, NULL, 0, ENABLE);
	//evb_setup_user_key(QST_KEY1, qst_evb_key_irq2, 1);
	//evb_setup_timer(TIM2, qst_evb_key_irq2, 20, ENABLE);

	while (1)
	{
		evb_irq_handle();
		evb_tim_handle();
		evb_key_handle();
		evb_usart_rx_handle();
	}
}


