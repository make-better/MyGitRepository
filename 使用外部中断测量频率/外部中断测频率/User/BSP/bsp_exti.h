#ifndef _BSP_EXTI_H_
#define _BSP_EXTI_H_
#include "stm32f10x.h"

//A4
#define Fre1_EXTI_PortSource     GPIO_PortSourceGPIOA
#define Fre1_EXTI_PinSource      GPIO_PinSource4
//A5
#define Fre2_EXTI_PortSource    GPIO_PortSourceGPIOA
#define Fre2_EXTI_PinSource     GPIO_PinSource5
//A6
#define Fre3_EXTI_PortSource     GPIO_PortSourceGPIOA
#define Fre3_EXTI_PinSource      GPIO_PinSource6
//A7
#define Fre4_EXTI_PortSource     GPIO_PortSourceGPIOA
#define Fre4_EXTI_PinSource      GPIO_PinSource7
//B12
#define Fre5_EXTI_PortSource     GPIO_PortSourceGPIOB
#define Fre5_EXTI_PinSource      GPIO_PinSource12
//B13
#define Fre6_EXTI_PortSource     GPIO_PortSourceGPIOB
#define Fre6_EXTI_PinSource      GPIO_PinSource13
//B14
#define Fre7_EXTI_PortSource     GPIO_PortSourceGPIOB
#define Fre7_EXTI_PinSource      GPIO_PinSource14
//B15
#define Fre8_EXTI_PortSource     GPIO_PortSourceGPIOB
#define Fre8_EXTI_PinSource      GPIO_PinSource15

#define EXTI_AFIO_CLK             RCC_APB2Periph_AFIO

//GPIOA_4~7
#define FRE1_4_EXTI_PORT 					  GPIOA
#define FRE1_EXTI_PIN   				  GPIO_Pin_4
#define FRE2_EXTI_PIN        		  GPIO_Pin_5
#define FRE3_EXTI_PIN   				  GPIO_Pin_6
#define FRE4_EXTI_PIN        		  GPIO_Pin_7
//GPIOB_12~15
#define FRE5_8_EXTI_PORT 					  GPIOB
#define FRE5_EXTI_PIN        		  GPIO_Pin_12
#define FRE6_EXTI_PIN        		  GPIO_Pin_13
#define FRE7_EXTI_PIN        		  GPIO_Pin_14
#define FRE8_EXTI_PIN        		  GPIO_Pin_15

//EXTI_Line
#define Fre1_INT_EXTI_Line        EXTI_Line4
#define Fre2_INT_EXTI_Line        EXTI_Line5
#define Fre3_INT_EXTI_Line        EXTI_Line6
#define Fre4_INT_EXTI_Line        EXTI_Line7

#define Fre5_INT_EXTI_Line        EXTI_Line12
#define Fre6_INT_EXTI_Line        EXTI_Line13
#define Fre7_INT_EXTI_Line        EXTI_Line14
#define Fre8_INT_EXTI_Line        EXTI_Line15

//中断源们
#define FRE1_EXTI_IRQn 				EXTI4_IRQn
#define FRE2_4_EXTI_IRQn		  EXTI9_5_IRQn
#define FRE5_8_EXTI_IRQn		  EXTI15_10_IRQn
//中断输入GPIO时钟
#define FRE_GPIO_CLK     RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB

void Fre_EXTI_Init(void);



#endif // _BSP_EXTI_H_


