#include "bsp_advance_tim.h"
static void GENERAL_TIM_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = ADVANCE_TIM_IRQ ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void Advance_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//����CH1
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH1_GPIO_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = ADVANCE_TIM_CH1_PIN;
	GPIO_Init(ADVANCE_TIM_CH1_PORT,&GPIO_InitStructure);
}

void Advance_TIM_Mode_Config(void)
{
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK,ENABLE);
	//��ʼ��ʱ�ӻ����ṹ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = ADVANCE_TIM_PERIOD;       //����
	TIM_TimeBaseStructure.TIM_Prescaler = ADVANCE_TIM_PSC;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);
	//����ȽϽṹ���ʼ��
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICInitStruct.TIM_Channel = ADVANCE_TIM_CHANNEL_x;
	TIM_ICInitStruct.TIM_ICFilter = 0;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// ��ʼ��PWM����ģʽ
  TIM_PWMIConfig(ADVANCE_TIM, &TIM_ICInitStruct);
	// ѡ�����벶��Ĵ����ź�
  TIM_SelectInputTrigger(ADVANCE_TIM, TIM_TS_TI1FP1);		

	// ѡ���ģʽ: ��λģʽ
	// PWM����ģʽʱ,��ģʽ���빤���ڸ�λģʽ��������ʼʱ,������CNT�ᱻ��λ
  TIM_SelectSlaveMode(ADVANCE_TIM, TIM_SlaveMode_Reset);
  TIM_SelectMasterSlaveMode(ADVANCE_TIM,TIM_MasterSlaveMode_Enable); 

  // ʹ�ܲ����ж�,����ж���Ե���������ͨ�������������Ǹ���
  TIM_ITConfig(ADVANCE_TIM, TIM_IT_CC1, ENABLE);	
	// ����жϱ�־λ
	TIM_ClearITPendingBit(ADVANCE_TIM, TIM_IT_CC1);
	
	 // ʹ�ܸ߼����ƶ�ʱ������������ʼ����
  TIM_Cmd(ADVANCE_TIM, ENABLE);
}

void Advance_TIM_Init(void)
{
	GENERAL_TIM_NVIC_Config();
	Advance_TIM_GPIO_Config();
	Advance_TIM_Mode_Config();
}


