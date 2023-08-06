//===============================================
//
//	File: pmsm.h
//	Author: afterGlow,4ever
//	Date: 02202023
//	Version: v1.0
//
// 	This file is including pmsm define structure.
//
//===============================================

#ifndef _PMSM_H_
#define _PMSM_H_

#include "pmsm_param.h"
#include "pmsm_bsp.h"
#include "math.h"

//===============================================
// PMSM basic enum and structure
//===============================================

typedef void(*commutation_funptr) (void);

typedef enum
{
  MOTOR_FWD = 0,
  MOTOR_REV = 1,
}pmsm_dir_e;

typedef enum
{
  MOTOR_NORMAL = 0,
  MOTOR_STALL = 1,
  MOTOR_COMMUTATION_ERROR = 2, 	
  MOTOR_VVVF_ERROR = 3, 
	MOTOR_EMFZERO_DETECT_ERROR = 4,
	MOTOR_SENSORLESS_CTRL_ERROR = 5,	
  MOTOR_OVERTEMP = 6,
  MOTOR_LOAD = 7,
  MOTOR_OVERVOLTAGE = 8,
}pmsm_error_status_e;

typedef enum
{
  MOTOR_STOP = 0,
  MOTOR_CALIBRATION_STAGE = 1,	
  MOTOR_CALIBRATION_DONE = 2,		
  MOTOR_VVVF_STAGE = 3,
  MOTOR_EMFZERO_DETECT_STAGE = 4,		
  MOTOR_DRIVING = 5,
  MOTOR_BRAKING = 6,
}pmsm_operation_status_e;

typedef enum
{
	VVVF_DISABLE = 0,
  VVVF_START = 1,
  VVVF_DELAY = 2,
  VVVF_COMMUTATION = 3,
	VVVF_OK = 4,	
}pmsm_vvvf_status_e;

typedef enum
{
	EMFZERO_DETECT_DISABLE = 0,
  EMFZERO_DETECTING = 1,		
  EMFZERO_PRE_COMMUTATION = 2,
	EMFZERO_COMMUTATION = 3,
}pmsm_emfzero_detect_status_e;

typedef enum
{
  MOTOR_DISABLE = 0,
  MOTOR_ENABLE = 1,
}pmsm_enable_status_e;

typedef struct
{	
	pmsm_vvvf_status_e vvvf_status;	
	uint16_t vvvf_step_delay;// delay counter in each acclerate step
	uint8_t vvvf_step_count;// acclerate step counter
	uint16_t vvvf_step_time;// time needed in each acclerate step
	uint8_t vvvf_commutation_count;// commutation counter for vvvf
}pmsm_vvvf_t;

typedef struct
{	
	pmsm_emfzero_detect_status_e emfzero_detect_status;	
	uint8_t emfzero_detect_queue[3];// emf zero detect value queue
	uint8_t emfzero_detect[3];// emf zero detect value result
	uint8_t emfzero_detect_last[3];// last emf zero detect value result	
	uint8_t emfedge_detect[3];// emf edge detect value result		
	uint8_t emfzero;// emf zero result		
	uint8_t emfzero_last;// last emf zero result		
	uint32_t emfzero_time;// emf zero duration	
	uint32_t emfzero_delay_time;// counter for delay 30 degrees time of emf zero 		
	uint32_t emfzero_commutation_count;// counter for emf zero commutation		
	uint32_t emfhigh_time;// emf high duration
	uint32_t emfedge_count;// counter for emf edge 	
}pmsm_emfzero_detect_t;

typedef struct
{	
	float speed_target;// input speed
	float speed_set;// speed from controller to actutor
	uint16_t speed_set_dutycycle;// speed to actutor in dutycycle	
	float speed_feedback_raw;// raw feedback speed from sensor
	float speed_feedback_raw_array[SPEED_FILTER_NUM];
	float speed_feedback;// calculated feedback speed from sensor	
	float speed_error;// input speed minus feedback speed to controller	
	uint8_t filter_member_count;//valid numbers in speed array
}pmsm_speed_t;

typedef struct
{	
	pmsm_dir_e dir_target;// input dir
	pmsm_dir_e dir_set;// dir from controller to actutor
}pmsm_dir_t;

typedef struct
{
	int16_t encoder_elapse_count;
//	int16_t encoder_current_count;	
	int32_t encoder_total_count;
	int32_t encoder_total_count_old;	
	float encoder_speed;		
}pmsm_encoder_sensor_t;

typedef struct
{	
	uint32_t adc_raw_ave[PMSM0_ADC_CHANNEL_NUM];	
	uint32_t adc_raw_reference[PMSM0_ADC_CHANNEL_NUM - 1];
	uint8_t adc_calibration_times;
}pmsm_adc_t;

typedef struct
{
	float u_reference_voltage;
	float v_reference_voltage;
	float w_reference_voltage;	
}pmsm_phase_reference_voltage_t;

typedef struct
{
	float u_current;
	float v_current;
	float w_current;	
}pmsm_phase_current_t;

typedef struct
{
	float u_back_emf;
	float v_back_emf;
	float w_back_emf;	
}pmsm_phase_back_emf_t;

