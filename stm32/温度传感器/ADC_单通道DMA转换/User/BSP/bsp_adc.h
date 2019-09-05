#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_
#include "stm32f10x.h"


#define ADC_x									ADC1
#define ADCx_Channel					ADC_Channel_2
//使用的输入管脚
#define ADCx_GPIO_PIN					GPIO_Pin_2
#define ADCx_GPIO_PORT				GPIOA
//时钟
#define ADCx_GPIO_RCC_FUN 		RCC_APB2PeriphClockCmd
#define ADCx_GPIO_CLK					RCC_APB2Periph_GPIOC
#define ADCx_RCC	 						RCC_APB2Periph_ADC1
//DMA
#define RCC_AHBPeriph_DMA			RCC_AHBPeriph_DMA1
#define RCC_AHBPeriph_DMA_FUN RCC_AHBPeriphClockCmd
#define DMA_Channel  					DMA1_Channel1

void ADCx_TEST_Init(void);
#endif //_BSP_ADC_H_
