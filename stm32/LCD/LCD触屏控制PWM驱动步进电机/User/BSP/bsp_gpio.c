#include "bsp_gpio.h"

uint8_t en=0;
uint8_t dir=0;
uint8_t onoff=0;

void SDC_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(SDC_GPIO_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = SDC_DIR_GPIO_Pin|SDC_EN_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SDC_GPIO_PORT,&GPIO_InitStructure);
	GPIO_Write(SDC_GPIO_PORT,0X0);
} 







