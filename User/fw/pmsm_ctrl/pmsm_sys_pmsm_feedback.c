//===============================================
//
//	File: pmsm_drive.c
//	Author: afterGlow,4ever
//	Date: 02202023
//	Version: v1.0
//
// 	This file is used to operate feedback in the loop. 
//
//===============================================

#include "pmsm.h"

extern pmsm_sensorless_status_t PMSM0;
extern commutation_funptr pmsm0_commutation_funptr[6];
extern uint8_t adc_calibration_update_flag;
DMA_HandleTypeDef DMA_BLDC_InitHandle;
ADC_HandleTypeDef ADC_BLDC_InitHandle;
uint32_t adc_raw1[40];

//===============================================
// pmsm system feedback init
//===============================================

void pmsm0_feedback_init(void)
{
	pmsm0_emf_zerodetect_init();
	pmsm0_encoder_init();
	pmsm0_dma_init();
	pmsm0_analog_init();
}

//===============================================
// pmsm system feedback switch
//===============================================

void pmsm0_feedback_enable(void)
{
	pmsm0_encoder_enable();
}

void pmsm0_feedback_always_on_enable(void)
{
	pmsm0_dma_adc_enable();
}

void pmsm0_feedback_disable(void)
{
	pmsm0_encoder_disable();
}

//===============================================
// calculate initial reference phase voltage
//===============================================

void pmsm0_reference_phase_voltage_feedback(void)
{
	uint32_t reference_raw_data[5];	
	uint8_t i;
	
	for(i = 0; i < PMSM0_ADC_CHANNEL_NUM; i++)
	{
		reference_raw_data[i] = 0;
	}	
//		PMSM0.operation_status = MOTOR_CALIBRATION_STAGE;
//		for(i = 0; i < PMSM0_ADC_CHANNEL_NUM; i++)
//		{
//			PMSM0.adc.adc_raw_reference[i] = PMSM0.adc.adc_raw_ave[i];
//		}	
//			PMSM0.adc.adc_raw_reference[0] = reference_raw_data[0];
//			PMSM0.adc.adc_raw_reference[1] = reference_raw_data[1];
//			PMSM0.adc.adc_raw_reference[2] = reference_raw_data[2];

	//adc calibration
	while(PMSM0.adc.adc_calibration_times < ADC_CALIBRATION_TIMES)
	{
		while(!adc_calibration_update_flag);
		PMSM0.operation_status = MOTOR_CALIBRATION_STAGE;
		for(i = 0; i < PMSM0_ADC_CHANNEL_NUM; i++)
		{
			reference_raw_data[i] += PMSM0.adc.adc_raw_ave[i];
		}

		// wait for next adc data
		PMSM0.adc.adc_calibration_times++;	
		adc_calibration_update_flag = 0;		
	}
	
	//u v w vbus
	PMSM0.adc.adc_raw_reference[0] = reference_raw_data[0] / ADC_CALIBRATION_TIMES;
	PMSM0.adc.adc_raw_reference[1] = reference_raw_data[1] / ADC_CALIBRATION_TIMES;
	PMSM0.adc.adc_raw_reference[2] = reference_raw_data[2] / ADC_CALIBRATION_TIMES;
	PMSM0.adc.adc_raw_reference[3] = reference_raw_data[3] / ADC_CALIBRATION_TIMES;		

//			printf("U=%d.\r\n", reference_raw_data[0]);
//			printf("V=%d.\r\n", reference_raw_data[1]);
//			printf("W=%d.\r\n", reference_raw_data[2]);		
//			printf("VB=%d.\r\n", reference_raw_data[3]);			
//			printf("U=%d.\r\n", PMSM0.adc.adc_raw_reference[0]);
//			printf("V=%d.\r\n", PMSM0.adc.adc_raw_reference[1]);
//			printf("W=%d.\r\n", PMSM0.adc.adc_raw_reference[2]);		
//			printf("VB=%d.\r\n", PMSM0.adc.adc_raw_reference[3]);			

	PMSM0.operation_status = MOTOR_CALIBRATION_DONE;
}

//===============================================
// calculate phase current
//===============================================

void pmsm0_phase_current_feedback(void)
{
	PMSM0.phase_current.u_current = ((int)PMSM0.adc.adc_raw_ave[0] - (int)PMSM0.adc.adc_raw_reference[0]) * ADC2CURRENT;
	PMSM0.phase_current.v_current = ((int)PMSM0.adc.adc_raw_ave[1] - (int)PMSM0.adc.adc_raw_reference[1]) * ADC2CURRENT;
	PMSM0.phase_current.w_current = ((int)PMSM0.adc.adc_raw_ave[2] - (int)PMSM0.adc.adc_raw_reference[2]) * ADC2CURRENT;
//	PMSM0.phase_current.u_current = (PMSM0.adc.adc_raw_ave[0] * 3.3f / 65536.0f) / 0.12;
//	PMSM0.phase_current.v_current = (PMSM0.adc.adc_raw_ave[1] * 3.3f / 65536.0f) / 0.12;
//	PMSM0.phase_current.w_current = (PMSM0.adc.adc_raw_ave[2] * 3.3f / 65536.0f) / 0.12;
}

