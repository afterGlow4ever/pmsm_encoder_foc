//=======================================================================================


//=======================================================================================

#include "stm32h7xx.h"
#include "stdio.h"

#ifndef _USART_BSP_
#define _USART_BSP_

#define USART_REC_LEN  			100  	//�����������ֽ��� 200
#define EN_USART3_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern uint8_t  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uint16_t USART_RX_STA;         		//����״̬���	

#define RXBUFFERSIZE   1 //�����С
extern uint8_t aRxBuffer[RXBUFFERSIZE];//HAL��USART����Buffer
extern UART_HandleTypeDef UART3_Handle;

void UART3_BSP_Init(uint32_t BoundRate);
void HAL_UART_MspInit(UART_HandleTypeDef *UART_Handle);


int fputc(int ch, FILE *f);
int fgetc(FILE *f);

	
#endif
