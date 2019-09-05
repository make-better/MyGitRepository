#include "bsp_tpad.h"
#include "bsp_general_Tim.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"

uint16_t Tpad_default_value=0;

void Tpad_Reset(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//配置CH2为输出模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GENERAL_TIM_CH2_PIN;
	GPIO_Init(GENERAL_TIM_CH2_PORT,&GPIO_InitStructure);
	GPIO_ResetBits(GENERAL_TIM_CH2_PORT,GENERAL_TIM_CH2_PIN);
	
	SysTick_Delay_Ms( 5 );
	//再次改为浮空输入模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GENERAL_TIM_CH2_PORT,&GPIO_InitStructure);
}

uint16_t Tpad_Get_Val(void)
{
	Tpad_Reset();
	//复位后计数器清零
	TIM_SetCounter (TPAD_TIM,0);
	// 清除相关的标志位
  TIM_ClearITPendingBit (TPAD_TIM, TPAD_TIM_IT_CCx | TIM_IT_Update);
	// 等待捕获上升沿，当电容按键充电到 1.8V 左右的时候，就会被认为是上升沿
	while (TIM_GetFlagStatus (TPAD_TIM, TPAD_TIM_IT_CCx) == RESET) 
		{
			// 如果超时了,直接返回 CNT 的值
			// 一般充电时间都是在 ms 级别以内，很少会超过定时器的最大计数值
			if (TIM_GetCounter(TPAD_TIM) > TPAD_TIM_Period-100) 
				{
					return TIM_GetCounter (TPAD_TIM);
				}
		}
	
	// 获取捕获比较寄存器的值
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
		printf("测得的空载充电时间为%d\n",Tpad_default_value);
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
			printf("你按了\n");
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



