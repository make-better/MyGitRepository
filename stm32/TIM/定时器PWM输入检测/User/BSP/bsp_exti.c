#include "bsp_exti.h"

//≈‰÷√NVIC
static void NVIC_Configer(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = KEY1_NVIC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
//	NVIC_InitStructure.NVIC_IRQChannel = KEY2_NVIC_IRQChannel;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//	NVIC_Init(&NVIC_InitStructure);
}

//≥ı ºªØEXTI
void EXTI_Configer(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
		//≈‰÷√NVIC
	NVIC_Configer();
	
	//ø™∆Ù∞¥º¸∂Àø⁄ ±÷”
	RCC_APB2PeriphClockCmd(KEY_EXTI_CLK,ENABLE);
	//ø™AFIO ±÷”
	RCC_APB2PeriphClockCmd(EXTI_AFIO_CLK, ENABLE);
	//≈‰÷√KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = KEY1_EXTI_PIN;
	GPIO_Init(KEY1_EXTI_PORT,&GPIO_InitStructure);
	GPIO_EXTILineConfig(EXTI1_GPIO_PortSource,EXTI1_GPIO_PinSource);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
  //≈‰÷√KEY2
//	GPIO_InitStructure.GPIO_Pin = KEY2_EXTI_PIN;
//	GPIO_Init(KEY2_EXTI_PORT,&GPIO_InitStructure);
//	GPIO_EXTILineConfig(EXTI13_GPIO_PortSource,EXTI13_GPIO_PinSource);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
//	EXTI_Init(&EXTI_InitStructure);
	
}









