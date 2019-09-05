#ifndef _BSP_BASIC_TIM_H_
#define _BSP_BASIC_TIM_H_

#include "stm32f10x.h"

#define NVIC_TIM_Channel    TIM6_IRQn

#define BASIC_TIM								TIM6
#define BASIC_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define BASIC_TIM_CLK               RCC_APB1Periph_TIM6





void Basic_Tim_Init(void);


#endif // _BSP_BASIC_TIM_H_
