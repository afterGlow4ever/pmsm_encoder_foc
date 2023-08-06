//===============================================
//
//	File: adc.c
//	Author: afterGlow,4ever
//	Date: 04012023
//	Version: v1.0
//
// 	This file is related to adc.
//
//===============================================

#include "pmsm.h"

extern pmsm_sensorless_status_t PMSM0;
uint32_t adc_raw[PMSM0_ADC_CHANNEL_TOTAL_TIMES];
uint8_t adc_calibration_update_flag = 0;

//===============================================
// adc gpio init
//===============================================

static void pmsm0_adc_gpio_init(void) 
{
  PMSM0_ADC_IU_GPIO_CLK_ENABLE();
  PMSM0_ADC_IV_GPIO_CLK_ENABLE();
	PMSM0_ADC_IW_GPIO_CLK_ENABLE();
	PMSM0_ADC_VBUS_GPIO_CLK_ENABLE();
	PMSM0_ADC_VTEMP_GPIO_CLK_ENABLE();
	
	PMSM0.GPIO_ADC_InitStructure.Pull = GPIO_NOPULL;
  PMSM0.GPIO_ADC_InitStructure.Mode = GPIO_MODE_ANALOG; 

	// iu
  PMSM0.GPIO_ADC_InitStructure.Pin = PMSM0_ADC_IU_GPIO_PIN;
  HAL_GPIO_Init(PMSM0_ADC_IU_GPIO_PORT, &PMSM0.GPIO_ADC_InitStructure);	
	// iv
  PMSM0.GPIO_ADC_InitStructure.Pin = PMSM0_ADC_IV_GPIO_PIN;
  HAL_GPIO_Init(PMSM0_ADC_IV_GPIO_PORT, &PMSM0.GPIO_ADC_InitStructure);	
	// iw
  PMSM0.GPIO_ADC_InitStructure.Pin = PMSM0_ADC_IW_GPIO_PIN;
  HAL_GPIO_Init(PMSM0_ADC_IW_GPIO_PORT, &PMSM0.GPIO_ADC_InitStructure);	
	// vbus
  PMSM0.GPIO_ADC_InitStructure.Pin = PMSM0_ADC_VBUS_GPIO_PIN;
  HAL_GPIO_Init(PMSM0_ADC_VBUS_GPIO_PORT, &PMSM0.GPIO_ADC_InitStructure);	
	// vtemp
  PMSM0.GPIO_ADC_InitStructure.Pin = PMSM0_ADC_VTEMP_GPIO_PIN;
  HAL_GPIO_Init(PMSM0_ADC_VTEMP_GPIO_PORT, &PMSM0.GPIO_ADC_InitStructure);		
}

//===============================================
// adc init with dma mode
//===============================================

