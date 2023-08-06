//=======================================================================================


//=======================================================================================

#include "usart_bsp.h"

//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
uint8_t USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
uint16_t USART_RX_STA=0;       //接收状态标记	
uint8_t aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲

UART_HandleTypeDef UART3_Handle;

void UART3_BSP_Init(uint32_t BoundRate)
{
	UART3_Handle.Instance = USART3;
	UART3_Handle.Init.BaudRate = BoundRate;
	UART3_Handle.Init.WordLength = UART_WORDLENGTH_8B;
	UART3_Handle.Init.StopBits = UART_STOPBITS_1;
	UART3_Handle.Init.Parity = UART_PARITY_NONE;
	UART3_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UART3_Handle.Init.Mode = UART_MODE_TX_RX;
	UART3_Handle.Init.OverSampling = UART_OVERSAMPLING_16;
	UART3_Handle.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;
	UART3_Handle.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	UART3_Handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	
	HAL_UART_Init(&UART3_Handle);
	
	HAL_NVIC_SetPriority(USART3_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART3_IRQn);
	
	__HAL_UART_ENABLE_IT(&UART3_Handle, UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(&UART3_Handle, UART_IT_RXFF);
	__HAL_UART_ENABLE_IT(&UART3_Handle, UART_IT_RXFNE);	
}

void HAL_UART_MspInit(UART_HandleTypeDef *UART_Handle)
{
	GPIO_InitTypeDef GPIO_Initure;
	if(UART_Handle -> Instance == USART3)
	{
		__HAL_RCC_GPIOD_CLK_ENABLE();
		__HAL_RCC_USART3_CLK_ENABLE();
		
		GPIO_Initure.Pin = GPIO_PIN_8;
		GPIO_Initure.Mode = GPIO_MODE_AF_PP;
		GPIO_Initure.Pull = GPIO_NOPULL;
		GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_Initure.Alternate=GPIO_AF7_USART3;
		HAL_GPIO_Init(GPIOD, &GPIO_Initure);		
		
		GPIO_Initure.Pin = GPIO_PIN_9;
		GPIO_Initure.Mode = GPIO_MODE_AF_PP;
		GPIO_Initure.Pull = GPIO_NOPULL;
		GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_Initure.Alternate=GPIO_AF7_USART3;
		HAL_GPIO_Init(GPIOD, &GPIO_Initure);			
	}
}



struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
//	while((USART1->ISR&0X40)==0);//循环发送,直到发送完毕   
//	USART1->TDR=(uint8_t)ch;      
//	return ch;
	/* 发送一个字节数据到串口DEBUG_USART */
	HAL_UART_Transmit(&UART3_Handle, (uint8_t *)&ch, 1, 500);	
	
	return (ch);	
}

int fgetc(FILE *f)
{
		
	int ch;
	HAL_UART_Receive(&UART3_Handle, (uint8_t *)&ch, 1, 0xffff);	
	return (ch);
}

void USART3_IRQHandler(void)                	
{ 
  if(__HAL_UART_GET_IT_SOURCE(&UART3_Handle, UART_IT_RXNE) != RESET)
	{  
    __HAL_UART_CLEAR_FLAG(&UART3_Handle, UART_IT_RXNE);
  }
	HAL_UART_IRQHandler(&UART3_Handle);	
} 

/*****************  发送字符 **********************/
void Usart_SendByte(uint8_t str)
{
  HAL_UART_Transmit(&UART3_Handle, &str, 1, 1000);
}

/*****************  发送字符串 **********************/
void Usart_SendString(uint8_t *str)
{
	unsigned int k=0;
  do 
  {
      HAL_UART_Transmit(&UART3_Handle,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');
}

