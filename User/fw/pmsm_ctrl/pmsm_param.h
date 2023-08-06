//===============================================
//
//	File: pmsm_param.h
//	Author: afterGlow,4ever
//	Date: 06252022
//	Version: v1.0
//
// 	This file is including pmsm basic parameters.
//
//===============================================

#ifndef _PMSM_PARAM_H_
#define _PMSM_PARAM_H_

//===============================================
// PMSM basic parameters
//===============================================
#define PMSM_MAX_SPEED 4500
#define PMSM_MAX_SPEED_PROTECT 2000
#define PMSM_MIN_SPEED_PROTECT 500
#define PMSM_STALL_THRESHOLD 1500
#define PMSM_FIXED_TIME 300
#define PMSM_START_SPEED 250

#define PMSM_POLE_PAIR 4
#define PMSM_POLE 8
#define PMSM_KT 0.06
#define PMSM_KE 0.00427
#define PMSM_J 0.0000119
#define PMSM_R 0.42
#define PMSM_L 0.00112
#define PMSM_TT 0.00204289
#define PMSM_TE 0.002667

//===============================================
// Board parameters
//===============================================
#define ADC_CALIBRATION_TIMES 50
//#define ADC2TEMP (float)(3.3f / 65536.0f / 4700.0f)
//#define ADC2CURRENT (float)(3.3f / 65536.0f / 1200.0f)
//#define ADC2VBUS (float)(3.3f * 25 / 65536.0f)
#define ADC2TEMP (float)(3.3f / 65536.0f / 4700.0f)
#define ADC2CURRENT (float)(3.3f / 65536.0f / 0.12f)
#define ADC2VBUS (float)(3.3f * 25 / 65536.0f)	

//===============================================
// Temp sensor parameters
//===============================================
#define Ka 273.15
#define Rp 10000.0
#define T2 298.15f
#define Bx 3380.0f

//===============================================
// Filter parameters
//===============================================                             
#define SPEED_FILTER_NUM 30

//===============================================
// Pid parameters
//===============================================   
#define SPEED_POS_PID_KP 1
#define SPEED_POS_PID_KI 0.05
#define SPEED_POS_PID_KD 0.1

#define SPEED_INC_PID_KP 1
#define SPEED_INC_PID_KI 0.03
#define SPEED_INC_PID_KD 0.1

#endif