static void pmsm0_adc_init(void)
{
	PMSM0_ADC_CLK_ENABLE();
	
	PMSM0.ADC_InitStructure.Instance = PMSM0_ADC;
	PMSM0.ADC_InitStructure.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;//50MHz
	PMSM0.ADC_InitStructure.Init.Resolution = ADC_RESOLUTION_16B;
	PMSM0.ADC_InitStructure.Init.ScanConvMode = ADC_SCAN_ENABLE;
	PMSM0.ADC_InitStructure.Init.ContinuousConvMode = ENABLE;
	PMSM0.ADC_InitStructure.Init.DiscontinuousConvMode = DISABLE;
	PMSM0.ADC_InitStructure.Init.NbrOfDiscConversion   = 0;
	PMSM0.ADC_InitStructure.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	PMSM0.ADC_InitStructure.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	PMSM0.ADC_InitStructure.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
	PMSM0.ADC_InitStructure.Init.NbrOfConversion = PMSM0_ADC_CHANNEL_NUM;
	PMSM0.ADC_InitStructure.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_ONESHOT;//dma
	PMSM0.ADC_InitStructure.Init.OversamplingMode = DISABLE;
	PMSM0.ADC_InitStructure.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	PMSM0.ADC_InitStructure.Init.EOCSelection = ADC_EOC_SINGLE_CONV;	
	HAL_ADC_Init(&PMSM0.ADC_InitStructure);

	// Calibration
	if (HAL_ADCEx_Calibration_Start(&PMSM0.ADC_InitStructure, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
	{
		while(1);
	}	
	
	PMSM0.ADC_CHInitStructure.Channel = PMSM0_ADC_CHANNEL0;
	PMSM0.ADC_CHInitStructure.Rank = ADC_REGULAR_RANK_1;
	PMSM0.ADC_CHInitStructure.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
	PMSM0.ADC_CHInitStructure.SingleDiff = ADC_SINGLE_ENDED;	
  PMSM0.ADC_CHInitStructure.Offset = 0;
	PMSM0.ADC_CHInitStructure.OffsetNumber = ADC_OFFSET_NONE;
	PMSM0.ADC_CHInitStructure.OffsetRightShift = DISABLE;
	PMSM0.ADC_CHInitStructure.OffsetSignedSaturation = DISABLE;	
	HAL_ADC_ConfigChannel(&PMSM0.ADC_InitStructure, &PMSM0.ADC_CHInitStructure);

	PMSM0.ADC_CHInitStructure.Channel = PMSM0_ADC_CHANNEL1;
	PMSM0.ADC_CHInitStructure.Rank = ADC_REGULAR_RANK_2;
	PMSM0.ADC_CHInitStructure.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
	PMSM0.ADC_CHInitStructure.SingleDiff = ADC_SINGLE_ENDED;	
  PMSM0.ADC_CHInitStructure.Offset = 0;
	PMSM0.ADC_CHInitStructure.OffsetNumber = ADC_OFFSET_NONE;
	PMSM0.ADC_CHInitStructure.OffsetRightShift = DISABLE;
	PMSM0.ADC_CHInitStructure.OffsetSignedSaturation = DISABLE;	
	HAL_ADC_ConfigChannel(&PMSM0.ADC_InitStructure, &PMSM0.ADC_CHInitStructure);

	PMSM0.ADC_CHInitStructure.Channel = PMSM0_ADC_CHANNEL2;
	PMSM0.ADC_CHInitStructure.Rank = ADC_REGULAR_RANK_3;
	PMSM0.ADC_CHInitStructure.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
	PMSM0.ADC_CHInitStructure.SingleDiff = ADC_SINGLE_ENDED;	
  PMSM0.ADC_CHInitStructure.Offset = 0;
	PMSM0.ADC_CHInitStructure.OffsetNumber = ADC_OFFSET_NONE;
	PMSM0.ADC_CHInitStructure.OffsetRightShift = DISABLE;
	PMSM0.ADC_CHInitStructure.OffsetSignedSaturation = DISABLE;	
	HAL_ADC_ConfigChannel(&PMSM0.ADC_InitStructure, &PMSM0.ADC_CHInitStructure);
	
	PMSM0.ADC_CHInitStructure.Channel = PMSM0_ADC_CHANNEL3;
	PMSM0.ADC_CHInitStructure.Rank = ADC_REGULAR_RANK_4;
	PMSM0.ADC_CHInitStructure.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
	PMSM0.ADC_CHInitStructure.SingleDiff = ADC_SINGLE_ENDED;	
  PMSM0.ADC_CHInitStructure.Offset = 0;
	PMSM0.ADC_CHInitStructure.OffsetNumber = ADC_OFFSET_NONE;
	PMSM0.ADC_CHInitStructure.OffsetRightShift = DISABLE;
	PMSM0.ADC_CHInitStructure.OffsetSignedSaturation = DISABLE;	
	HAL_ADC_ConfigChannel(&PMSM0.ADC_InitStructure, &PMSM0.ADC_CHInitStructure);
	
	PMSM0.ADC_CHInitStructure.Channel = PMSM0_ADC_CHANNEL4;
	PMSM0.ADC_CHInitStructure.Rank = ADC_REGULAR_RANK_5;
	PMSM0.ADC_CHInitStructure.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
	PMSM0.ADC_CHInitStructure.SingleDiff = ADC_SINGLE_ENDED;	
  PMSM0.ADC_CHInitStructure.Offset = 0;
	PMSM0.ADC_CHInitStructure.OffsetNumber = ADC_OFFSET_NONE;
	PMSM0.ADC_CHInitStructure.OffsetRightShift = DISABLE;
	PMSM0.ADC_CHInitStructure.OffsetSignedSaturation = DISABLE;	
	HAL_ADC_ConfigChannel(&PMSM0.ADC_InitStructure, &PMSM0.ADC_CHInitStructure);
	HAL_NVIC_SetPriority(PMSM0_DMA_ADC_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ(PMSM0_DMA_ADC_IRQn);	
	HAL_NVIC_SetPriority(PMSM0_ADC_IRQ, 0, 2);
	HAL_NVIC_EnableIRQ(PMSM0_ADC_IRQ);	

}

//===============================================
// adc init
//===============================================

void pmsm0_analog_init(void)
{
	pmsm0_adc_gpio_init();
	pmsm0_adc_init();
}

//===============================================
// adc data preporcess
//===============================================

static void pmsm0_adc_data_preprocess(void)
{
	uint32_t raw_data[5];
	uint8_t i, j;

	for(j = 0; j < PMSM0_ADC_CHANNEL_NUM; j++)
	{
		raw_data[j] = 0;
	}			
	for(i = 0; i < PMSM0_ADC_CHANNEL_NUM; i++)
	{
		for(j = i; j < PMSM0_ADC_CHANNEL_TOTAL_TIMES; j = j + PMSM0_ADC_CHANNEL_NUM)
		{
			raw_data[i] += adc_raw[j];
		}
	}
	for(j = 0; j < PMSM0_ADC_CHANNEL_NUM; j++)
	{
		PMSM0.adc.adc_raw_ave[j] = raw_data[j] / PMSM0_ADC_CHANNEL_TIMES;
	}		
	adc_calibration_update_flag = 1;
}

//===============================================
// adc callback
//===============================================

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc == &PMSM0.ADC_InitStructure)
	{	
//		HAL_ADC_Stop_DMA(&PMSM0.ADC_InitStructure);			
		pmsm0_adc_data_preprocess();
		HAL_ADC_Start_DMA(&PMSM0.ADC_InitStructure, (uint32_t*)adc_raw, PMSM0_ADC_CHANNEL_TOTAL_TIMES);	
	}		
}

//===============================================
// adc int handler
//===============================================

void PMSM0_ADC_IRQHandler()
{
	HAL_ADC_IRQHandler(&PMSM0.ADC_InitStructure);
}
