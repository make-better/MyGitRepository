#ifndef _BSP_GENERAL_TIM_H_
#define _BSP_GENERAL_TIM_H_

#include "stm32f10x.h"


typedef struct {
	uint8_t Capture_FinishFlag; // 捕获结束标志位
	uint8_t Capture_StartFlag; // 捕获开始标志位
	uint16_t Capture_CNTValue; // 第一次捕获边沿时计数器的值
	uint16_t Capture_CcrValue; // 捕获寄存器的值
	uint16_t Capture_Period; // 自动重装载寄存器更新标志
}TIM_ICUserValueType;

#define USE_TIM3							1
#define USE_TIM3_CH1          1
#define USE_TIM3_CH2          1
#define USE_TIM3_CH3          1
#define USE_TIM3_CH4          1
//对TIM3的定义
#define TIM3_IT_CCx						/**/TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4/**/


#define USE_TIM4              0
#define USE_TIM4_CH1          1
#define USE_TIM4_CH2          1
#define USE_TIM4_CH3          1
#define USE_TIM4_CH4          1
//对TIM4的定义
#define TIM4_IT_CCx						TIM_IT_CC1//|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4


//对时钟的宏定义
//TIM3
#if (USE_TIM3_CH1|USE_TIM3_CH2)&(USE_TIM3_CH3|USE_TIM3_CH4)		
	#define TIM3_GPIO_CLK 	RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC 
#elif (USE_TIM3_CH1|USE_TIM3_CH2 == 1)&&(USE_TIM3_CH3|USE_TIM3_CH4 == 0)
	#define TIM3_GPIO_CLK 	RCC_APB2Periph_GPIOA
#else
	#define TIM3_GPIO_CLK 	RCC_APB2Periph_GPIOB
#endif														
//TIM4
#define TIM4_GPIO_CLK 		RCC_APB2Periph_GPIOB
	
													
	
//TIM3的GPIO引脚
#define TIM3_CH1_PIN						GPIO_Pin_6
#define TIM3_CH1_PORT					GPIOA

#define TIM3_CH2_PIN						GPIO_Pin_7
#define TIM3_CH2_PORT					GPIOA

#define TIM3_CH3_PIN						GPIO_Pin_0
#define TIM3_CH3_PORT					GPIOB

#define TIM3_CH4_PIN						GPIO_Pin_1
#define TIM3_CH4_PORT					GPIOB

//TIM4的GPIO引脚
#define TIM4_CH1_PIN						GPIO_Pin_6
#define TIM4_CH1_PORT					GPIOB

#define TIM4_CH2_PIN						GPIO_Pin_7
#define TIM4_CH2_PORT					GPIOB

#define TIM4_CH3_PIN						GPIO_Pin_8
#define TIM4_CH3_PORT					GPIOB

#define TIM4_CH4_PIN						GPIO_Pin_9
#define TIM4_CH4_PORT					GPIOB




// 获取捕获寄存器值函数宏定义
#define GENERAL_TIM_GetCapturex_FUN 	TIM_GetCapture1
// 捕获信号极性函数宏定义
#define GENERAL_TIM_OCxPolarityConfig_FUN TIM_OC1PolarityConfig
// 测量的起始边沿
#define GENERAL_TIM_STRAT_ICPolarity 	TIM_ICPolarity_Rising
// 测量的结束边沿
//#define GENERAL_TIM_END_ICPolarity 		TIM_ICPolarity_Falling

void TIM_Init(void);
void PWM_IC_Print(void);




#endif // _BSP_GENERAL_TIM_H_
