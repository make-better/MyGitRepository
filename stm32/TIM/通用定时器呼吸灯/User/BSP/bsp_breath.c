#include "bsp_breath.h"
//����������ε�Ƶ��
__IO uint16_t period_class = 5;

/* LED���ȵȼ� PWM��,ָ������ ���˱�ʹ�ù���Ŀ¼�µ�python�ű�index_wave.py����*/
uint16_t indexWave[] = {
1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 4,
4, 5, 5, 6, 7, 8, 9, 10, 11, 13,
15, 17, 19, 22, 25, 28, 32, 36,
41, 47, 53, 61, 69, 79, 89, 102,
116, 131, 149, 170, 193, 219, 250,
284, 323, 367, 417, 474, 539, 613,
697, 792, 901, 1024, 1024, 901, 792,
697, 613, 539, 474, 417, 367, 323,
284, 250, 219, 193, 170, 149, 131, 
116, 102, 89, 79, 69, 61, 53, 47, 41,
36, 32, 28, 25, 22, 19, 17, 15, 13, 
11, 10, 9, 8, 7, 6, 5, 5, 4, 4, 3, 3,
2, 2, 2, 2, 1, 1, 1, 1
	
};

//����PWM���ж��ٸ�Ԫ��
uint16_t POINT_NUM = sizeof(indexWave)/sizeof(indexWave[0]); 

static void LED_Breath_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//����GPIOʱ��
	BRE_TIM_GPIO_APBxClock_FUN(BRE_TIM_GPIO_CLK,ENABLE);
	//PB5��Ҫ��ӳ��
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
  
  /* ����TIM3_IRQ�ж�Ϊ�ж�Դ */
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
	//������ʱ��ʱ��
	BRE_TIM_APBxClock_FUN(BRE_TIM_CLK,ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = 200-1;
	TIM_TimeBaseInitStructure.TIM_Period = 2048-1;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(BRE_TIMx,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;		//���ο�ʼ�ǵ͵�ƽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	
	BRE_TIM_OCxPreloadConfig(BRE_TIMx,TIM_OCPreload_Enable);
	
	BRE_TIM_OCxInit(BRE_TIMx,&TIM_OCInitStructure);
	
	TIM_ARRPreloadConfig(BRE_TIMx, ENABLE);			 										//ʹ��COLOR_TIMx���ؼĴ���ARR
	TIM_Cmd(BRE_TIMx,ENABLE);
	TIM_ITConfig(BRE_TIMx, TIM_IT_Update, ENABLE);										//ʹ��update�ж�
	
	NVIC_Config_PWM();
}

void TIM_Breath_Init(void)
{
	LED_Breath_GPIO_Config();
	LED_Breath_Tim_Config();
}




