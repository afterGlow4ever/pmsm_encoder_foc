//===============================================
//
//	File: commutation.c
//	Author: afterGlow,4ever
//	Date: 02202023
//	Version: v1.0
//
// 	This file is used to commutation according to
//  the commutation true table.
//
//===============================================

#include "pmsm.h"

extern pmsm_sensorless_status_t PMSM0;

//===============================================
// PMSM commutation function ptr
//===============================================

commutation_funptr pmsm0_fwd_commutation_funptr[6] =
{
    &pmsm0_commutation_uhwl, 
    &pmsm0_commutation_vhul, 
    &pmsm0_commutation_vhwl,
    &pmsm0_commutation_whvl, 
    &pmsm0_commutation_uhvl, 
    &pmsm0_commutation_whul
};

commutation_funptr pmsm0_rev_commutation_funptr[6] =
{
	  &pmsm0_commutation_uhvl, 
    &pmsm0_commutation_vhwl,	
    &pmsm0_commutation_uhwl, 
    &pmsm0_commutation_whul,	
    &pmsm0_commutation_whvl, 
    &pmsm0_commutation_vhul  
};

commutation_funptr pmsm0_accelerate_commutation_funptr[6] =
{
    &pmsm0_commutation_uhvl, 	
    &pmsm0_commutation_uhwl, 
    &pmsm0_commutation_vhwl,	
    &pmsm0_commutation_vhul, 
    &pmsm0_commutation_whul,
    &pmsm0_commutation_whvl 
};

//===============================================
// PMSM commutation function
// H PWM L ON
// Forward/Anticlockwise: 154623
// Backward/Clockwise: 645132
//===============================================

// U+ W-
// Forward/Anticlockwise: 1
// Backward/Clockwise: 6
void pmsm0_commutation_uhwl(void)
{
	__HAL_TIM_SET_COMPARE(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL1, PMSM0.speed.speed_set_dutycycle);	
	__HAL_TIM_SET_COMPARE(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL2, 0);		
	__HAL_TIM_SET_COMPARE(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL3, 0); 
	HAL_GPIO_WritePin(PMSM0_OCNPWM1_GPIO_PORT, PMSM0_OCNPWM1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PMSM0_OCNPWM2_GPIO_PORT, PMSM0_OCNPWM2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PMSM0_OCNPWM3_GPIO_PORT, PMSM0_OCNPWM3_PIN, GPIO_PIN_SET);
}

// V+ U-
// Forward/Anticlockwise: 2
// Backward/Clockwise: 5
void pmsm0_commutation_vhul(void)
{
	__HAL_TIM_SET_COMPARE(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL1, 0);	
	__HAL_TIM_SET_COMPARE(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL2, PMSM0.speed.speed_set_dutycycle);		
	__HAL_TIM_SET_COMPARE(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL3, 0); 
	HAL_GPIO_WritePin(PMSM0_OCNPWM1_GPIO_PORT, PMSM0_OCNPWM1_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PMSM0_OCNPWM2_GPIO_PORT, PMSM0_OCNPWM2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PMSM0_OCNPWM3_GPIO_PORT, PMSM0_OCNPWM3_PIN, GPIO_PIN_RESET);
}

// V+ W-
// Forward/Anticlockwise: 3
// Backward/Clockwise: 4
void pmsm0_commutation_vhwl(void)
{
	__HAL_TIM_SET_COMPARE(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL1, 0);	
	__HAL_TIM_SET_COMPARE(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL2, PMSM0.speed.speed_set_dutycycle);		
	__HAL_TIM_SET_COMPARE(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL3, 0); 
	HAL_GPIO_WritePin(PMSM0_OCNPWM1_GPIO_PORT, PMSM0_OCNPWM1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PMSM0_OCNPWM2_GPIO_PORT, PMSM0_OCNPWM2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PMSM0_OCNPWM3_GPIO_PORT, PMSM0_OCNPWM3_PIN, GPIO_PIN_SET);
}

