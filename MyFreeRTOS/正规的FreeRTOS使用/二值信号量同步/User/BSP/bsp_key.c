#include "bsp_key.h"

void Key_Configer(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(KEY_GPIO_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;
	GPIO_Init(KEY1_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN;
	GPIO_Init(KEY2_GPIO_PORT,&GPIO_InitStructure);
}

uint16_t Key_Scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_PIN)
{
	if((GPIOx->IDR&GPIO_PIN)==GPIO_PIN)
	{
		while((GPIOx->IDR&GPIO_PIN)==GPIO_PIN);
		return KEY_ON;
	}
	else
		return KEY_OFF;
}




