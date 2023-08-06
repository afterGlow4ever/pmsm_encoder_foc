//===============================================
//
//	File: tim_callback.c
//	Author: afterGlow,4ever
//	Date: 06252022
//	Version: v1.0
//
// 	This file is including any timer callback function. 
//
//===============================================

#include "pmsm.h"

extern pmsm_sensorless_status_t PMSM0;

//void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim)
//{
//  uint8_t hall_temp = 0;
////  hall_temp = pmsm0_hall_getstate();
//	
//	if((hall_temp) >= 1 && (hall_temp <= 6))// hall normal value
//	{
//		if(htim == &PMSM0.TIM_HALL_InitStructure)// hall sensor timer
//		{
//			pmsm0_speed_feedback(hall_temp, __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1));		
//			PMSM0.stall_timeout = 0;
//		}
//		pmsm0_hall_commutation(hall_temp);
//		// In hall mode, only when a sw trigger is occured, commutation can be implemetioned.
//	  HAL_TIM_GenerateEvent(&PMSM0.TIM_PWM_InitStructure, TIM_EVENTSOURCE_COM);  
//	}
//	else
//	{
//		// receive an abnormal hall value
////		pmsm0_hall_failure();
//	}
//}

//===============================================
// elapsed callback function
//===============================================

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &PMSM0.TIM_loop_InitStructure)// loop timer
	{
		pmsm0_loop();
		pmsm0_encoder_total_counter();
		pmsm0_speed_calculate();
	}
  else if(htim == &PMSM0.TIM_ENCODER_InitStructure)// encoder timer
	{		
		//elapse from encoder	
		pmsm0_encoder_elapse_counter();
	}
  else if(htim == &PMSM0.TIM_PWM_InitStructure)// pwm timer
	{
		pmsm0_phase_current_feedback();
		//emf zero control loop
		pmsm_sensorless_top();
//		// commutation failure
//		pmsm0_hall_commutation_failure();
	}
}