//===============================================
// calculate power
//===============================================

void pmsm0_vbus_feedback(void)
{
	PMSM0.vbus = PMSM0.adc.adc_raw_ave[3] * ADC2VBUS;
}

//===============================================
// calculate bldc board temperature
// Rt = R *EXP(B*(1/T1-1/T2))
//===============================================

void pmsm0_board_temperature_feedback(void)
{
    double Rt;
    double temperature;
    Rt = 3.3f / (PMSM0.adc.adc_raw_ave[4] * ADC2TEMP) - 4700.0f;
    // like this R=5000, T2=273.15+25,B=3470, RT=5000*EXP(3470*(1/T1-1/(273.15+25))
    temperature = Rt / Rp;
    temperature = log(temperature);       /* ln(Rt/Rp) */
    temperature /= Bx;             /* ln(Rt/Rp)/B */
    temperature += (1.0f / T2);
    temperature = 1.0f / (temperature);
    temperature -= Ka;
    PMSM0.temperature = temperature;	
}

//===============================================
// calculate speed from encoder
//===============================================

void pmsm0_speed_calculate(void)
{
	double temp;
	temp = PMSM0.encoder_status.encoder_total_count - PMSM0.encoder_status.encoder_total_count_old;
//	PMSM0.encoder_status.encoder_speed = temp * 1000 * 20 * 60.0/ 4000;
	PMSM0.encoder_status.encoder_speed = temp * 50 * 60.0/ 4000;
	PMSM0.encoder_status.encoder_total_count_old = PMSM0.encoder_status.encoder_total_count;
}

//===============================================
// speed from emf zero detect
// Forward/Anticlockwise: 154623
// Backward/Clockwise: 645132
//===============================================

void pmsm0_speed_feedback(void)
{
  float speed_temp = 0;
  float f = 0;
	uint8_t i;

	// speed from emf zero detect
	// half period of electrical degree: PMSM0.emfzero_detect.emfzero_delay_time * 6
	// timer scale: 1/(Timer clk/PMSM0_PWM_PRESCALER_COUNT/PMSM0_PWM_PERIOD_COUNT)
	// timer scale: 1/(200000000/5000/2) = 50us(20kHz)
	// thus, time of half period of electrical degree: (unit:s)
	f = (1.0f / ((200000000.0f / (float)PMSM0_PWM_PRESCALER_COUNT) / 
			(float)PMSM0_PWM_PERIOD_COUNT) * (float)(PMSM0.emfzero_detect.emfzero_delay_time * 6));
	// changing from second to minute: f/60
	// half period of electrical degree of per minute: 1/(f/60)
	// half period of electrical degree is accord with PMSM_POLE
	// one period of electrical degree is accord with PMSM_POLE_PAIR
	// formula: mechanical degree * PMSM_POLE_PAIR = electrical degree
	// one period of mechanical degree of per minute: 1/(f/60)/PMSM_POLE
	// one period of mechanical degree of per minute: 1/(f/60)/8	
	f = (1.0f / (float)PMSM_POLE) / (f  / 60.0f);	
	
	// update speed direction
	if(PMSM0.direction.dir_target == MOTOR_REV)
	{
		f = -f;
	}
	PMSM0.speed.speed_feedback_raw_array[PMSM0.speed.filter_member_count++] = f;
	
	// When the number of raw speed data is more than filter member number, switching speed filter algorithm.
  if (PMSM0.speed.filter_member_count >= SPEED_FILTER_NUM)
  {
    PMSM0.speed.filter_member_count = 0;
  }

  speed_temp = 0;

	// Using average filter
  if (PMSM0.operation_status > MOTOR_VVVF_STAGE)
  {
    for (i = 0; i < SPEED_FILTER_NUM; i++)
    {
      speed_temp += PMSM0.speed.speed_feedback_raw_array[i];
    }
    PMSM0.speed.speed_feedback = speed_temp / (float)SPEED_FILTER_NUM;
  }
  else
  {
    for (i = 0; i < PMSM0.speed.filter_member_count; i++)
    {
      speed_temp += PMSM0.speed.speed_feedback_raw_array[i];
    }
    PMSM0.speed.speed_feedback = speed_temp / PMSM0.speed.filter_member_count;
  }
}

