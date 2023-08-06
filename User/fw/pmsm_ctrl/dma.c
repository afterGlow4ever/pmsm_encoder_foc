//===============================================
//
//	File: dma.c
//	Author: afterGlow,4ever
//	Date: 04012023
//	Version: v1.0
//
// 	This file is related to adc dma.
//
//===============================================

#include "pmsm.h"

extern pmsm_sensorless_status_t PMSM0;
extern uint32_t adc_raw[PMSM0_ADC_CHANNEL_TOTAL_TIMES];

//===============================================
// dma init
//===============================================

void pmsm0_dma_init(void)
{
	PMSM0_DMA_ADC_CLK_ENABLE();
	
	PMSM0.DMA_ADC_InitStructure.Instance = PMSM0_DMA_ADC_Stream;
	PMSM0.DMA_ADC_InitStructure.Init.Direction = DMA_PERIPH_TO_MEMORY;
	PMSM0.DMA_ADC_InitStructure.Init.PeriphInc = DMA_PINC_DISABLE;
	PMSM0.DMA_ADC_InitStructure.Init.MemInc = DMA_MINC_ENABLE;
	PMSM0.DMA_ADC_InitStructure.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	PMSM0.DMA_ADC_InitStructure.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	PMSM0.DMA_ADC_InitStructure.Init.Mode = DMA_NORMAL;
	PMSM0.DMA_ADC_InitStructure.Init.Priority = DMA_PRIORITY_HIGH;
  PMSM0.DMA_ADC_InitStructure.Init.FIFOMode = DMA_FIFOMODE_DISABLE;  
  PMSM0.DMA_ADC_InitStructure.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
  PMSM0.DMA_ADC_InitStructure.Init.MemBurst = DMA_MBURST_SINGLE;
  PMSM0.DMA_ADC_InitStructure.Init.PeriphBurst = DMA_PBURST_SINGLE;  
  PMSM0.DMA_ADC_InitStructure.Init.Request = PMSM0_DMA_ADC_REQUEST; 	
	
	HAL_DMA_Init(&PMSM0.DMA_ADC_InitStructure);

	__HAL_LINKDMA(&PMSM0.ADC_InitStructure, DMA_Handle, PMSM0.DMA_ADC_InitStructure);
}

//===============================================
// dma switch
//===============================================

void pmsm0_dma_adc_enable(void)
{
	HAL_ADC_Start_DMA(&PMSM0.ADC_InitStructure, (uint32_t*)adc_raw, PMSM0_ADC_CHANNEL_TOTAL_TIMES);	
}

void pmsm0_dma_adc_stop(void)
{
	HAL_ADC_Stop_DMA(&PMSM0.ADC_InitStructure);		
}

//===============================================
// dma int handler
//===============================================

void PMSM0_DMA_ADC_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&PMSM0.DMA_ADC_InitStructure);
}
