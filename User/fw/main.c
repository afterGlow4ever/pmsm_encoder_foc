//===============================================
//
//	File: main.c
//	Author: afterGlow,4ever
//	Date: 02212023
//	Version: v1.0
//
// 	This file is including main function.
//
//===============================================

#include "main.h"

pmsm_sensorless_status_t PMSM0;
pid_pos_controller_t pid_pos_controller0;
pid_inc_controller_t pid_inc_controller0;
int Key_Value = 4;

int main(void)
{
	long cnt, temp = 0;
	HAL_Init();
	SystemClock_Config();//400MHz
	HAL_InitTick(0);
	delay_init();
	UART3_BSP_Init(115200);
	LED_BSP_Init();
	KEY_BSP_Init();
	pmsm0_system_init();
	printf("PMSM sensorless test begins.\n\r");	
	pmsm0_system_always_on_enable();
	printf("PMSM sensorless calibration begins.\n\r");	
	pmsm0_system_calibration();
	printf("PMSM sensorless calibration ends.\n\r");		
	LED1(0);
	LED2(0);
	LED3(0);	
	while(1)
	{		
		#if 1
		if(PMSM0.error_status == MOTOR_SENSORLESS_CTRL_ERROR)
		{
			LED1(0);
			LED2(0);
			LED3(1);			
			printf("PMSM disables due to sensorless ctrl error.\n\r");		
		}	
		if(PMSM0.error_status == MOTOR_COMMUTATION_ERROR)
		{
			LED1(0);
			LED2(0);
			LED3(1);	
			printf("PMSM disables due to commutation error.\n\r");		
		}	
		if(PMSM0.error_status == MOTOR_STALL)
		{
			LED1(0);
			LED2(0);
			LED3(1);	
			printf("PMSM disables due to stall.\n\r");		
		}			
		if(PMSM0.error_status == MOTOR_EMFZERO_DETECT_ERROR)
		{
			LED1(0);
			LED2(0);
			LED3(1);	
			printf("PMSM disables due to emf zero detect error.\n\r");		
		}		
		if(PMSM0.error_status == MOTOR_VVVF_ERROR)
		{
			LED1(0);
			LED2(0);
			LED3(1);	
			printf("PMSM disables due to vvvf error.\n\r");		
		}
		if(Key_Value == 2)
		{		
			if((PMSM0.enable_status == MOTOR_DISABLE) && (PMSM0.operation_status == MOTOR_CALIBRATION_DONE))
			{	
				PMSM0.direction.dir_target = MOTOR_FWD;
				pmsm0_system_calibration();
				printf("PMSM is starting.\n\r");						
				pmsm0_system_enable();
				LED1(1);
				LED2(0);
				LED3(0);			
			}
			else
			{
				printf("PMSM is braking.\n\r");					
				pmsm0_system_disable();	
				LED1(0);
				LED2(0);
				LED3(0);
				printf("PMSM disabled.\n\r");			
				pmsm0_system_calibration();
			}
			Key_Value = 4;	
		}
//#ifdef EMFZERO_DETECT
//		if(Key_Value == 1)
//		{
//			pmsm0_speed_up();
//			printf("PMSM speed:%f.\n\r", PMSM0.speed.speed_target);			
//			Key_Value = 4;	
//		}					
//		if(Key_Value == 2)
//		{								
//			pmsm0_speed_down();
//			printf("PMSM speed:%f.\n\r", PMSM0.speed.speed_target);			
//			Key_Value = 4;	
//		}		
//#endif		
//		if(Key_Value == 3)
//		{
//			if((PMSM0.enable_status == MOTOR_DISABLE) && (PMSM0.operation_status == MOTOR_CALIBRATION_DONE))
//			{
//				PMSM0.direction.dir_target = MOTOR_REV;
//				printf("PMSM is starting.\n\r");		
//				pmsm0_system_enable();
//				LED0(0);
//				LED1(1);			
//			}
//			else
//			{
//				printf("PMSM is braking.\n\r");		
//				pmsm0_system_disable();	
//				LED0(0);
//				LED1(0);
//				printf("PMSM disabled.\n\r");			
//			}
//			Key_Value = 4;	
//		}

		if(PMSM0.operation_status == MOTOR_DRIVING)
		{
			LED2(1);			
		}
		#endif
		if(cnt == 1000000)
		{
			pmsm0_vbus_feedback();
			pmsm0_board_temperature_feedback();
			cnt = 0;	
//			printf("%d.\r\n", __HAL_TIM_GET_COUNTER(&PMSM0.TIM_ENCODER_InitStructure));		
			printf("ESpeed=%0.1fRPM.\r\n", PMSM0.encoder_status.encoder_speed);
//			printf("%d\r\n", PMSM0.encoder_status.encoder_elapse_count);
//			printf("%d\r\n", PMSM0.encoder_status.encoder_current_count);
//			printf("%d\r\n", PMSM0.encoder_status.encoder_total_count);
//			printf("%d\r\n", PMSM0.encoder_status.encoder_total_count_old);
			printf("CSpeed=%0.1fRPM.\r\n", PMSM0.speed.speed_feedback);
			printf("IU=%f.\r\n", PMSM0.phase_current.u_current * 1000);
			printf("IV=%f.\r\n", PMSM0.phase_current.v_current * 1000);
			printf("IW=%f.\r\n", PMSM0.phase_current.w_current * 1000);	
//			printf("t=%d.\r\n", PMSM0.adc.adc_calibration_times);
			printf("RU=%d.\r\n", PMSM0.adc.adc_raw_reference[0]);
			printf("RV=%d.\r\n", PMSM0.adc.adc_raw_reference[1]);
			printf("RW=%d.\r\n", PMSM0.adc.adc_raw_reference[2]);		
			printf("AU=%d.\r\n", PMSM0.adc.adc_raw_ave[0]);
			printf("AV=%d.\r\n", PMSM0.adc.adc_raw_ave[1]);
			printf("AW=%d.\r\n", PMSM0.adc.adc_raw_ave[2]);					
			printf("DU=%d.\r\n", (int)PMSM0.adc.adc_raw_reference[0] - (int)PMSM0.adc.adc_raw_ave[0]);
			printf("DV=%d.\r\n", (int)PMSM0.adc.adc_raw_reference[1] - (int)PMSM0.adc.adc_raw_ave[1]);
			printf("DW=%d.\r\n", (int)PMSM0.adc.adc_raw_reference[2] - (int)PMSM0.adc.adc_raw_ave[2]);				
//			printf("R=%d.\r\n", PMSM0.adc.adc_raw_reference[3]);						
			printf("VBUS=%0.01f.\r\n", PMSM0.vbus);
			printf("TEMP=%0.01f.\r\n", PMSM0.temperature);	
//			printf("4=%d.\r\n", PMSM0.adc.adc_raw_ave[3]);
//			printf("5=%d.\r\n", PMSM0.adc.adc_raw_ave[4]);	
//			printf("Status=%d.\r\n", PMSM0.operation_status);			
		}
		else
			cnt++;

	}		
}

