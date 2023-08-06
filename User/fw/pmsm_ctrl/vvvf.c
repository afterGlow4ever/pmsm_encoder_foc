//===============================================
//
//	File: emf_zerodetect.c
//	Author: afterGlow,4ever
//	Date: 02202023
//	Version: v1.0
//
// 	This file is related to pmsm vvvf. 
//
//===============================================

#include "pmsm.h"

extern pmsm_sensorless_status_t PMSM0;

//===============================================
// vvvf start stage
// rotor prepositioning & orientation
//===============================================

static void vvvf_start(void)
{
	PMSM0.speed.speed_set_dutycycle = PMSM_START_SPEED;
	// rotor prepositioning & orientation
	pmsm0_commutation_uhvl();
	// initial time 50us * 1200
	PMSM0.vvvf.vvvf_step_time = PMSM_FIXED_TIME;	
	PMSM0.vvvf.vvvf_status = VVVF_DELAY;	
}

//===============================================
// vvvf commutation stage
//===============================================

static void vvvf_commutation(void)
{
	// reducing delay time needed before next commutation
	PMSM0.vvvf.vvvf_step_time -= PMSM0.vvvf.vvvf_step_time / 16 + 1;
	if(PMSM0.vvvf.vvvf_step_time < 75)
	{
		PMSM0.vvvf.vvvf_step_time = 75;							
	}
	
	// setting vvvf
	pmsm0_accelerate_pwm_set();
	
	// 24 steps in vvvf stage
	if(PMSM0.vvvf.vvvf_step_count++ >= 24)
	{
		if(PMSM0.direction.dir_set == MOTOR_FWD)
		{
			PMSM0.speed.speed_target = PMSM0.speed.speed_feedback;
		}
		else
		{
			PMSM0.speed.speed_target = - PMSM0.speed.speed_feedback;
		}		
		PMSM0.vvvf.vvvf_step_count = 24;
	}
	
	// commutation setting
	PMSM0.vvvf.vvvf_commutation_count++;
	if(PMSM0.vvvf.vvvf_commutation_count == 6)
	{
		PMSM0.vvvf.vvvf_commutation_count = 0;
	}
	pmsm0_sensorless_accelerate_commutation(PMSM0.vvvf.vvvf_commutation_count);			

	PMSM0.vvvf.vvvf_status = VVVF_DELAY;		
}

//===============================================
// vvvf top with software fsm
// return value:
// 0: vvvf remains running
// 1: vvvf ok
//===============================================

uint8_t pmsm0_vvvf_top(void)
{
	switch(PMSM0.vvvf.vvvf_status)
	{
		// vvvf initial status
		case VVVF_START:
		{
			vvvf_start();
			return 0;
		}
		// vvvf time counter for next vvvf commutation
		case VVVF_DELAY:
		{
			if(PMSM0.vvvf.vvvf_step_delay++ >= PMSM0.vvvf.vvvf_step_time)
			{
				PMSM0.vvvf.vvvf_step_delay = 0;
				PMSM0.vvvf.vvvf_status = VVVF_COMMUTATION;
			}		
			return 0;			
		}
		// performing vvvf commutation
		case VVVF_COMMUTATION:
		{
			vvvf_commutation();							
			if(PMSM0.vvvf.vvvf_step_count >= 24)
			{
				return 1;	
			}
			else
			{
				return 0;	
			}
		}		
		default:
		{
			pmsm0_system_disable();
			PMSM0.error_status = MOTOR_VVVF_ERROR;				
		}
	}
	return 0;	
}
