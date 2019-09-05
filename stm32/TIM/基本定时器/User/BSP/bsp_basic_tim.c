#include "bsp_basic_tim.h"

static void TIM_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannel = NVIC_TIM_Channel;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
	
}

static void TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;
	TIM_TimeBaseInitStructure.TIM_Period = 999;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	
	TIM_ClearFlag(TIMx, TIM_FLAG_Update);
  TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE);
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseInitStructure);
	TIM_Cmd(TIMx,ENABLE);
	
}

void Tim_Init(void)
{
	TIM_Config();
	TIM_NVIC_Config();
}

