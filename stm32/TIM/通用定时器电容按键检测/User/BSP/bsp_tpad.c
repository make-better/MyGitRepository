#include "bsp_tpad.h"
#include "bsp_general_Tim.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"

uint16_t Tpad_default_value=0;

void Tpad_Reset(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//����CH2Ϊ���ģʽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GENERAL_TIM_CH2_PIN;
	GPIO_Init(GENERAL_TIM_CH2_PORT,&GPIO_InitStructure);
	GPIO_ResetBits(GENERAL_TIM_CH2_PORT,GENERAL_TIM_CH2_PIN);
	
	SysTick_Delay_Ms( 5 );
	//�ٴθ�Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GENERAL_TIM_CH2_PORT,&GPIO_InitStructure);
}

uint16_t Tpad_Get_Val(void)
{
	Tpad_Reset();
	//��λ�����������
	TIM_SetCounter (TPAD_TIM,0);
	// �����صı�־λ
  TIM_ClearITPendingBit (TPAD_TIM, TPAD_TIM_IT_CCx | TIM_IT_Update);
	// �ȴ����������أ������ݰ�����絽 1.8V ���ҵ�ʱ�򣬾ͻᱻ��Ϊ��������
	while (TIM_GetFlagStatus (TPAD_TIM, TPAD_TIM_IT_CCx) == RESET) 
		{
			// �����ʱ��,ֱ�ӷ��� CNT ��ֵ
			// һ����ʱ�䶼���� ms �������ڣ����ٻᳬ����ʱ����������ֵ
			if (TIM_GetCounter(TPAD_TIM) > TPAD_TIM_Period-100) 
				{
					return TIM_GetCounter (TPAD_TIM);
				}
		}
	
	// ��ȡ����ȽϼĴ�����ֵ
	return TPAD_TIM_GetCapturex_FUN(TPAD_TIM);
}

uint16_t Tpad_Get_MAX(uint8_t n)
{
	uint16_t max=0,temp=0;
	max=Tpad_Get_Val();
	while(n--)
	{
		temp=Tpad_Get_Val();
		if(temp>max)
			max=temp;
	}
	return max;
}

uint8_t Tpad_Init(void)
{
	uint16_t temp;
	TIM_Init(); 
	
	temp = Tpad_Get_MAX(8);
	if(temp>TPAD_DEFAULT_VAL_MIN && temp<TPAD_DEFAULT_VAL_MAX)
	{
		Tpad_default_value = temp;
		printf("��õĿ��س��ʱ��Ϊ%d\n",Tpad_default_value);
		return 0;
	}
	else
		return 1;
}

uint8_t Tpad_Scan(void)
{
	static uint8_t flag=0;
	uint16_t value=0;
	SysTick_Delay_Ms(50);
	value = Tpad_Get_MAX(8);
	if(flag==0 && value>Tpad_default_value+TPAD_GATE_VAL)
	{
		value = Tpad_Get_MAX(8);
		if(value>Tpad_default_value+TPAD_GATE_VAL)
		{
			flag=1;
			printf("�㰴��\n");
			return 1;
		}
	}
	else
	{
		if(value<TPAD_DEFAULT_VAL_MAX)
		{
			flag=0;
		}
	}
	return 0;
}



