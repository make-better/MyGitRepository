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
	//开启窗口看门狗的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
	//设置递减计数器的值
	WWDG_SetCounter(tr);
	//设置预分频器的值
	WWDG_SetPrescaler(prv);
	//设置上窗口值
	WWDG_SetWindowValue(wr);
	//设置计数器的值，使能wwdg
	WWDG_Enable(WWDG_CNT);
	//清除中断标志
	WWDG_ClearFlag();
	//使能中断
	NVIC_Config();
	WWDG_EnableIT();
}

void WWDG_Feed(void)
{
	//设置递减计数器的值
	WWDG_SetCounter(WWDG_CNT);
}

