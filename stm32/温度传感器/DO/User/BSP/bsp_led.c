#include "bsp_led.h"



void LED_Configer(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(LED_GPIO_CLK,ENABLE);
	//≈‰÷√LED¬Ãµ∆
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = LED_G_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_GPIO_PORT,&GPIO_InitStructure);
	//≈‰÷√DIN ‰»Î
	GPIO_InitStructure.GPIO_Pin = DIN_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(LED_GPIO_PORT,&GPIO_InitStructure);		
	
}



