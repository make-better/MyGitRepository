#ifndef _BSP_GENERAL2_TIM_H_
#define _BSP_GENERAL2_TIM_H_

#include "stm32f10x.h"

#define GENERAL_TIMx_OUT									TIM4
#define GENERAL_TIM_OUT_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define GENERAL_TIM_OUT_CLK               RCC_APB1Periph_TIM4

#define GENERAL_TIM_OUT_CHx_GPIO_CLK 			RCC_APB2Periph_GPIOB


#define GENERAL_TIM_OUT_CHx_PIN						GPIO_Pin_6
#define GENERAL_TIM_OUT_CHx_PORT					GPIOB
#define GENERAL_TIM_OUT_CHANNEL_x 				TIM_Channel_1


void TIM_OutPut_Init(void);





#endif // _BSP_GENERAL_TIM_H_
