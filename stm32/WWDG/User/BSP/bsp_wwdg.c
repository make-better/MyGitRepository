#include "bsp_wwdg.h"

static void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}

void WWDG_Config(uint8_t wr,uint8_t tr,uint32_t prv)
{
	//�������ڿ��Ź���ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
	//���õݼ���������ֵ
	WWDG_SetCounter(tr);
	//����Ԥ��Ƶ����ֵ
	WWDG_SetPrescaler(prv);
	//�����ϴ���ֵ
	WWDG_SetWindowValue(wr);
	//���ü�������ֵ��ʹ��wwdg
	WWDG_Enable(WWDG_CNT);
	//����жϱ�־
	WWDG_ClearFlag();
	//ʹ���ж�
	NVIC_Config();
	WWDG_EnableIT();
}

void WWDG_Feed(void)
{
	//���õݼ���������ֵ
	WWDG_SetCounter(WWDG_CNT);
}

