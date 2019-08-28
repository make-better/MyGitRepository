#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_key.h"

void IWDG_Config(uint8_t prv,uint16_t rlv);
void IWDG_Feed(void);

void delay(unsigned int n)
{
	while(n--);
}


int main(void)
{
	LED_Configer();
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{
		LED_R(ON);
		RCC_ClearFlag();
	}
	else
		LED_B(ON);
	
	delay(0xfffff);
	Key_Configer();
	//喂狗时间大约1S
	IWDG_Config(IWDG_Prescaler_64,625);
	while(1)
	{
		//按下KEY1喂狗
		if(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN))
		{
			LED_G(ON);
			LED_R(OFF);
			IWDG_Feed();
		}
		if(Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN))
			LED_R_TOGGLE;

	}

}

//IWDG配置函数Tout = prv/40 * rlv (s)
void IWDG_Config(uint8_t prv,uint16_t rlv)
{
	//使能PR和RLR可写
	IWDG_WriteAccessCmd(ENABLE);
	//设置预分频器值
	IWDG_SetPrescaler(prv);
	//设置重装载值
	IWDG_SetReload(rlv);
	//把重装值放入计数器中
	IWDG_ReloadCounter();
	//使能独立看门狗
	IWDG_Enable();
}

//喂狗函数
void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}

