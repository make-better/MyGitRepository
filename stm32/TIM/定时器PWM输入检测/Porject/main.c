#include "stm32f10x.h"
#include "bsp_exti.h"
#include "bsp_led.h"
#include "bsp_advance_tim.h"
#include "bsp_general_Tim.h"
#include "bsp_usart.h"
#include "bsp_exti.h"

void delay(uint32_t n)
{
	while(n--);
}

int main(void)
{
	LED_Configer();
	EXTI_Configer();
	USART_Config();
	GENERAL_TIM_Init();
	Advance_TIM_Init();
	printf("高级定时器输入脉冲检测\r\n");
	while(1)
	{
		
	}

}
