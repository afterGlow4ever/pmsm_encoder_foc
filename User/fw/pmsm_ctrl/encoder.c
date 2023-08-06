//===============================================
//
//	File: encoder.c
//	Author: afterGlow,4ever
//	Date: 03252023
//	Version: v1.0
//
// 	This file is related to encoder.
//
//===============================================

#include "pmsm.h"

extern pmsm_sensorless_status_t PMSM0;

static void pmsm0_encoder_gpio_init(void) 
{
  PMSM0_ENCODER_ENCA_GPIO_CLK_ENABLE();
  PMSM0_ENCODER_ENCB_GPIO_CLK_ENABLE();

	PMSM0.GPIO_ENCODER_InitStructure.Pull = GPIO_NOPULL;
  PMSM0.GPIO_ENCODER_InitStructure.Speed = GPIO_SPEED_HIGH;
  PMSM0.GPIO_ENCODER_InitStructure.Mode = GPIO_MODE_AF_PP; 

	// enca
	PMSM0.GPIO_ENCODER_InitStructure.Alternate = PMSM0_ENCODER_ENCA_AF;	
  PMSM0.GPIO_ENCODER_InitStructure.Pin = PMSM0_ENCODER_ENCA_PIN;
  HAL_GPIO_Init(PMSM0_ENCODER_ENCA_GPIO_PORT, &PMSM0.GPIO_ENCODER_InitStructure);	
	// encb
	PMSM0.GPIO_ENCODER_InitStructure.Alternate = PMSM0_ENCODER_ENCB_AF;
  PMSM0.GPIO_ENCODER_InitStructure.Pin = PMSM0_ENCODER_ENCB_PIN;
  HAL_GPIO_Init(PMSM0_ENCODER_ENCB_GPIO_PORT, &PMSM0.GPIO_ENCODER_InitStructure);	
}

//===============================================
// encoder timer init
// system clock 400MHz
// timer clock 200MHz
//===============================================

static void pmsm0_encoder_tim_init(void)
{
	PMSM0_ENCODER_TIM_CLK_ENABLE();

	PMSM0.TIM_ENCODER_InitStructure.Instance = PMSM0_ENCODER_TIM;                     
	PMSM0.TIM_ENCODER_InitStructure.Init.Prescaler = PMSM0_ENCODER_PRESCALER_COUNT - 1;                             
	PMSM0.TIM_ENCODER_InitStructure.Init.Period = PMSM0_ENCODER_PERIOD_COUNT - 1;
	PMSM0.TIM_ENCODER_InitStructure.Init.CounterMode = TIM_COUNTERMODE_UP;
	PMSM0.TIM_ENCODER_InitStructure.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;      
	PMSM0.TIM_ENCODER_InitStructure.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	
	PMSM0.TIM_ENCODER_CHInitStructure.EncoderMode = TIM_ENCODERMODE_TI12;             
	PMSM0.TIM_ENCODER_CHInitStructure.IC1Polarity = TIM_ICPOLARITY_RISING;            
	PMSM0.TIM_ENCODER_CHInitStructure.IC1Selection = TIM_ICSELECTION_DIRECTTI;        
	PMSM0.TIM_ENCODER_CHInitStructure.IC1Prescaler = TIM_ICPSC_DIV1;                  
	PMSM0.TIM_ENCODER_CHInitStructure.IC1Filter = 0;                                 
	PMSM0.TIM_ENCODER_CHInitStructure.IC2Polarity = TIM_ICPOLARITY_RISING;            
	PMSM0.TIM_ENCODER_CHInitStructure.IC2Selection = TIM_ICSELECTION_DIRECTTI;        
	PMSM0.TIM_ENCODER_CHInitStructure.IC2Prescaler = TIM_ICPSC_DIV1;                  
	PMSM0.TIM_ENCODER_CHInitStructure.IC2Filter = 0;                                 
	HAL_TIM_Encoder_Init(&PMSM0.TIM_ENCODER_InitStructure, &PMSM0.TIM_ENCODER_CHInitStructure); 

	//need clear once (found in test)
	__HAL_TIM_CLEAR_IT(&PMSM0.TIM_ENCODER_InitStructure,TIM_IT_UPDATE);
  __HAL_TIM_SET_COUNTER(&PMSM0.TIM_ENCODER_InitStructure, 0);		
	
	//timer int for encoder process
	HAL_NVIC_SetPriority(PMSM0_ENCODER_TIM_IRQn, 0, 2);
	HAL_NVIC_EnableIRQ(PMSM0_ENCODER_TIM_IRQn);		
}

