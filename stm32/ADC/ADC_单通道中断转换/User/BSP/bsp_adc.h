#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_
#include "stm32f10x.h"


#define ADC_x									ADC1
#define ADCx_Channel					ADC_Channel_12
//使用的输入管脚
#define ADCx_GPIO_PIN					GPIO_Pin_2
#define ADCx_GPIO_PORT				GPIOC
//时钟
#define ADCx_GPIO_RCC_FUN 		RCC_APB2PeriphClockCmd
#define ADCx_GPIO_CLK					RCC_APB2Periph_GPIOC
#define ADCx_RCC	 						RCC_APB2Periph_ADC1
//中断
#define ADCx_IRQChannel				ADC1_2_IRQn
#define ADCx_IRQFUN						ADC1_2_IRQHandler

void ADCx_TEST_Init(void);
#endif //_BSP_ADC_H_