typedef struct
{
	DMA_HandleTypeDef DMA_ADC_InitStructure;
	ADC_HandleTypeDef ADC_InitStructure;
	ADC_ChannelConfTypeDef ADC_CHInitStructure;
	TIM_HandleTypeDef TIM_ENCODER_InitStructure;
	TIM_Encoder_InitTypeDef TIM_ENCODER_CHInitStructure;
	TIM_HandleTypeDef  TIM_PWM_InitStructure;
	TIM_OC_InitTypeDef TIM_PWM_OCInitStructure;	
	TIM_HandleTypeDef TIM_loop_InitStructure;
	GPIO_InitTypeDef GPIO_ADC_InitStructure;		
	GPIO_InitTypeDef GPIO_ENCODER_InitStructure;	
	GPIO_InitTypeDef GPIO_EMFZERO_InitStructure;
	GPIO_InitTypeDef GPIO_PWM_InitStructure;
	GPIO_InitTypeDef GPIO_SD_InitStructure;	
	pmsm_error_status_e error_status;
	pmsm_operation_status_e operation_status;
	pmsm_enable_status_e enable_status;
  pmsm_dir_t direction;
	pmsm_vvvf_t vvvf;
	pmsm_emfzero_detect_t emfzero_detect;
	pmsm_speed_t speed;
	pmsm_encoder_sensor_t encoder_status;
	pmsm_adc_t adc;
	pmsm_phase_reference_voltage_t phase_reference_voltage;
//	uint32_t commutation_count;
  uint32_t stall_timeout; 
	pmsm_phase_current_t phase_current;
	pmsm_phase_back_emf_t phase_back_emf;
	float temperature;
	float vbus;
}pmsm_sensorless_status_t;

typedef struct
{
    float target_val;          
    float actual_val;        	
    float err;             			
    float err_last;          		
    float kp,ki,kd;          	
    float integral;          	
}pid_pos_controller_t;

typedef struct
{
    float target_val;   
    float actual_val;   
    float err;          
    float err_last2;     
		float err_last;     
    float kp,ki,kd;     
}pid_inc_controller_t;

//===============================================
// PMSM function head
//===============================================

// vvvf
uint8_t pmsm0_vvvf_top(void);

// emf zerodetect
uint8_t pmsm0_emfzero_detect_top(void);
void pmsm0_emf_zerodetect_init(void);

//sensorless operation
void pmsm_sensorless_top(void);

// pwm
void pmsm0_pwm_init(void);
void pmsm0_pwm_enable(void);
void pmsm0_pwm_disable(void);
void pmsm0_pwm_set(float speed);
void pmsm0_dir_set(pmsm_dir_e dir);
void pmsm0_accelerate_pwm_set(void);

// encoder
void pmsm0_encoder_init(void);
void pmsm0_encoder_enable(void);
void pmsm0_encoder_disable(void);
void pmsm0_encoder_elapse_counter(void);
void pmsm0_encoder_total_counter(void);

// analog & dma
void pmsm0_analog_init(void);
void pmsm0_dma_init(void);
void pmsm0_dma_adc_enable(void);
void pmsm0_dma_adc_stop(void);

// actutor
void pmsm0_actutor_speed_set(void);
void pmsm0_actutor_dir_set(pmsm_dir_e dir_temp);

// commutation
void pmsm0_commutation_uhwl(void); 
void pmsm0_commutation_vhul(void); 
void pmsm0_commutation_vhwl(void);
void pmsm0_commutation_whvl(void); 
void pmsm0_commutation_uhvl(void); 
void pmsm0_commutation_whul(void);
void pmsm0_sensorless_commutation(uint8_t emf_zero_step);
void pmsm0_sensorless_commutation_failure(void);
void pmsm0_sensorless_accelerate_commutation(uint8_t commutation_step);

// sd
void pmsm0_sd_init(void);
void pmsm0_sd_enable(void);
void pmsm0_sd_disable(void);

// pmsm system parts
void pmsm0_actutor_init(void);//actutor
void pmsm0_feedback_init(void);//feedback
void pmsm0_actutor_enable(void);
void pmsm0_feedback_enable(void);
void pmsm0_feedback_always_on_enable(void);
void pmsm0_actutor_disable(void);
void pmsm0_feedback_disable(void);
void pmsm0_para_reset(void);

// feedback
void pmsm0_board_temperature_feedback(void);
void pmsm0_speed_feedback(void);
void pmsm0_speed_calculate(void);
void pmsm0_reference_phase_voltage_feedback(void);
void pmsm0_phase_current_feedback(void);
void pmsm0_vbus_feedback(void);

// loop
void pmsm0_loop_init(void);
void pmsm0_loop(void);
void pmsm0_loop_enable(void);
void pmsm0_loop_disable(void);
void PMSM0_LOOP_TIM_IRQHandler(void);

// pid
void pid_pos_controller0_init(void);
void pid_inc_controller0_init(void);
void pid_pos_controller0_para_set(float kp, float ki, float kd);
void pid_inc_controller0_para_set(float kp, float ki, float kd);
float pid_pos_controller0_control(float actual_value);
float pid_inc_controller0_control(float actual_value);
void pid_pos_controller0_para_reset(void);
void pid_inc_controller0_para_reset(void);

//system
void pmsm0_system_init(void);
void pmsm0_system_enable(void);
void pmsm0_system_always_on_enable(void);
void pmsm0_system_disable(void);
void pmsm0_system_calibration(void);
void pmsm0_speed_up(void);
void pmsm0_speed_down(void);
void pmsm0_reverse(void);


#endif

