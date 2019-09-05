#ifndef _BSP_ADVANCE_TIM_H_
#define _BSP_ADVANCE_TIM_H_
#include "stm32f10x.h"
 
#define ADVANCE_TIM_CH4_GPIO_CLK 				RCC_APB2Periph_GPIOA
#define ADVANCE_TIM_CH4_PIN 						GPIO_Pin_11
#define ADVANCE_TIM_CH4_PORT 						GPIOA

#define ADVANCE_TIM_CH1N_GPIO_CLK 			RCC_APB2Periph_GPIOA
#define ADVANCE_TIM_CH1N_PORT 					GPIOA
#define ADVANCE_TIM_CH1N_PIN 						GPIO_Pin_7

#define ADVANCE_TIM_BKIN_GPIO_CLK 			RCC_APB2Periph_GPIOA
#define ADVANCE_TIM_BKIN_PORT 					GPIOA
#define ADVANCE_TIM_BKIN_PIN 						GPIO_Pin_6

#define ADVANCE_TIM											TIM1
#define ADVANCE_TIM_APBxClock_FUN 			RCC_APB2PeriphClockCmd
#define ADVANCE_TIM_CLK 								RCC_APB2Periph_TIM1
	// PWM �źŵ�Ƶ�� F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define ADVANCE_TIM_PERIOD 							(16000-1)//����
#define ADVANCE_TIM_PSC 								(9-1)
#define ADVANCE_TIM_PULSE 							(ADVANCE_TIM_PERIOD+1)/2		//ռ�ձ�

#define ADVANCE_TIM_IRQ 								TIM8_UP_IRQn
#define ADVANCE_TIM_IRQHandler 					TIM8_UP_IRQHandler


void Pulse_output(uint16_t Cycle,uint16_t PulseNum);

#endif //_BSP_ADVANCE_TIM_H_
