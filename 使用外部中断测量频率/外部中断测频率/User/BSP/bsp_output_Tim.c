#include "bsp_output_Tim.h"

static void GENERAL_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//����CHx
	RCC_APB2PeriphClockCmd(GENERAL_TIM_OUT_CHx_GPIO_CLK,ENABLE);
	//B6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�����������
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
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//1��Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//����
	
	TIM_ClearFlag(GENERAL_TIMx_OUT, TIM_FLAG_Update);
	TIM_TimeBaseInit(GENERAL_TIMx_OUT,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef	 TIM_OCInitStructure;
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;						//����ģʽѡ��PWM1
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;	//����״̬�����1
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//����״̬�»������1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;			//������Ը�
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//�Ƚϻ������ʧ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		//�Ƚ����ʹ��

	TIM_OCInitStructure.TIM_Pulse = 200;														//������CCR������ת���״̬
	TIM_OC_x_Init(GENERAL_TIMx_OUT,&TIM_OCInitStructure);				//����ͨ��x
	TIM_OC_x_PreloadConfig(GENERAL_TIMx_OUT, TIM_OCPreload_Enable);//����Ԥװ�ؼĴ���
	TIM_Cmd(GENERAL_TIMx_OUT,ENABLE);						//��ʱ������
}
void TIM_OutPut_Init(void)
{
	GENERAL_TIM_GPIO_Config();
	GENERAL_TIM_Config();
}

