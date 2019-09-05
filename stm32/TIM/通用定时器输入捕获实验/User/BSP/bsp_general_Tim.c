#include "bsp_general_Tim.h"

TIM_ICUserValueType TIM_ICUserValueStructure;

 // 中断优先级配置
static void GENERAL_TIM_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	// 设置中断组为 0
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQ ;
	// 设置主优先级为 0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	// 设置抢占优先级为 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
static void GENERAL_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//配置CH1
	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH1_GPIO_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GENERAL_TIM_CH1_PIN;
	GPIO_Init(GENERAL_TIM_CH1_PORT,&GPIO_InitStructure);
}
static void GENERAL_TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK, ENABLE);
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;
	TIM_TimeBaseInitStructure.TIM_Period = 0xffff;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	
	TIM_ClearFlag(GENERAL_TIMx, TIM_FLAG_Update);
	TIM_TimeBaseInit(GENERAL_TIMx,&TIM_TimeBaseInitStructure);
	
}

static void TIM_GENERAL_Config(void)
{
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	TIM_ICInitStructure.TIM_Channel = GENERAL_TIM_CHANNEL_x;
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(GENERAL_TIMx, &TIM_ICInitStructure);
	TIM_ClearFlag(GENERAL_TIMx, TIM_FLAG_Update|GENERAL_TIM_IT_CCx);
// 开启更新和捕获中断
	TIM_ITConfig (GENERAL_TIMx, TIM_IT_Update | GENERAL_TIM_IT_CCx, ENABLE );
// 使能计数器
	TIM_Cmd(GENERAL_TIMx, ENABLE);
}
void TIM_Init(void)
{
	GENERAL_TIM_NVIC_Config();
	GENERAL_TIM_GPIO_Config();
	GENERAL_TIM_Config();
	TIM_GENERAL_Config();
}

