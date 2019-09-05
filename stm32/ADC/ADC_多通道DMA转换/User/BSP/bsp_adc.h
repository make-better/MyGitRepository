#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_
#include "stm32f10x.h"


#define ADC_x									ADC1
#define ADCx_Channel1					ADC_Channel_10
#define ADCx_Channel2					ADC_Channel_11
#define ADCx_Channel3					ADC_Channel_13
#define ADCx_Channel4					ADC_Channel_14
#define ADCx_Channel5					ADC_Channel_15
//使用的输入管脚
#define ADC1_GPIO_PIN					GPIO_Pin_0
#define ADC2_GPIO_PIN					GPIO_Pin_1
#define ADC3_GPIO_PIN					GPIO_Pin_3
#define ADC4_GPIO_PIN					GPIO_Pin_4
#define ADC5_GPIO_PIN					GPIO_Pin_5
#define ADCx_GPIO_PORT				GPIOC

#define NumOfAdc 							5
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
