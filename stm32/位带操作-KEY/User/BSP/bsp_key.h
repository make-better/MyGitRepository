#ifndef _BSP_KEY_H
#define _BSP_KEY_H

#include "stm32f10x.h"
#define KEY1_GPIO_PORT    GPIOA
#define KEY1_GPIO_PIN     GPIO_Pin_0
#define KEY2_GPIO_PORT    GPIOC
#define KEY2_GPIO_PIN     GPIO_Pin_13

#define KEY_ON  1
#define KEY_OFF 0

#define KEY_GPIO_CLK     RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC

void Kye_Configer(void);
uint16_t Key_Scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_PIN);



#endif //_BSP_KEY_H
