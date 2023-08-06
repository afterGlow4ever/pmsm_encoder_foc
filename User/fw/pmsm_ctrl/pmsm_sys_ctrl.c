//===============================================
//
//	File: pmsm_ctrl.c
//	Author: afterGlow,4ever
//	Date: 02202023
//	Version: v1.0
//
// 	This file is used to operate cotrollers in the loop. 
//
//===============================================

#include "pmsm.h"

extern pmsm_sensorless_status_t PMSM0;
extern pid_pos_controller_t pid_pos_controller0;
extern pid_inc_controller_t pid_inc_controller0;

//===============================================
// pmsm system loop controller init
//===============================================

void pmsm0_system_init(void)
{
#ifdef SPEED_POS_PID		
	pid_pos_controller0_init();
#endif		
#ifdef SPEED_INC_PID	
	pid_inc_controller0_init();
#endif		
#ifdef LOCATION_POS_PID		
#endif		
#ifdef LOCATION_INC_PID		
#endif			
	pmsm0_para_reset();
	PMSM0.error_status = MOTOR_NORMAL;
	pmsm0_actutor_init();
	pmsm0_feedback_init();
	pmsm0_loop_init();
}

void pmsm0_system_enable(void)
{
	PMSM0.speed.speed_set = PMSM0.speed.speed_target;
	pmsm0_actutor_dir_set(PMSM0.direction.dir_target);// start direction		
#ifdef SPEED_POS_PID			
	pid_pos_controller0.target_val = PMSM0.speed.speed_target;	
#endif		
#ifdef SPEED_INC_PID	
	pid_inc_controller0.target_val = PMSM0.speed.speed_target;	
#endif		
#ifdef LOCATION_POS_PID		
#endif		
#ifdef LOCATION_INC_PID		
#endif			
	PMSM0.enable_status = MOTOR_ENABLE;
	PMSM0.operation_status = MOTOR_VVVF_STAGE;
	PMSM0.error_status = MOTOR_NORMAL;
	PMSM0.vvvf.vvvf_status = VVVF_START;
	PMSM0.emfzero_detect.emfzero_detect_status = EMFZERO_DETECTING;
//	pmsm0_encoder_enable();
	pmsm0_actutor_enable();
	delay_ms(50);
	pmsm0_feedback_enable();
  pmsm0_loop_enable();
}

void pmsm0_system_disable(void)
{
//	pmsm0_encoder_disable();
	pmsm0_actutor_disable();
	pmsm0_feedback_disable();
	pmsm0_loop_disable();
#ifdef SPEED_POS_PID		
	pid_pos_controller0_para_reset();
#endif		
#ifdef SPEED_INC_PID		
	pid_inc_controller0_para_reset();
#endif		
#ifdef LOCATION_POS_PID		
#endif		
#ifdef LOCATION_INC_PID		
#endif			
	pmsm0_para_reset();
}

