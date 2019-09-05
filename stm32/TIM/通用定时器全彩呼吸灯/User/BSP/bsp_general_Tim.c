#include "bsp_general_Tim.h"

TIM_OCInitTypeDef	 TIM_OCInitStructure;


static void GENERAL_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//配置CH2
	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH2_GPIO_CLK|RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GENERAL_TIM_CH2_PIN;
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GENERAL_TIM_CH2_PORT,&GPIO_InitStructure);
	//配置CH3
	GPIO_InitStructure.GPIO_Pin = GENERAL_TIM_CH3_PIN;
	GPIO_Init(GENERAL_TIM_CH3_PORT,&GPIO_InitStructure);
	//配置CH4
	GPIO_InitStructure.GPIO_Pin = GENERAL_TIM_CH4_PIN;
	GPIO_Init(GENERAL_TIM_CH4_PORT,&GPIO_InitStructure);
}

static void GENERAL_TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK, ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1999;
	TIM_TimeBaseInitStructure.TIM_Period = 255;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	
	TIM_ClearFlag(TIMx, TIM_FLAG_Update);
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	//TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;		//波形开始是低电平
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	
	TIM_OC2Init(TIMx,&TIM_OCInitStructure);//B5
	TIM_OC2PreloadConfig(TIMx,TIM_OCPreload_Enable);

	TIM_OC3Init(TIMx,&TIM_OCInitStructure);//B0
	TIM_OC3PreloadConfig(TIMx,TIM_OCPreload_Enable);

	TIM_OC4Init(TIMx,&TIM_OCInitStructure);//B1
	TIM_OC4PreloadConfig(TIMx,TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIMx, ENABLE);			 										//使能COLOR_TIMx重载寄存器ARR
	
	TIM_Cmd(TIMx,ENABLE);
}

void TIM_Init(void)
{
	GENERAL_TIM_GPIO_Config();
	GENERAL_TIM_Config();
}