// W+ V-
// Forward/Anticlockwise: 4
// Backward/Clockwise: 3
void pmsm0_commutation_whvl(void)
{
	__HAL_TIM_SET_COMPARE(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL1, 0);	
	__HAL_TIM_SET_COMPARE(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL2, 0);		
	__HAL_TIM_SET_COMPARE(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL3, PMSM0.speed.speed_set_dutycycle); 
	HAL_GPIO_WritePin(PMSM0_OCNPWM1_GPIO_PORT, PMSM0_OCNPWM1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PMSM0_OCNPWM2_GPIO_PORT, PMSM0_OCNPWM2_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PMSM0_OCNPWM3_GPIO_PORT, PMSM0_OCNPWM3_PIN, GPIO_PIN_RESET);
}

// U+ V-
// Forward/Anticlockwise: 5
// Backward/Clockwise: 2
void pmsm0_commutation_uhvl(void)
{
	__HAL_TIM_SET_COMPARE(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL1, PMSM0.speed.speed_set_dutycycle);	
	__HAL_TIM_SET_COMPARE(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL2, 0);		
	__HAL_TIM_SET_COMPARE(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL3, 0); 
	HAL_GPIO_WritePin(PMSM0_OCNPWM1_GPIO_PORT, PMSM0_OCNPWM1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PMSM0_OCNPWM2_GPIO_PORT, PMSM0_OCNPWM2_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PMSM0_OCNPWM3_GPIO_PORT, PMSM0_OCNPWM3_PIN, GPIO_PIN_RESET);
}

// W+ U-
// Forward/Anticlockwise: 6
// Backward/Clockwise: 1
void pmsm0_commutation_whul(void)
{
	__HAL_TIM_SET_COMPARE(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL1, 0);	
	__HAL_TIM_SET_COMPARE(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL2, 0);		
	__HAL_TIM_SET_COMPARE(&PMSM0.TIM_PWM_InitStructure, PMSM0_PWM_CHANNEL3, PMSM0.speed.speed_set_dutycycle); 
	HAL_GPIO_WritePin(PMSM0_OCNPWM1_GPIO_PORT, PMSM0_OCNPWM1_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PMSM0_OCNPWM2_GPIO_PORT, PMSM0_OCNPWM2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PMSM0_OCNPWM3_GPIO_PORT, PMSM0_OCNPWM3_PIN, GPIO_PIN_RESET);
}

//===============================================
// sensorless commutation
// Forward/Anticlockwise: 154623
// Backward/Clockwise: 645132
//===============================================

void pmsm0_sensorless_commutation(uint8_t emf_zero_step)
{
	if((emf_zero_step > 6) || (emf_zero_step == 0))
	{
		pmsm0_system_disable();
		PMSM0.error_status = MOTOR_COMMUTATION_ERROR;					
	}	
	if(PMSM0.direction.dir_set == MOTOR_FWD)
	{ 
		pmsm0_fwd_commutation_funptr[emf_zero_step - 1]();
	}
	else
	{
		pmsm0_rev_commutation_funptr[emf_zero_step - 1]();
	}	
	HAL_TIM_GenerateEvent(&PMSM0.TIM_PWM_InitStructure, TIM_EVENTSOURCE_COM); 
}

void pmsm0_sensorless_commutation_failure(void)
{
	if(PMSM0.stall_timeout++ > PMSM_STALL_THRESHOLD)// stall when not receive a valid hall value in a hall period
	{
		pmsm0_system_disable();
		PMSM0.error_status = MOTOR_STALL;
	}	
}

//===============================================
// sensorless commutation in accelerate status
// Forward/Anticlockwise: 154623
// Backward/Clockwise: 645132
//===============================================

void pmsm0_sensorless_accelerate_commutation(uint8_t commutation_step)
{
	if(commutation_step > 6)
	{
		pmsm0_system_disable();
		PMSM0.error_status = MOTOR_COMMUTATION_ERROR;					
	}		
	if(PMSM0.direction.dir_set == MOTOR_FWD)
	{
		pmsm0_accelerate_commutation_funptr[commutation_step]();
	}
	else
	{
		pmsm0_accelerate_commutation_funptr[5 - commutation_step]();
	}
	HAL_TIM_GenerateEvent(&PMSM0.TIM_PWM_InitStructure, TIM_EVENTSOURCE_COM);  	
}

