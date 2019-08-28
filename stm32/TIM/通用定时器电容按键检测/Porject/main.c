#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_general_Tim.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include "bsp_tpad.h"
int main(void)
{
	USART_Config();
	printf("电容按键实验\r\n");
	LED_Configer();
  Tpad_Init();
		while(1)
	{	
		if(Tpad_Scan())
		{
			LED_G(ON);
		}
		else
			LED_G(OFF);
	}
}

