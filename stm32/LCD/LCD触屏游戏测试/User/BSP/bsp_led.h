#ifndef _BSP_LED_H
#define _BSP_LED_H

#include "stm32f10x.h"
#define LED_GPIO_PORT    GPIOB
#define LED_G_GPIO_PIN     GPIO_Pin_0
#define LED_R_GPIO_PIN     GPIO_Pin_5
#define LED_B_GPIO_PIN     GPIO_Pin_1
#define LED_GPIO_CLK     RCC_APB2Periph_GPIOB

#define ON  1
#define OFF 0
#define LED_1(A) if(A) GPIO_ResetBits(LED_GPIO_PORT,LED_G_GPIO_PIN);\
								else GPIO_SetBits(LED_GPIO_PORT,LED_G_GPIO_PIN);
#define LED_2(A) if(A) GPIO_ResetBits(LED_GPIO_PORT,LED_R_GPIO_PIN);\
								else GPIO_SetBits(LED_GPIO_PORT,LED_R_GPIO_PIN);
#define LED_3(A) if(A) GPIO_ResetBits(LED_GPIO_PORT,LED_B_GPIO_PIN);\
								else GPIO_SetBits(LED_GPIO_PORT,LED_B_GPIO_PIN);

//ºì
#define LED_RED  \
					LED_1(OFF)\
					LED_2(ON)\
					LED_3(OFF)

//ÂÌ
#define LED_GREEN		\
					LED_1(ON);\
					LED_2(OFF)\
					LED_3(OFF)

//À¶
#define LED_BLUE	\
					LED_1(OFF);\
					LED_2(OFF)\
					LED_3(ON)


void LED_Configer(void);

#endif //_BSP_H

