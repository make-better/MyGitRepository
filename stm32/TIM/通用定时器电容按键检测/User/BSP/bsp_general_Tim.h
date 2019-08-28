#ifndef _BSP_GENERAL_TIM_H_
#define _BSP_GENERAL_TIM_H_

#include "stm32f10x.h"


#define GENERAL_TIMx									TIM5
#define GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define GENERAL_TIM_CLK               RCC_APB1Periph_TIM5

#define GENERAL_TIM_CH2_GPIO_CLK 			RCC_APB2Periph_GPIOA 


#define GENERAL_TIM_CH2_PIN						GPIO_Pin_1
#define GENERAL_TIM_CH2_PORT					GPIOA
#define GENERAL_TIM_CHANNEL_x 				TIM_Channel_2

 // 中断相关宏定义
#define GENERAL_TIM_IT_CCx						TIM_IT_CC2
#define GENERAL_TIM_IRQ 							TIM5_IRQn
#define GENERAL_TIM_INT_FUN 					TIM5_IRQHandler
// 获取捕获寄存器值函数宏定义
#define GENERAL_TIM_GetCapturex_FUN 	TIM_GetCapture2
// 捕获信号极性函数宏定义
#define GENERAL_TIM_OCxPolarityConfig_FUN TIM_OC2PolarityConfig
// 测量的起始边沿
#define GENERAL_TIM_STRAT_ICPolarity 	TIM_ICPolarity_Rising



void TIM_Init(void);





#endif // _BSP_GENERAL_TIM_H_
