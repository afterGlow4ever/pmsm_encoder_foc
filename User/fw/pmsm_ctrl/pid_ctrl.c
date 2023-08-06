//===============================================
//
//	File: pid_ctrl.c
//	Author: afterGlow,4ever
//	Date: 06252022
//	Version: v1.0
//
// 	This file is related to pid controller. 
//
//===============================================

#include "pmsm.h"

extern pmsm_sensorless_status_t PMSM0;
extern pid_pos_controller_t pid_pos_controller0;
extern pid_inc_controller_t pid_inc_controller0;

//===============================================
// Pid controller init
//===============================================   

void pid_pos_controller0_init(void)
{
  pid_pos_controller0.target_val = 0.0;				
  pid_pos_controller0.actual_val = 0.0;
  pid_pos_controller0.err = 0.0;
  pid_pos_controller0.err_last = 0.0;
  pid_pos_controller0.integral = 0.0;

  pid_pos_controller0.kp = SPEED_POS_PID_KP;
  pid_pos_controller0.ki = SPEED_POS_PID_KI;
  pid_pos_controller0.kd = SPEED_POS_PID_KD;
}

void pid_inc_controller0_init(void)
{
  pid_inc_controller0.target_val = 0.0;				
  pid_inc_controller0.actual_val = 0.0;
  pid_inc_controller0.err = 0.0;
  pid_inc_controller0.err_last2 = 0.0;
  pid_inc_controller0.err_last = 0.0;
	
  pid_inc_controller0.kp = SPEED_INC_PID_KP;
  pid_inc_controller0.ki = SPEED_INC_PID_KI;
  pid_inc_controller0.kd = SPEED_INC_PID_KD;
}

#if 0
void Pos_Pid_Location_Para_Init(void)
{
  PMSM_location_pos_pid.Target_val = 0.0;				
  PMSM_location_pos_pid.Actual_val = 0.0;
  PMSM_location_pos_pid.Err = 0.0;
  PMSM_location_pos_pid.Err_last = 0.0;
  PMSM_location_pos_pid.Integral = 0.0;
      
  PMSM_location_pos_pid.Kp = 80;//24
  PMSM_location_pos_pid.Ki = 0.5;
  PMSM_location_pos_pid.Kd = 2;
}

void Inc_Pid_Location_Para_Init(void)
{
  PMSM_location_inc_pid.Target_val = 0.0;				
  PMSM_location_inc_pid.Actual_val = 0.0;
  PMSM_location_inc_pid.Err = 0.0;
  PMSM_location_inc_pid.Err_last = 0.0;
  PMSM_location_inc_pid.Err_next = 0.0;
	    
  PMSM_location_inc_pid.Kp = 50;
  PMSM_location_inc_pid.Ki = 1.0;
  PMSM_location_inc_pid.Kd = 0.6;
}
#endif

//===============================================
// Pid controller parameters set
//===============================================   

void pid_pos_controller0_para_set(float kp, float ki, float kd)
{
	pid_pos_controller0.kp = kp;
	pid_pos_controller0.ki = ki;	
	pid_pos_controller0.kd = kd;	
}

void pid_inc_controller0_para_set(float kp, float ki, float kd)
{
	pid_inc_controller0.kp = kp;
	pid_inc_controller0.ki = ki;	
	pid_inc_controller0.kd = kd;	
}

#if 0
void Pos_Pid_Location_Para_Set(float Kp, float Ki, float Kd)
{
	PMSM_location_pos_pid.Kp = Kp;
	PMSM_location_pos_pid.Ki = Ki;	
	PMSM_location_pos_pid.Kd = Kd;	
}

void Inc_Pid_Location_Para_Set(float Kp, float Ki, float Kd)
{
	PMSM_location_inc_pid.Kp = Kp;
	PMSM_location_inc_pid.Ki = Ki;	
	PMSM_location_inc_pid.Kd = Kd;	
}
#endif

//===============================================
// Pid controller algorithm
//===============================================   

