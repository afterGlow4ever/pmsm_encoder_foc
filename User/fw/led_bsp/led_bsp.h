//=======================================================================================


//=======================================================================================

#include "stm32h7xx.h"

#ifndef _LED_BSP_
#define _LED_BSP_

#define LED1(n)	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, (n ? GPIO_PIN_SET : GPIO_PIN_RESET))
#define LED1_Toggle HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0)
#define LED2(n)	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, (n ? GPIO_PIN_SET : GPIO_PIN_RESET))
#define LED2_Toggle HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1)
#define LED3(n)	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, (n ? GPIO_PIN_SET : GPIO_PIN_RESET))
#define LED3_Toggle HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14)

//=========================================

void LED_BSP_Init(void);

#endif
