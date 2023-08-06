//===============================================
//
//	File: sensorless_ctrl.c
//	Author: afterGlow,4ever
//	Date: 02202023
//	Version: v1.0
//
// 	This file is related to pmsm sensorless operation
//  including vvvf stage and normal operation. 
//
//===============================================

#include "pmsm.h"

extern pmsm_sensorless_status_t PMSM0;

//===============================================
// sensorless control loop
// comment: hall 1762 emfzero 2.04v emf 500mv
//===============================================

void pmsm_sensorless_top(void)
{
	uint8_t temp = 0;
	if(PMSM0.enable_status == MOTOR_ENABLE)
	{
		switch(PMSM0.operation_status)
		{
			case MOTOR_VVVF_STAGE:			
			{			
				temp = pmsm0_vvvf_top();	
#ifdef EMFZERO_DETECT				
				// if accelerating successful, starting to emf zero detect	
				if(temp == 1)
				{
					PMSM0.operation_status = MOTOR_EMFZERO_DETECT_STAGE;
				}
			}	
			break;
			case MOTOR_EMFZERO_DETECT_STAGE:			
			{			
				temp = pmsm0_vvvf_top();		
				// if accelerating successful, starting to emf zero detect				
				if(temp == 0)
				{				
					temp = pmsm0_emfzero_detect_top();
					if(temp == 2)
					{
						PMSM0.vvvf.vvvf_status = VVVF_OK;
						PMSM0.operation_status = MOTOR_DRIVING;					
					}									
				}
			}	
			break;			
			case MOTOR_DRIVING:			
			{			
				pmsm0_emfzero_detect_top();
#endif				
			}			
			break;				
			default:
			{
				pmsm0_system_disable();
				PMSM0.error_status = MOTOR_SENSORLESS_CTRL_ERROR;		
			}				
		}
	}
}

