//===============================================
//
//	File: pmsm_drive.c
//	Author: afterGlow,4ever
//	Date: 06252022
//	Version: v1.0
//
// 	This file is used to operate actutors in the loop. 
//
//===============================================

#include "pmsm.h"

extern pmsm_sensorless_status_t PMSM0;

//===============================================
// pmsm driving board sd init
//===============================================

void pmsm0_sd_init(void)
{
	PMSM0_SHUTDOWN_GPIO_CLK_ENABLE();
  PMSM0.GPIO_SD_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  PMSM0.GPIO_SD_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	PMSM0.GPIO_SD_InitStructure.Pin = PMSM0_SHUTDOWN_PIN;
  HAL_GPIO_Init(PMSM0_SHUTDOWN_GPIO_PORT, &PMSM0.GPIO_SD_InitStructure);
}

//===============================================
// pmsm driving board sd
//===============================================

void pmsm0_sd_enable(void)
{
	HAL_GPIO_WritePin(PMSM0_SHUTDOWN_GPIO_PORT, PMSM0_SHUTDOWN_PIN, GPIO_PIN_SET);  
}    
                
void pmsm0_sd_disable(void)
{
	HAL_GPIO_WritePin(PMSM0_SHUTDOWN_GPIO_PORT, PMSM0_SHUTDOWN_PIN, GPIO_PIN_RESET);  
}                

//===============================================
// pmsm system actutor init
//===============================================

void pmsm0_actutor_init(void)
{
	pmsm0_pwm_init();
	pmsm0_sd_init();	
}

//===============================================
// pmsm system actutor switch
//===============================================

void pmsm0_actutor_enable(void)
{
	pmsm0_sd_enable();
	pmsm0_pwm_enable();
}

void pmsm0_actutor_disable(void)
{
	pmsm0_sd_disable();
	pmsm0_pwm_disable();
}

//===============================================
// pmsm system actutor speed set
//===============================================

void pmsm0_actutor_speed_set(void)
{	
	float speed_temp;
	speed_temp = PMSM0.speed.speed_set;
	if(speed_temp < 0)	
		speed_temp = - speed_temp;
	pmsm0_pwm_set(speed_temp);
}

void pmsm0_actutor_dir_set(pmsm_dir_e dir_temp)
{
	pmsm0_dir_set(dir_temp);
}
