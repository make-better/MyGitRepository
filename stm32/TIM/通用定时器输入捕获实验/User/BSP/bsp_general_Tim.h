#ifndef _BSP_GENERAL_TIM_H_
#define _BSP_GENERAL_TIM_H_

#include "stm32f10x.h"


typedef struct {
	uint8_t Capture_FinishFlag; // 捕获结束标志位
	uint8_t Capture_StartFlag; // 捕获开始标志位
	uint16_t Capture_CcrValue; // 捕获寄存器的值
	uint16_t Capture_Period; // 自动重装载寄存器更新标志
}TIM_ICUserValueType;


#define GENERAL_TIMx									TIM5
#define GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define GENERAL_TIM_CLK               RCC_APB1Periph_TIM5

#define GENERAL_TIM_CH1_GPIO_CLK 			RCC_APB2Periph_GPIOA 


#define GENERAL_TIM_CH1_PIN						GPIO_Pin_0
#define GENERAL_TIM_CH1_PORT					GPIOA
#define GENERAL_TIM_CHANNEL_x 				TIM_Channel_1

 // 中断相关宏定义
#define GENERAL_TIM_IT_CCx						TIM_IT_CC1
#define GENERAL_TIM_IRQ 							TIM5_IRQn
#define GENERAL_TIM_INT_FUN 					TIM5_IRQHandler
// 获取捕获寄存器值函数宏定义
#define GENERAL_TIM_GetCapturex_FUN 	TIM_GetCapture1
// 捕获信号极性函数宏定义
#define GENERAL_TIM_OCxPolarityConfig_FUN TIM_OC1PolarityConfig
// 测量的起始边沿
#define GENERAL_TIM_STRAT_ICPolarity 	TIM_ICPolarity_Rising
// 测量的结束边沿
#define GENERAL_TIM_END_ICPolarity 		TIM_ICPolarity_Falling

void TIM_Init(void);





#endif // _BSP_GENERAL_TIM_H_
