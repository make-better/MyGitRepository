#ifndef _BSP_DMA_MTP_H_
#define _BSP_DMA_MTP_H_
#include "stm32f10x.h"
#include <stdio.h>


// 串口1-USART1
#define  DEBUG_USARTx                   USART1
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10

#define  DEBUG_USART_IRQ                USART1_IRQn
#define  DEBUG_USART_IRQHandler         USART1_IRQHandler


//设置数组大小
#define BUFFER_SIZE    	 		500
//设置DMA
#define RCC_AHBPeriph_DMA   RCC_AHBPeriph_DMA1
#define DMA_Channel         DMA1_Channel4
#define DMA_FLAG_TC			    DMA1_FLAG_TC4
//设置USART
#define USART_DR_ADDRESS        (USART1_BASE+0x04)


void DMA_Configer(void);
void USART_Config(void);
int fputc(int ch, FILE *f);



#endif //_BSP_DMA_MTP_H_