//===============================================
//
//	File: pmsm_bsp.h
//	Author: afterGlow,4ever
//	Date: 06252022
//	Version: v1.0
//
// 	This file is for transplant.
//
//===============================================

#ifndef __PMSM_BSP_H_
#define __PMSM_BSP_H_

#include "stm32h7xx_hal.h"
#include "stm32h7xx.h"
#include "core_delay.h"
//#include "DataScope_DP.h"
#include "design_define.h"

//===============================================
// pwm timer8
//===============================================

#define PMSM0_PWM_TIM           				  TIM1
#define PMSM0_PWM_TIM_CLK_ENABLE()  			__TIM1_CLK_ENABLE()

// u+
#define PMSM0_OCPWM1_PIN           		    GPIO_PIN_9
#define PMSM0_OCPWM1_GPIO_PORT     		    GPIOE
#define PMSM0_OCPWM1_GPIO_CLK_ENABLE() 	  __GPIOE_CLK_ENABLE()
#define PMSM0_OCPWM1_AF					          GPIO_AF1_TIM1

// v+
#define PMSM0_OCPWM2_PIN           		    GPIO_PIN_11
#define PMSM0_OCPWM2_GPIO_PORT     		    GPIOE
#define PMSM0_OCPWM2_GPIO_CLK_ENABLE() 	  __GPIOE_CLK_ENABLE()
#define PMSM0_OCPWM2_AF					          GPIO_AF1_TIM1

// w+
#define PMSM0_OCPWM3_PIN           		    GPIO_PIN_13
#define PMSM0_OCPWM3_GPIO_PORT     		    GPIOE
#define PMSM0_OCPWM3_GPIO_CLK_ENABLE() 	  __GPIOE_CLK_ENABLE()
#define PMSM0_OCPWM3_AF					          GPIO_AF1_TIM1

// u-
#define PMSM0_OCNPWM1_PIN            		  GPIO_PIN_8
#define PMSM0_OCNPWM1_GPIO_PORT      		  GPIOE
#define PMSM0_OCNPWM1_GPIO_CLK_ENABLE()	  __GPIOE_CLK_ENABLE()
#define PMSM0_OCNPWM1_AF					        GPIO_AF1_TIM1

// v-
#define PMSM0_OCNPWM2_PIN            		  GPIO_PIN_10
#define PMSM0_OCNPWM2_GPIO_PORT      		  GPIOE
#define PMSM0_OCNPWM2_GPIO_CLK_ENABLE()	  __GPIOE_CLK_ENABLE()
#define PMSM0_OCNPWM2_AF					        GPIO_AF1_TIM1

// w-
#define PMSM0_OCNPWM3_PIN            		  GPIO_PIN_12
#define PMSM0_OCNPWM3_GPIO_PORT      		  GPIOE
#define PMSM0_OCNPWM3_GPIO_CLK_ENABLE()	  __GPIOE_CLK_ENABLE()
#define PMSM0_OCNPWM3_AF					        GPIO_AF1_TIM1

// int after TIM_Period
// a period is 0~4999
#define PMSM0_PWM_PERIOD_COUNT     				(5000)
#define PMSM0_PWM_MAX_PERIOD_COUNT    		(PWM_PERIOD_COUNT - 1000)

// TIMxCLK=2*PCLK1  
// PCLK1=HCLK/2
// TIMxCLK=HCLK/2=SystemCoreClock/2=200MHz
// set TIMxCLK/(PWM_PRESCALER_COUNT)/PWM_PERIOD_COUNT=20KHz
// period 20kHz 50us
#define PMSM0_PWM_PRESCALER_COUNT     		(2)

// internal trigger TIM8->ITR3->TIM5
#define TIM_COM_TS_ITRx                   TIM_TS_ITR3

#define PMSM0_PWM_CHANNEL1 								TIM_CHANNEL_1
#define PMSM0_PWM_CHANNEL2 								TIM_CHANNEL_2
#define PMSM0_PWM_CHANNEL3 								TIM_CHANNEL_3

//timer int for sensorless process
#define PMSM0_PWM_TIM_IRQn								TIM1_UP_IRQn
#define PMSM0_PWM_TIM_IRQHandler        	TIM1_UP_IRQHandler

//===============================================
// sd
//===============================================

#define PMSM0_SHUTDOWN_PIN                GPIO_PIN_12
#define PMSM0_SHUTDOWN_GPIO_PORT          GPIOC
#define PMSM0_SHUTDOWN_GPIO_CLK_ENABLE()  __GPIOC_CLK_ENABLE()

//===============================================
// zero detect gpio
//===============================================

// u
#define PMSM0_EMFZERO_U_PIN           		    GPIO_PIN_6
#define PMSM0_EMFZERO_U_GPIO_PORT     		    GPIOC
#define PMSM0_EMFZERO_U_GPIO_CLK_ENABLE() 	  __GPIOC_CLK_ENABLE()

// v
#define PMSM0_EMFZERO_V_PIN           		    GPIO_PIN_7
#define PMSM0_EMFZERO_V_GPIO_PORT     		    GPIOC
#define PMSM0_EMFZERO_V_GPIO_CLK_ENABLE() 	  __GPIOC_CLK_ENABLE()

// w
#define PMSM0_EMFZERO_W_PIN           		    GPIO_PIN_8
#define PMSM0_EMFZERO_W_GPIO_PORT     		    GPIOC
#define PMSM0_EMFZERO_W_GPIO_CLK_ENABLE() 	  __GPIOC_CLK_ENABLE()

