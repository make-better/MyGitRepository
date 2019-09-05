#ifndef _BSP_ADVANCE_TIM_H_
#define _BSP_ADVANCE_TIM_H_
#include "stm32f10x.h"


#define ADVANCE_TIM_CH1_GPIO_CLK 				RCC_APB2Periph_GPIOA
#define ADVANCE_TIM_CH1_PIN 						GPIO_Pin_8
#define ADVANCE_TIM_CH1_PORT 						GPIOA
#define ADVANCE_TIM_CHANNEL_x 					TIM_Channel_1
							
#define ADVANCE_TIM											TIM1
#define ADVANCE_TIM_APBxClock_FUN 			RCC_APB2PeriphClockCmd
#define ADVANCE_TIM_CLK 								RCC_APB2Periph_TIM1
	// PWM 信号的频率 F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define ADVANCE_TIM_PERIOD 							(1000-1)//周期
#define ADVANCE_TIM_PSC 								(72-1)

#define ADVANCE_TIM_IT_CCx              TIM_IT_CC1
#define ADVANCE_TIM_IRQ 								TIM1_CC_IRQn
#define ADVANCE_TIM_IRQHandler 					TIM1_CC_IRQHandler


void Advance_TIM_Init(void);

#endif //_BSP_ADVANCE_TIM_H_