void pmsm0_para_reset(void)
{
	PMSM0.enable_status = MOTOR_DISABLE;
	PMSM0.operation_status = MOTOR_STOP;
	PMSM0.stall_timeout = 0;
	PMSM0.speed.speed_target = 0;	
	PMSM0.speed.speed_set = 0;		
	PMSM0.speed.speed_set_dutycycle = 0;		
	PMSM0.speed.speed_feedback = 0;
	PMSM0.speed.filter_member_count = 0;
	PMSM0.adc.adc_raw_ave[0] = 0;
	PMSM0.adc.adc_raw_ave[1] = 0;	
	PMSM0.adc.adc_raw_ave[2] = 0;	
	PMSM0.adc.adc_raw_ave[3] = 0;	
	PMSM0.adc.adc_raw_ave[4] = 0;	
	PMSM0.adc.adc_calibration_times = 0;
	PMSM0.phase_current.u_current = 0;
	PMSM0.phase_current.v_current = 0;
	PMSM0.phase_current.w_current = 0;	
	PMSM0.encoder_status.encoder_elapse_count = 0;
//	PMSM0.encoder_status.encoder_current_count = 0;
	PMSM0.encoder_status.encoder_total_count = 0;	
	PMSM0.encoder_status.encoder_total_count_old = 0;		
	PMSM0.encoder_status.encoder_speed = 0;
	PMSM0.vvvf.vvvf_status = VVVF_DISABLE;
	PMSM0.vvvf.vvvf_step_delay = 0;
	PMSM0.vvvf.vvvf_step_count = 0;
	PMSM0.vvvf.vvvf_step_time = 0;	
	PMSM0.vvvf.vvvf_commutation_count = 0;	
	PMSM0.emfzero_detect.emfzero_detect_status = EMFZERO_DETECT_DISABLE;
	PMSM0.emfzero_detect.emfzero_detect_queue[0] = 0;
	PMSM0.emfzero_detect.emfzero_detect_queue[1] = 0;	
	PMSM0.emfzero_detect.emfzero_detect_queue[2] = 0;	
	PMSM0.emfzero_detect.emfzero_detect[0] = 0;
	PMSM0.emfzero_detect.emfzero_detect[1] = 0;	
	PMSM0.emfzero_detect.emfzero_detect[2] = 0;		
	PMSM0.emfzero_detect.emfzero_detect_last[0] = 0;
	PMSM0.emfzero_detect.emfzero_detect_last[1] = 0;	
	PMSM0.emfzero_detect.emfzero_detect_last[2] = 0;		
	PMSM0.emfzero_detect.emfedge_detect[0] = 0;
	PMSM0.emfzero_detect.emfedge_detect[1] = 0;	
	PMSM0.emfzero_detect.emfedge_detect[2] = 0;			
	PMSM0.emfzero_detect.emfzero_time = 0;
	PMSM0.emfzero_detect.emfzero_delay_time = 0;		
	PMSM0.emfzero_detect.emfzero_commutation_count = 0;	
	PMSM0.emfzero_detect.emfhigh_time = 0;
	PMSM0.emfzero_detect.emfedge_count = 0;	
	PMSM0.emfzero_detect.emfzero_last = 0;
	PMSM0.emfzero_detect.emfzero = 0;
}

void pmsm0_system_always_on_enable(void)
{
	pmsm0_feedback_always_on_enable();
}

//===============================================
// pmsm system calibration operation
//===============================================

void pmsm0_system_calibration(void)
{
	if((PMSM0.operation_status == MOTOR_STOP) || (PMSM0.operation_status == MOTOR_CALIBRATION_STAGE))
	{
		pmsm0_reference_phase_voltage_feedback();
	}
}

//===============================================
// pmsm system operation set
//===============================================

void pmsm0_speed_up(void)
{
	PMSM0.speed.speed_target = PMSM0.speed.speed_target + 100;
	if(PMSM0.speed.speed_target > 0)
	{
		if(PMSM0.speed.speed_target > PMSM_MAX_SPEED_PROTECT)
			PMSM0.speed.speed_target = PMSM_MAX_SPEED_PROTECT;
		if(PMSM0.speed.speed_target < PMSM_MIN_SPEED_PROTECT)
			PMSM0.speed.speed_target = PMSM_MIN_SPEED_PROTECT;		
	}
	else if(PMSM0.speed.speed_target < 0)
	{
		if(PMSM0.speed.speed_target < -PMSM_MAX_SPEED_PROTECT)
			PMSM0.speed.speed_target = -PMSM_MAX_SPEED_PROTECT;
		if(PMSM0.speed.speed_target > -PMSM_MIN_SPEED_PROTECT)
			PMSM0.speed.speed_target = -PMSM_MIN_SPEED_PROTECT;		
	}		
#ifdef SPEED_POS_PID		
	pid_pos_controller0.target_val = PMSM0.speed.speed_target;
#endif
#ifdef SPEED_INC_PID	
	pid_inc_controller0.target_val = PMSM0.speed.speed_target;
#endif
}

void pmsm0_speed_down(void)
{
	PMSM0.speed.speed_target = PMSM0.speed.speed_target - 100;
	if(PMSM0.speed.speed_target > 0)
	{
		if(PMSM0.speed.speed_target > PMSM_MAX_SPEED_PROTECT)
			PMSM0.speed.speed_target = PMSM_MAX_SPEED_PROTECT;
		if(PMSM0.speed.speed_target < PMSM_MIN_SPEED_PROTECT)
			PMSM0.speed.speed_target = PMSM_MIN_SPEED_PROTECT;		
	}
	else if(PMSM0.speed.speed_target < 0)
	{
		if(PMSM0.speed.speed_target < -PMSM_MAX_SPEED_PROTECT)
			PMSM0.speed.speed_target = -PMSM_MAX_SPEED_PROTECT;
		if(PMSM0.speed.speed_target > -PMSM_MIN_SPEED_PROTECT)
			PMSM0.speed.speed_target = -PMSM_MIN_SPEED_PROTECT;		
	}	
#ifdef SPEED_POS_PID		
	pid_pos_controller0.target_val = PMSM0.speed.speed_target;
#endif
#ifdef SPEED_INC_PID	
	pid_inc_controller0.target_val = PMSM0.speed.speed_target;
#endif
}

