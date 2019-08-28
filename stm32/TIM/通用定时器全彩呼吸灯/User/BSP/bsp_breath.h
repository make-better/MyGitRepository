#ifndef _BSP_BREATH_H_
#define _BSP_BREATH_H_
#include "stm32f10x.h"
/*PWM表中的点数*/
extern uint16_t  POINT_NUM	;
//控制输出波形的频率
extern __IO uint16_t period_class ;

#define 		MAX 							255

/********************定时器通道**************************/

/************红灯***************/
	#define   BRE_TIMx                      TIM3

	#define   BRE_TIM_APBxClock_FUN        RCC_APB1PeriphClockCmd
	#define   BRE_TIM_CLK                   RCC_APB1Periph_TIM3
	#define   BRE_TIM_GPIO_APBxClock_FUN   RCC_APB2PeriphClockCmd
	#define   BRE_TIM_GPIO_CLK              (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)

	//红灯的引脚需要重映射
	#define   BRE_GPIO_REMAP_FUN()						GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); 				

	#define  BRE_TIM_LED_PORT               GPIOB
	#define  BRE_TIM_LED_PIN                GPIO_Pin_5|GPIO_Pin_0|GPIO_Pin_1

	#define  BRE_RED_TIM_OCxInit                TIM_OC2Init            //通道选择，1~4
	#define  BRE_RED_TIM_OCxPreloadConfig       TIM_OC2PreloadConfig 
	#define  BRE_RED_CCRx                       CCR2

	#define   BRE_TIMx_IRQn                TIM3_IRQn              //中断
	#define   BRE_TIMx_IRQHandler          TIM3_IRQHandler


/************绿灯***************/
			
	#define  BRE_GREEN_TIM_OCxInit                TIM_OC3Init            //通道选择，1~4
	#define  BRE_GREEN_TIM_OCxPreloadConfig       TIM_OC3PreloadConfig 
	#define  BRE_GREEN_CCRx                       CCR3

/************蓝灯***************/

	#define   BRE_BLUE_TIM_OCxInit              TIM_OC4Init            //通道选择，1~4
	#define   BRE_BLUE_TIM_OCxPreloadConfig    TIM_OC4PreloadConfig 
	#define   BRE_BLUE_CCRx                      CCR4


void TIM_Breath_Init(void);



#endif //_BSP_BREATH_H_
