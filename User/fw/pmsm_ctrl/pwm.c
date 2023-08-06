//===============================================
//
//	File: pwm.c
//	Author: afterGlow,4ever
//	Date: 02202023
//	Version: v1.0
//
// 	This file is related to pwm output. 
//
//===============================================

#include "pmsm.h"

extern pmsm_sensorless_status_t PMSM0;

//===============================================
// pwm gpio init
//===============================================

static void pmsm0_pwm_gpio_init(void) 
{
  PMSM0_OCPWM1_GPIO_CLK_ENABLE();
  PMSM0_OCNPWM1_GPIO_CLK_ENABLE();
  PMSM0_OCPWM2_GPIO_CLK_ENABLE();
  PMSM0_OCNPWM2_GPIO_CLK_ENABLE();
  PMSM0_OCPWM3_GPIO_CLK_ENABLE();
  PMSM0_OCNPWM3_GPIO_CLK_ENABLE();
														   
  PMSM0.GPIO_PWM_InitStructure.Pull = GPIO_NOPULL;
  PMSM0.GPIO_PWM_InitStructure.Speed = GPIO_SPEED_HIGH;
  PMSM0.GPIO_PWM_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; 

	// u-
  PMSM0.GPIO_PWM_InitStructure.Pin = PMSM0_OCNPWM1_PIN;
  HAL_GPIO_Init(PMSM0_OCNPWM1_GPIO_PORT, &PMSM0.GPIO_PWM_InitStructure);	

	// v-
  PMSM0.GPIO_PWM_InitStructure.Pin = PMSM0_OCNPWM2_PIN;	
  HAL_GPIO_Init(PMSM0_OCNPWM2_GPIO_PORT, &PMSM0.GPIO_PWM_InitStructure);

	// w-
  PMSM0.GPIO_PWM_InitStructure.Pin = PMSM0_OCNPWM3_PIN;	
  HAL_GPIO_Init(PMSM0_OCNPWM3_GPIO_PORT, &PMSM0.GPIO_PWM_InitStructure);	

  PMSM0.GPIO_PWM_InitStructure.Mode = GPIO_MODE_AF_PP;  
	
	// u+
  PMSM0.GPIO_PWM_InitStructure.Pin = PMSM0_OCPWM1_PIN;
  PMSM0.GPIO_PWM_InitStructure.Alternate = PMSM0_OCPWM1_AF;	
  HAL_GPIO_Init(PMSM0_OCPWM1_GPIO_PORT, &PMSM0.GPIO_PWM_InitStructure);
  
	// v+
  PMSM0.GPIO_PWM_InitStructure.Pin = PMSM0_OCPWM2_PIN;	
  PMSM0.GPIO_PWM_InitStructure.Alternate = PMSM0_OCPWM2_AF;	
  HAL_GPIO_Init(PMSM0_OCPWM2_GPIO_PORT, &PMSM0.GPIO_PWM_InitStructure);

	// w+
  PMSM0.GPIO_PWM_InitStructure.Pin = PMSM0_OCPWM3_PIN;	
  PMSM0.GPIO_PWM_InitStructure.Alternate =PMSM0_OCPWM3_AF;	
  HAL_GPIO_Init(PMSM0_OCPWM3_GPIO_PORT, &PMSM0.GPIO_PWM_InitStructure);
}

//===============================================
// pwm timer init
// system clock 400MHz
// advanced timer clock 200MHz
// period 20kHz 50us
//===============================================

