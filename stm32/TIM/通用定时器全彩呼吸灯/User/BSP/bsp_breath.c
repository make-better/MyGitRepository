#include "bsp_breath.h"
//控制输出波形的频率
__IO uint16_t period_class = 1;
uint32_t color_value = 0xffff00;

/* LED亮度等级 PWM表,指数曲线 ，此表使用工程目录下的python脚本index_wave.py生成*/
uint16_t indexWave[] = {
 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2,
 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5,
 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10,
 11, 12, 12, 13, 14, 15, 17, 18,
 19, 20, 22, 23, 25, 27, 29, 31,
 33, 36, 38, 41, 44, 47, 51, 54,
 58, 63, 67, 72, 77, 83, 89, 95,
 102, 110, 117, 126, 135, 145, 156,
 167, 179, 192, 206, 221, 237, 254,
 272, 292, 313, 336, 361, 387, 415,
 445, 477, 512, 512, 477, 445, 415,
 387, 361, 336, 313, 292, 272, 254,
 237, 221, 206, 192, 179, 167, 156,
 145, 135, 126, 117, 110, 102, 95,
 89, 83, 77, 72, 67, 63, 58, 54, 51,
 47, 44, 41, 38, 36, 33, 31, 29, 27,
 25, 23, 22, 20, 19, 18, 17, 15, 14,
 13, 12, 12, 11, 10, 9, 9, 8, 8, 7, 7,
 6, 6, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3,
 3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1
	
};

//计算PWM表有多少个元素
uint16_t POINT_NUM = sizeof(indexWave)/sizeof(indexWave[0]); 

static void LED_Breath_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//开启GPIO时钟
	BRE_TIM_GPIO_APBxClock_FUN(BRE_TIM_GPIO_CLK,ENABLE);
	//PB5需要重映射
	BRE_GPIO_REMAP_FUN();
	
	GPIO_InitStructure.GPIO_Pin = BRE_TIM_LED_PIN;
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(BRE_TIM_LED_PORT,&GPIO_InitStructure);
}

static void NVIC_Config_PWM(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* 配置TIM3_IRQ中断为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = BRE_TIMx_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


static void LED_Breath_Tim_Config(void)
{
	TIM_OCInitTypeDef	 TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	//开启定时器时钟
	BRE_TIM_APBxClock_FUN(BRE_TIM_CLK,ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = 10-1;
	TIM_TimeBaseInitStructure.TIM_Period = 512-1;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(BRE_TIMx,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;		//波形开始是低电平
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	
	BRE_RED_TIM_OCxPreloadConfig(BRE_TIMx,TIM_OCPreload_Enable);
	BRE_RED_TIM_OCxInit(BRE_TIMx,&TIM_OCInitStructure);
	
	BRE_GREEN_TIM_OCxPreloadConfig(BRE_TIMx,TIM_OCPreload_Enable);
	BRE_GREEN_TIM_OCxInit(BRE_TIMx,&TIM_OCInitStructure);
	
	BRE_BLUE_TIM_OCxPreloadConfig(BRE_TIMx,TIM_OCPreload_Enable);
	BRE_BLUE_TIM_OCxInit(BRE_TIMx,&TIM_OCInitStructure);
	
	TIM_ARRPreloadConfig(BRE_TIMx, ENABLE);			 										//使能COLOR_TIMx重载寄存器ARR
	TIM_Cmd(BRE_TIMx,ENABLE);
	TIM_ITConfig(BRE_TIMx, TIM_IT_Update, ENABLE);										//使能update中断
	
	NVIC_Config_PWM();
}

void TIM_Breath_Init(void)
{
	LED_Breath_GPIO_Config();
	LED_Breath_Tim_Config();
}