void pmsm0_reverse(void)
{
	PMSM0.speed.speed_target = - PMSM0.speed.speed_target;	
#ifdef SPEED_CLOSE_LOOP	
	#ifdef SPEED_POS_PID		
	pid_pos_controller0.target_val = PMSM0.speed.speed_target;
	#endif
	#ifdef SPEED_INC_PID	
	pid_inc_controller0.target_val = PMSM0.speed.speed_target;
	#endif	
#endif
}

//===============================================
// pmsm system loop init
//===============================================

static void pmsm0_loop_tim_init(void)
{
	PMSM0_LOOP_TIM_CLK_ENABLE(); 

	PMSM0.TIM_loop_InitStructure.Instance = PMSM0_LOOP_TIM;
	PMSM0.TIM_loop_InitStructure.Init.Period = PMSM0_LOOP_PERIOD_COUNT - 1;       

	PMSM0.TIM_loop_InitStructure.Init.Prescaler = PMSM0_LOOP_PRESCALER_COUNT - 1;	
  PMSM0.TIM_loop_InitStructure.Init.CounterMode = TIM_COUNTERMODE_UP;        
  PMSM0.TIM_loop_InitStructure.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;  

	HAL_TIM_Base_Init(&PMSM0.TIM_loop_InitStructure);

	HAL_NVIC_SetPriority(PMSM0_LOOP_TIM_IRQn, 1, 3);
	HAL_NVIC_EnableIRQ(PMSM0_LOOP_TIM_IRQn);	
}

void pmsm0_loop_init(void)
{
	pmsm0_loop_tim_init();
}

void pmsm0_loop_enable(void)
{
	HAL_TIM_Base_Start_IT(&PMSM0.TIM_loop_InitStructure);	
}

void pmsm0_loop_disable(void)
{
	HAL_TIM_Base_Stop_IT(&PMSM0.TIM_loop_InitStructure);	
}

void PMSM0_LOOP_TIM_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&PMSM0.TIM_loop_InitStructure);
}

//===============================================
// pmsm system loop controller
//===============================================

static void pmsm0_speed_loop(void)
{
	// In sensorless mode, ctrl loop must be carried out after vvvf stage & stable commutation
	if((PMSM0.enable_status == MOTOR_ENABLE) && (PMSM0.operation_status == MOTOR_DRIVING))
	{
#ifdef SPEED_CLOSE_LOOP	
	#ifdef SPEED_POS_PID		
		pid_pos_controller0.target_val = PMSM0.speed.speed_target;
		PMSM0.speed.speed_set = pid_pos_controller0_control(PMSM0.speed.speed_feedback);
	#endif		
	#ifdef SPEED_INC_PID		
		pid_inc_controller0.target_val = PMSM0.speed.speed_target;
		PMSM0.speed.speed_set = pid_inc_controller0_control(PMSM0.speed.speed_feedback);
	#endif		
#else
		PMSM0.speed.speed_set = PMSM0.speed.speed_target;	
#endif		
		if(PMSM0.speed.speed_target < 0)
		{
			if((- PMSM0.speed.speed_set) > PMSM_MAX_SPEED_PROTECT)
				PMSM0.speed.speed_set =  - PMSM_MAX_SPEED_PROTECT;	
			if((- PMSM0.speed.speed_set) < PMSM_MIN_SPEED_PROTECT)
				PMSM0.speed.speed_set =  - PMSM_MIN_SPEED_PROTECT;				
		}
		else
		{
			if(PMSM0.speed.speed_set > PMSM_MAX_SPEED_PROTECT)
				PMSM0.speed.speed_set = PMSM_MAX_SPEED_PROTECT;	
			if(PMSM0.speed.speed_set < PMSM_MIN_SPEED_PROTECT)
				PMSM0.speed.speed_set = PMSM_MIN_SPEED_PROTECT;				
		}		
		pmsm0_actutor_speed_set();
	}
}

void pmsm0_loop(void)
{
	pmsm0_speed_loop();
#ifdef MINIBALANCE_SSDP_EN 				
	DataScope();
#endif	
}