/**
  * @brief  System Clock 配置
  *         system Clock 配置如下: 
	*            System Clock source  = PLL (HSE)
	*            SYSCLK(Hz)           = 400000000 (CPU Clock)
	*            HCLK(Hz)             = 200000000 (AXI and AHBs Clock)
	*            AHB Prescaler        = 2
	*            D1 APB3 Prescaler    = 2 (APB3 Clock  100MHz)
	*            D2 APB1 Prescaler    = 2 (APB1 Clock  100MHz)
	*            D2 APB2 Prescaler    = 2 (APB2 Clock  100MHz)
	*            D3 APB4 Prescaler    = 2 (APB4 Clock  100MHz)
	*            HSE Frequency(Hz)    = 25000000
	*            PLL_M                = 5
	*            PLL_N                = 160
	*            PLL_P                = 2
	*            PLL_Q                = 4
	*            PLL_R                = 2
	*            VDD(V)               = 3.3
	*            Flash Latency(WS)    = 4
  * @param  None
  * @retval None
  */
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
//	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** 启用电源配置更新
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** 配置主内稳压器输出电压
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** 初始化CPU、AHB和APB总线时钟
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;		
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
		while(1);
  }
  /** 初始化CPU、AHB和APB总线时钟
  */
	/* 选择PLL作为系统时钟源并配置总线时钟分频器 */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK  | \
																 RCC_CLOCKTYPE_HCLK    | \
																 RCC_CLOCKTYPE_D1PCLK1 | \
																 RCC_CLOCKTYPE_PCLK1   | \
                                 RCC_CLOCKTYPE_PCLK2   | \
																 RCC_CLOCKTYPE_D3PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;  
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2; 
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2; 
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2; 
//  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    while(1) { ; }
  }
//	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
//  PeriphClkInitStruct.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL;
//  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
//  {
//		while(1);
//  }
}
/****************************END OF FILE***************************/

