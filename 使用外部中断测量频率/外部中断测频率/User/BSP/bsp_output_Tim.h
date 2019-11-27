#ifndef _BSP_OUTPUT_TIM_H_
#define _BSP_OUTPUT_TIM_H_

#include "stm32f10x.h"

#define GENERAL_TIMx_OUT									TIM2
#define GENERAL_TIM_OUT_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define GENERAL_TIM_OUT_CLK               RCC_APB1Periph_TIM2

#define GENERAL_TIM_OUT_CHx_GPIO_CLK 			RCC_APB2Periph_GPIOA


#define GENERAL_TIM_OUT_CHx_PIN						GPIO_Pin_2
#define GENERAL_TIM_OUT_CHx_PORT					GPIOA
#define GENERAL_TIM_OUT_CHANNEL_x 				TIM_Channel_3

#define TIM_OC_x_Init											TIM_OC3Init
#define TIM_OC_x_PreloadConfig						TIM_OC3PreloadConfig


void TIM_OutPut_Init(void);





#endif // _BSP_OUTPUT_TIM_H_
