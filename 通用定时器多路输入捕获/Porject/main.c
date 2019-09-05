#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_general_Tim.h"
#include "bsp_general_Tim2.h"
#include "bsp_usart.h"

void delay(uint32_t n)
{
	while(n--);
}
int main(void)
{
	USART_Config();
	LED_Configer();
  TIM_Init(); 
	TIM_OutPut_Init();
	printf("\r\n输入捕获实验\r\n");
	while(1)
	{	
		delay(0xff);
		PWM_IC_Print();
	}
}