float pid_pos_controller0_control(float actual_value)
{
	pid_pos_controller0.err = pid_pos_controller0.target_val - actual_value;
	pid_pos_controller0.integral += pid_pos_controller0.err;
	pid_pos_controller0.actual_val = pid_pos_controller0.kp * pid_pos_controller0.err +
														pid_pos_controller0.ki * pid_pos_controller0.integral +
														pid_pos_controller0.kd * (pid_pos_controller0.err - pid_pos_controller0.err_last);
	pid_pos_controller0.err_last = pid_pos_controller0.err;
	
	return pid_pos_controller0.actual_val;
}

float pid_inc_controller0_control(float actual_value)
{
	pid_inc_controller0.err = pid_inc_controller0.target_val - actual_value;
	pid_inc_controller0.actual_val = pid_inc_controller0.actual_val + 
			pid_inc_controller0.kp * (pid_inc_controller0.err - pid_inc_controller0.err_last) +
			pid_inc_controller0.ki * pid_inc_controller0.err +
			pid_inc_controller0.kd * (pid_inc_controller0.err + pid_inc_controller0.err_last2 - 2 * pid_inc_controller0.err_last);
	pid_inc_controller0.err_last2 = pid_inc_controller0.err_last;
	pid_inc_controller0.err_last = pid_inc_controller0.err;
	
	return pid_inc_controller0.actual_val;
}

#if 0
float Pos_Pid_Location_Control(float Actual_value)
{
	PMSM_location_pos_pid.Err = PMSM_location_pos_pid.Target_val - Actual_value;
	PMSM_location_pos_pid.Integral += PMSM_location_pos_pid.Err;
	PMSM_location_pos_pid.Actual_val = PMSM_location_pos_pid.Kp * PMSM_location_pos_pid.Err +
														PMSM_location_pos_pid.Ki * PMSM_location_pos_pid.Integral +
														PMSM_location_pos_pid.Kd * (PMSM_location_pos_pid.Err - PMSM_location_pos_pid.Err_last);
	PMSM_location_pos_pid.Err_last = PMSM_location_pos_pid.Err;
	
	return PMSM_location_pos_pid.Actual_val;
}

float Inc_Pid_Location_Control(float Actual_value)
{
	PMSM_location_inc_pid.Err = PMSM_location_inc_pid.Target_val - Actual_value;
	PMSM_location_inc_pid.Actual_val = PMSM_location_inc_pid.Actual_val + 
			PMSM_location_inc_pid.Kp * (PMSM_location_inc_pid.Err - PMSM_location_inc_pid.Err_next) +
			PMSM_location_inc_pid.Ki * PMSM_location_inc_pid.Err +
			PMSM_location_inc_pid.Kd * (PMSM_location_inc_pid.Err + PMSM_location_inc_pid.Err_last - 2 * PMSM_location_inc_pid.Err_next);
	PMSM_location_inc_pid.Err_last = PMSM_location_inc_pid.Err_next;
	PMSM_location_inc_pid.Err_next = PMSM_location_inc_pid.Err;
	
	return PMSM_location_inc_pid.Actual_val;
}
#endif

//===============================================
// Pid controller reset
//===============================================  

void pid_pos_controller0_para_reset(void)
{	
  pid_pos_controller0.target_val = 0.0;	
  pid_pos_controller0.actual_val = 0.0;
  pid_pos_controller0.err = 0.0;
  pid_pos_controller0.err_last = 0.0;
  pid_pos_controller0.integral = 0.0;
}

void pid_inc_controller0_para_reset(void)
{	
	pid_inc_controller0.target_val = 0.0;	
  pid_inc_controller0.actual_val = 0.0;
  pid_inc_controller0.err = 0.0;
  pid_inc_controller0.err_last2 = 0.0;
  pid_inc_controller0.err_last = 0.0;
}

#if 0
void Pos_Pid_Location_Para_Reset(void)
{	
  PMSM_location_pos_pid.Actual_val = 0.0;
  PMSM_location_pos_pid.Err = 0.0;
  PMSM_location_pos_pid.Err_last = 0.0;
  PMSM_location_pos_pid.Integral = 0.0;
}

void Inc_Pid_Location_Para_Reset(void)
{	
  PMSM_location_inc_pid.Actual_val = 0.0;
  PMSM_location_inc_pid.Err = 0.0;
  PMSM_location_inc_pid.Err_last = 0.0;
  PMSM_location_inc_pid.Err_next = 0.0;
}
#endif
