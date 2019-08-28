#include "bsp_gpio.h"
//电机控制管脚初始化
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







