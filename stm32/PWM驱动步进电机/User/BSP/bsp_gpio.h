#ifndef _BSP_GPIO_H_
#define _BSP_GPIO_H_
#include "stm32f10x.h"

#define SDC_GPIO_CLK 			RCC_APB2Periph_GPIOC
#define SDC_DIR_GPIO_Pin  GPIO_Pin_9
#define SDC_EN_GPIO_Pin   GPIO_Pin_8
#define SDC_GPIO_PORT			GPIOC


void SDC_GPIO_Init(void);


#endif //_BSP_GPIO_H_