//===============================================
// encoder init
//===============================================

void pmsm0_encoder_init(void)
{
	pmsm0_encoder_gpio_init();
	pmsm0_encoder_tim_init();
}

//===============================================
// encoder switch
//===============================================

void pmsm0_encoder_disable(void)
{
	//disable int
  __HAL_TIM_URS_DISABLE(&PMSM0.TIM_ENCODER_InitStructure);	
	__HAL_TIM_DISABLE_IT(&PMSM0.TIM_ENCODER_InitStructure, TIM_IT_UPDATE); 
	HAL_TIM_Encoder_Stop(&PMSM0.TIM_ENCODER_InitStructure, TIM_CHANNEL_ALL); 	
//  HAL_TIM_Encoder_Stop(&PMSM0.TIM_ENCODER_InitStructure, PMSM0_ENCODER_CHANNEL1);
//  HAL_TIM_Encoder_Stop(&PMSM0.TIM_ENCODER_InitStructure, PMSM0_ENCODER_CHANNEL2);
  __HAL_TIM_SET_COUNTER(&PMSM0.TIM_ENCODER_InitStructure, 0);	
  __HAL_TIM_SET_COMPARE(&PMSM0.TIM_ENCODER_InitStructure, PMSM0_ENCODER_CHANNEL1, 0);
  __HAL_TIM_SET_COMPARE(&PMSM0.TIM_ENCODER_InitStructure, PMSM0_ENCODER_CHANNEL2, 0);
}

void pmsm0_encoder_enable(void)
{
	//need clear once (found in test)
	__HAL_TIM_CLEAR_IT(&PMSM0.TIM_ENCODER_InitStructure,TIM_IT_UPDATE);	
  __HAL_TIM_SET_COUNTER(&PMSM0.TIM_ENCODER_InitStructure, 0);		
	__HAL_TIM_ENABLE_IT(&PMSM0.TIM_ENCODER_InitStructure, TIM_IT_UPDATE); 
  __HAL_TIM_URS_ENABLE(&PMSM0.TIM_ENCODER_InitStructure);
	HAL_TIM_Encoder_Start(&PMSM0.TIM_ENCODER_InitStructure, TIM_CHANNEL_ALL); 	
//	HAL_TIM_Encoder_Start(&PMSM0.TIM_ENCODER_InitStructure, PMSM0_ENCODER_CHANNEL1); 
//	HAL_TIM_Encoder_Start(&PMSM0.TIM_ENCODER_InitStructure, PMSM0_ENCODER_CHANNEL2); 	
}

//===============================================
// encoder elapse counter
//===============================================

void pmsm0_encoder_elapse_counter(void)
{
	if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&PMSM0.TIM_ENCODER_InitStructure))
	{
		// reverse
		PMSM0.encoder_status.encoder_elapse_count--;
	}
	else
	{
		// forward
		PMSM0.encoder_status.encoder_elapse_count++;
	} 
}

void pmsm0_encoder_total_counter(void)
{
	PMSM0.encoder_status.encoder_total_count = 
	__HAL_TIM_GET_COUNTER(&PMSM0.TIM_ENCODER_InitStructure) + PMSM0.encoder_status.encoder_elapse_count * 65536;
}

//===============================================
// encoder int handler
//===============================================

void PMSM0_ENCODER_TIM_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&PMSM0.TIM_ENCODER_InitStructure);
}
