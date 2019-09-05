#include "bsp_general_Tim.h"
#include "bsp_usart.h"
#include "bsp_led.h"

TIM_ICUserValueType TIM_ICUserValue[8];

 // �ж����ȼ�����
static void GENERAL_TIM_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
#if USE_TIM3
	// �����ж���Ϊ 1
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	// �����ж���Դ
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn ;
	// ���������ȼ�Ϊ 1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	// ������ռ���ȼ�Ϊ 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

#if USE_TIM4
		// �����ж���Ϊ 1
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	// �����ж���Դ
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn ;
	// ���������ȼ�Ϊ 1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	// ������ռ���ȼ�Ϊ 4
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
}

static void TIM3_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(TIM3_GPIO_CLK,ENABLE);
#if USE_TIM3_CH1
	//����CH1 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = TIM3_CH1_PIN;
	GPIO_Init(TIM3_CH1_PORT,&GPIO_InitStructure);
#endif
	
#if USE_TIM3_CH2
	//����CH2 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = TIM3_CH2_PIN;
	GPIO_Init(TIM3_CH2_PORT,&GPIO_InitStructure);
#endif
	
#if USE_TIM3_CH3
	//����CH3 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = TIM3_CH3_PIN;
	GPIO_Init(TIM3_CH3_PORT,&GPIO_InitStructure);
#endif

#if USE_TIM3_CH4
	//����CH4 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = TIM3_CH4_PIN;
	GPIO_Init(TIM3_CH4_PORT,&GPIO_InitStructure);
#endif
}

static void GENERAL_TIM3_Config(void)
{
	//��������
	TIM3_GPIO_Config();
	//����ʱ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;
	TIM_TimeBaseInitStructure.TIM_Period = 0xffff;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
}

static void TIM3_IC_Config(void)
{
	TIM_ICInitTypeDef TIM_ICInitStructure;
#if USE_TIM3_CH1
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0f;
	TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
#endif
#if USE_TIM3_CH2
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0x0f;
	TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
#endif
#if USE_TIM3_CH3
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
	TIM_ICInitStructure.TIM_ICFilter = 0x0f;
	TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
#endif
#if USE_TIM3_CH4
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
	TIM_ICInitStructure.TIM_ICFilter = 0x0f;
	TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
#endif
// �������ºͲ����ж�
	TIM_ITConfig (TIM3, TIM_IT_Update | TIM3_IT_CCx, ENABLE );
	TIM_ClearFlag(TIM3, TIM_FLAG_Update|TIM3_IT_CCx);
// ʹ�ܼ�����
	TIM_Cmd(TIM3, ENABLE);
}
void TIM_Init(void)
{
	GENERAL_TIM_NVIC_Config();
	
#if USE_TIM3
	GENERAL_TIM3_Config();
	TIM3_IC_Config();
#endif
	
	
}

void PWM_IC_Print(void)
{
	//Ҫ��д��������������������������������������������
	double time1;
// TIM ������������ʱ��
	uint32_t TIM_PscCLK = 72000000 / (71+1);
	int i = 0;
	for(i = 0;i<8;i++)
	{
		if (TIM_ICUserValue[i].Capture_FinishFlag == 1) 
		{
		 // ����ߵ�ƽʱ��ļ�������ֵ
		 time1 = (TIM_ICUserValue[i].Capture_CcrValue+1);
		 // ��ӡ�ߵ�ƽ����ʱ��
			printf ( "\r\n %d��������Ƶ��: %f hz\r\n",i,1/(time1/TIM_PscCLK));
		 TIM_ICUserValue[i].Capture_FinishFlag = 0;
		}
	}
	
}