//===============================================
// encoder
//===============================================

#define PMSM0_ENCODER_TIM           				  TIM4
#define PMSM0_ENCODER_TIM_CLK_ENABLE()  			__TIM4_CLK_ENABLE()
#define PMSM0_ENCODER_CHANNEL1 								TIM_CHANNEL_1
#define PMSM0_ENCODER_CHANNEL2 								TIM_CHANNEL_2

#define PMSM0_ENCODER_TIM_IRQn								TIM4_IRQn
#define PMSM0_ENCODER_TIM_IRQHandler        	TIM4_IRQHandler

// enca
#define PMSM0_ENCODER_ENCA_PIN           		  GPIO_PIN_12
#define PMSM0_ENCODER_ENCA_GPIO_PORT     		 	GPIOD
#define PMSM0_ENCODER_ENCA_GPIO_CLK_ENABLE() 	__GPIOD_CLK_ENABLE()
#define PMSM0_ENCODER_ENCA_AF					        GPIO_AF2_TIM4

// encb
#define PMSM0_ENCODER_ENCB_PIN           		  GPIO_PIN_13
#define PMSM0_ENCODER_ENCB_GPIO_PORT     		  GPIOD
#define PMSM0_ENCODER_ENCB_GPIO_CLK_ENABLE() 	__GPIOD_CLK_ENABLE()
#define PMSM0_ENCODER_ENCB_AF					        GPIO_AF2_TIM4

// int after TIM_Period
// TIMxCLK=2*PCLK1  
// PCLK1=HCLK/2
// TIMxCLK=HCLK/2=SystemCoreClock/2=200MHz
#define PMSM0_ENCODER_PERIOD_COUNT     				(65536)
#define PMSM0_ENCODER_PRESCALER_COUNT     		(1)

//===============================================
// adc & adc dma
//===============================================

#define PMSM0_ADC                         ADC3
#define PMSM0_ADC_CLK_ENABLE()            __HAL_RCC_ADC3_CLK_ENABLE()

//IU
#define PMSM0_ADC_IU_GPIO_PORT            GPIOF
#define PMSM0_ADC_IU_GPIO_PIN             GPIO_PIN_9
#define PMSM0_ADC_IU_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()

//IV
#define PMSM0_ADC_IV_GPIO_PORT            GPIOF
#define PMSM0_ADC_IV_GPIO_PIN             GPIO_PIN_7
#define PMSM0_ADC_IV_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()

//IW
#define PMSM0_ADC_IW_GPIO_PORT            GPIOF
#define PMSM0_ADC_IW_GPIO_PIN             GPIO_PIN_5
#define PMSM0_ADC_IW_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()

//VBUS
#define PMSM0_ADC_VBUS_GPIO_PORT          GPIOF
#define PMSM0_ADC_VBUS_GPIO_PIN           GPIO_PIN_3
#define PMSM0_ADC_VBUS_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()

//VTEMP
#define PMSM0_ADC_VTEMP_GPIO_PORT         GPIOF
#define PMSM0_ADC_VTEMP_GPIO_PIN          GPIO_PIN_10
#define PMSM0_ADC_VTEMP_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()

#define PMSM0_ADC_CHANNEL0                ADC_CHANNEL_2
#define PMSM0_ADC_CHANNEL1                ADC_CHANNEL_3
#define PMSM0_ADC_CHANNEL2                ADC_CHANNEL_4
#define PMSM0_ADC_CHANNEL3                ADC_CHANNEL_5
#define PMSM0_ADC_CHANNEL4                ADC_CHANNEL_6

#define PMSM0_ADC_IRQ                    	ADC3_IRQn
#define PMSM0_ADC_IRQHandler             	ADC3_IRQHandler

#define PMSM0_ADC_CHANNEL_NUM             5
#define PMSM0_ADC_CHANNEL_TIMES           10
//#define PMSM0_ADC_CHANNEL_TOTAL_TIMES     PMSM0_ADC_CHANNEL_NUM * PMSM0_ADC_CHANNEL_TIMES     
#define PMSM0_ADC_CHANNEL_TOTAL_TIMES     50

#define PMSM0_DMA_ADC_CLK_ENABLE()        __HAL_RCC_DMA2_CLK_ENABLE()
#define PMSM0_DMA_ADC_REQUEST 						DMA_REQUEST_ADC3
#define PMSM0_DMA_ADC_Stream              DMA2_Stream0
#define PMSM0_DMA_ADC_IRQn                DMA2_Stream0_IRQn
#define PMSM0_DMA_ADC_IRQHandler          DMA2_Stream0_IRQHandler

//===============================================
// loop timer6
//===============================================

#define PMSM0_LOOP_TIM           		  		TIM6
#define PMSM0_LOOP_TIM_CLK_ENABLE()   		__TIM6_CLK_ENABLE()

#define PMSM0_LOOP_TIM_IRQn				    		TIM6_DAC_IRQn
#define PMSM0_LOOP_TIM_IRQHandler    			TIM6_DAC_IRQHandler

// TIMxCLK=2*PCLK1  
// PCLK1=HCLK/2
// TIMxCLK=HCLK/2=SystemCoreClock/2=200MHz
// set TIMxCLK/(LOOP_PERIOD_COUNT)/LOOP_PRESCALER_COUNT=10Hz
// period 50Hz 20ms
#define PMSM0_LOOP_PERIOD_COUNT    (2000)
#define PMSM0_LOOP_PRESCALER_COUNT   (2000)

#endif
