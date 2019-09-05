#include "bsp_advance_tim.h"

TIM_TimeBaseInitTypeDef 	TIM_TimeBaseStructure;
TIM_OCInitTypeDef 				TIM_OCInitStructure;

void Advance_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//����CH1
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH1_GPIO_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = ADVANCE_TIM_CH1_PIN;
	GPIO_Init(ADVANCE_TIM_CH1_PORT,&GPIO_InitStructure);
	
	//����CH1N
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH1N_GPIO_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Pin = ADVANCE_TIM_CH1N_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(ADVANCE_TIM_CH1N_PORT,&GPIO_InitStructure);
	
	//����BKIN
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_BKIN_GPIO_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Pin = ADVANCE_TIM_BKIN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(ADVANCE_TIM_BKIN_PORT,&GPIO_InitStructure);
	// BKIN ����Ĭ��������͵�ƽ
	GPIO_ResetBits(ADVANCE_TIM_BKIN_PORT,ADVANCE_TIM_BKIN_PIN);
}

void Advance_TIM_Mode_Config(void)
{
	
	
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK,ENABLE);
	//��ʼ��ʱ�ӻ����ṹ��
	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = ADVANCE_TIM_PERIOD;       //����
	TIM_TimeBaseStructure.TIM_Prescaler = ADVANCE_TIM_PSC;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);
	//����ȽϽṹ���ʼ��
	
	TIM_OCInitStructure.TIM_Pulse = ADVANCE_TIM_PULSE;       //ռ�ձ�
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCNIdleState =  TIM_OCNIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC1Init(ADVANCE_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(ADVANCE_TIM, TIM_OCPreload_Enable);
	
	
	TIM_Cmd(ADVANCE_TIM, ENABLE);
	// �����ʹ�ܣ���ʹ�õ���ͨ�ö�ʱ��ʱ����䲻��Ҫ
	TIM_CtrlPWMOutputs(ADVANCE_TIM, ENABLE);
}

void Advance_TIM_Init(void)
{
	Advance_TIM_GPIO_Config();
	Advance_TIM_Mode_Config();
}


