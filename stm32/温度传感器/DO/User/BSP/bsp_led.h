#ifndef _BSP_LED_H
#define _BSP_LED_H

#include "stm32f10x.h"
#define LED_GPIO_PORT    GPIOB
#define LED_G_GPIO_PIN     GPIO_Pin_0
#define LED_R_GPIO_PIN     GPIO_Pin_5
#define LED_B_GPIO_PIN     GPIO_Pin_1
#define LED_GPIO_CLK     RCC_APB2Periph_GPIOB
#define DIN_GPIO_PIN     GPIO_Pin_9

#define ON  1
#define OFF 0
#define LED_G(A) if(A) GPIO_ResetBits(LED_GPIO_PORT,LED_G_GPIO_PIN);\
								else GPIO_SetBits(LED_GPIO_PORT,LED_G_GPIO_PIN);
#define LED_R(A) if(A) GPIO_ResetBits(LED_GPIO_PORT,LED_R_GPIO_PIN);\
								else GPIO_SetBits(LED_GPIO_PORT,LED_R_GPIO_PIN);
#define LED_B(A) if(A) GPIO_ResetBits(LED_GPIO_PORT,LED_B_GPIO_PIN);\
								else GPIO_SetBits(LED_GPIO_PORT,LED_B_GPIO_PIN);

void LED_Configer(void);

#endif //_BSP_H

