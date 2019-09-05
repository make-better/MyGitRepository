#ifndef _BSP_EXTI_H_
#define _BSP_EXTI_H_

#include "stm32f10x.h"
#include "stm32f10x_exti.h"

#define EXTI1_GPIO_PortSource     GPIO_PortSourceGPIOA
#define EXTI1_GPIO_PinSource      GPIO_PinSource0
#define EXTI13_GPIO_PortSource    GPIO_PortSourceGPIOC
#define EXTI13_GPIO_PinSource     GPIO_PinSource13
#define EXTI_AFIO_CLK             RCC_APB2Periph_AFIO

#define KEY1_EXTI_PORT 					  GPIOA
#define KEY1_EXTI_PIN   				  GPIO_Pin_0
#define KEY2_EXTI_PORT 					  GPIOC
#define KEY2_EXTI_PIN        		  GPIO_Pin_13
#define KEY1_NVIC_IRQChannel 			EXTI0_IRQn
#define KEY2_NVIC_IRQChannel		  EXTI15_10_IRQn

#define KEY_EXTI_CLK     RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC

void EXTI_Configer(void);



#endif // _BSP_EXTI_H_