static void pmsm0_pwm_tim_init(void)
{
  PMSM0_PWM_TIM_CLK_ENABLE(); 
  PMSM0.TIM_PWM_InitStructure.Instance = PMSM0_PWM_TIM;
  PMSM0.TIM_PWM_InitStructure.Init.Period =  PMSM0_PWM_PERIOD_COUNT - 1;
  PMSM0.TIM_PWM_InitStructure.Init.Prescaler = PMSM0_PWM_PRESCALER_COUNT - 1;	
  PMSM0.TIM_PWM_InitStructure.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  PMSM0.TIM_PWM_InitStructure.Init.CounterMode = TIM_COUNTERMODE_UP;
  PMSM0.TIM_PWM_InitStructure.Init.RepetitionCounter = 0;	
  HAL_TIM_PWM_Init(&PMSM0.TIM_PWM_InitStructure);

  PMSM0.TIM_PWM_OCInitStructure.OCMode = TIM_OCMODE_PWM1;
  PMSM0.TIM_PWM_OCInitStructure.Pulse = 0;
  PMSM0.TIM_PWM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
  PMSM0.TIM_PWM_OCInitStructure.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  PMSM0.TIM_PWM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_SET;
  PMSM0.TIM_PWM_OCInitStructure.OCNIdleState = TIM_OCNIDLESTATE_RESET;

  HAL_TIM_PWM_ConfigChannel(&PMSM0.TIM_PWM_InitStructure, &PMSM0.TIM_PWM_OCInitStructure, PMSM0_PWM_CHANNEL1);  
  HAL_TIM_PWM_ConfigChannel(&PMSM0.TIM_PWM_InitStructure, &PMSM0.TIM_PWM_OCInitStructure, PMSM0_PWM_CHANNEL2); 
  HAL_TIM_PWM_ConfigChannel(&PMSM0.TIM_PWM_InitStructure, &PMSM0.TIM_PWM_OCInitStructure, PMSM0_PWM_CHANNEL3); 
  
	//software mode instead of trigger mode
  HAL_TIMEx_ConfigCommutationEvent(&PMSM0.TIM_PWM_InitStructure, TIM_COM_TS_ITRx, TIM_COMMUTATION_SOFTWARE);
	
	//timer int for sensorless process
	HAL_NVIC_SetPriority(PMSM0_PWM_TIM_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(PMSM0_PWM_TIM_IRQn);	
}

//===============================================
// pwm init
//===============================================

void pmsm0_pwm_init(void)
{
	pmsm0_pwm_gpio_init();
	pmsm0_pwm_tim_init();
}

//===============================================
// switch for pwm
//===============================================

void pmsm0_pwm_disable(void)
{
	//disable int
	HAL_TIM_Base_Stop_IT(&PMSM0.TIM_PWM_InitStructure);	
  HAL_TIM_PWM_Stop(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL1);
  HAL_TIM_PWM_Stop(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL2);
  HAL_TIM_PWM_Stop(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL3);	
  __HAL_TIM_SET_COMPARE(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL1, 0);
  __HAL_TIM_SET_COMPARE(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL2, 0);
  __HAL_TIM_SET_COMPARE(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL3, 0);
  HAL_GPIO_WritePin(PMSM0_OCNPWM1_GPIO_PORT, PMSM0_OCNPWM1_PIN, GPIO_PIN_RESET);   
  HAL_GPIO_WritePin(PMSM0_OCNPWM2_GPIO_PORT, PMSM0_OCNPWM2_PIN, GPIO_PIN_RESET);   
  HAL_GPIO_WritePin(PMSM0_OCNPWM3_GPIO_PORT, PMSM0_OCNPWM3_PIN, GPIO_PIN_RESET);   
}

void pmsm0_pwm_enable(void)
{
	// u+
  HAL_TIM_PWM_Start(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL1);
	// v+
  HAL_TIM_PWM_Start(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL2);
	// w+
  HAL_TIM_PWM_Start(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL3);		
	//enable int
	HAL_TIM_Base_Start_IT(&PMSM0.TIM_PWM_InitStructure);
}

//===============================================
// set pwm dutycycle
// implement in commutation
//===============================================

void pmsm0_pwm_set(float speed)
{
	PMSM0.speed.speed_set_dutycycle = speed * PMSM0_PWM_PERIOD_COUNT / PMSM_MAX_SPEED ;
}

void pmsm0_dir_set(pmsm_dir_e dir)
{
	PMSM0.direction.dir_set = dir;
}

//===============================================
// set pwm dutycycle in accelerate status
// implement in commutation
//===============================================

void pmsm0_accelerate_pwm_set(void)
{
	//f	//6 105 //8 150 //5 90 //4 75
	//atk	//8 210 //4 95
	switch(PMSM0.vvvf.vvvf_step_count)
	{
		case 0: PMSM0.speed.speed_set_dutycycle = PMSM_MAX_SPEED / 20; break;
		case 1: PMSM0.speed.speed_set_dutycycle = PMSM_MAX_SPEED / 19; break;
		case 2: PMSM0.speed.speed_set_dutycycle = PMSM_MAX_SPEED / 18; break;
		case 3: PMSM0.speed.speed_set_dutycycle = PMSM_MAX_SPEED / 17; break;
		case 4: PMSM0.speed.speed_set_dutycycle = PMSM_MAX_SPEED / 16; break;
		case 5: PMSM0.speed.speed_set_dutycycle = PMSM_MAX_SPEED / 15; break;	
		case 6: PMSM0.speed.speed_set_dutycycle = PMSM_MAX_SPEED / 14; break;
		case 7: PMSM0.speed.speed_set_dutycycle = PMSM_MAX_SPEED / 13; break;
		case 8: PMSM0.speed.speed_set_dutycycle = PMSM_MAX_SPEED / 12; break;
		case 9: PMSM0.speed.speed_set_dutycycle = PMSM_MAX_SPEED / 11; break;
		case 10: PMSM0.speed.speed_set_dutycycle = PMSM_MAX_SPEED / 10; break;
		case 11: PMSM0.speed.speed_set_dutycycle = PMSM_MAX_SPEED / 10; break;
		case 12: PMSM0.speed.speed_set_dutycycle = PMSM_MAX_SPEED / 9; break;
		case 13: PMSM0.speed.speed_set_dutycycle = PMSM_MAX_SPEED / 9; break;
		case 14: PMSM0.speed.speed_set_dutycycle = PMSM_MAX_SPEED / 8; break;
		case 15: PMSM0.speed.speed_set_dutycycle = PMSM_MAX_SPEED / 8; break;	
//		default: PMSM0.speed.speed_set_dutycycle = PMSM_MAX_SPEED / 8; break;		
		case 16: PMSM0.speed.speed_set_dutycycle = PMSM_MAX_SPEED / 7; break;		
		case 17: PMSM0.speed.speed_set_dutycycle = PMSM_MAX_SPEED / 6; break;	
		case 18: PMSM0.speed.speed_set_dutycycle = PMSM_MAX_SPEED / 5; break;			
		default: PMSM0.speed.speed_set_dutycycle = PMSM_MAX_SPEED / 4; break;	//6 105 //8 150 //5 90 //4 75
	}
}

//===============================================
// pwm int handler
//===============================================

void PMSM0_PWM_TIM_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&PMSM0.TIM_PWM_InitStructure);
}
