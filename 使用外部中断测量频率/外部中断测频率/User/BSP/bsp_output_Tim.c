#include "bsp_output_Tim.h"

static void GENERAL_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//配置CHx
	RCC_APB2PeriphClockCmd(GENERAL_TIM_OUT_CHx_GPIO_CLK,ENABLE);
	//B6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GENERAL_TIM_OUT_CHx_PIN;
	GPIO_Init(GENERAL_TIM_OUT_CHx_PORT,&GPIO_InitStructure);
}
static void GENERAL_TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	GENERAL_TIM_OUT_APBxClock_FUN(GENERAL_TIM_OUT_CLK, ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720-1;//100kHz
	TIM_TimeBaseInitStructure.TIM_Period = 500-1;		//100k/500 = 200hz
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//1分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//忽略
	
	TIM_ClearFlag(GENERAL_TIMx_OUT, TIM_FLAG_Update);
	TIM_TimeBaseInit(GENERAL_TIMx_OUT,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef	 TIM_OCInitStructure;
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;						//工作模式选择PWM1
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;	//空闲状态下输出1
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//空闲状态下互补输出1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;			//输出极性高
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//比较互补输出失能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		//比较输出使能

	TIM_OCInitStructure.TIM_Pulse = 200;														//脉冲宽度CCR到几反转输出状态
	TIM_OC_x_Init(GENERAL_TIMx_OUT,&TIM_OCInitStructure);				//配置通道x
	TIM_OC_x_PreloadConfig(GENERAL_TIMx_OUT, TIM_OCPreload_Enable);//启用预装载寄存器
	TIM_Cmd(GENERAL_TIMx_OUT,ENABLE);						//定时器启动
}
void TIM_OutPut_Init(void)
{
	GENERAL_TIM_GPIO_Config();
	GENERAL_TIM_Config();
}

