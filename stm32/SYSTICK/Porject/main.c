#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_systick.h"

void delay(unsigned int n)
{
	while(n--);
}


int main(void)
{
	LED_Configer();
	while(1)
	{
		LED_G(ON);
	  SysTick_Delay_ms(500);
		LED_B(ON);
		SysTick_Delay_ms(500);
		LED_R(ON);
		SysTick_Delay_ms(500);
		LED_B(OFF);
		SysTick_Delay_ms(500);
		LED_G(OFF);
		SysTick_Delay_ms(500);
		LED_R(OFF);
		SysTick_Delay_ms(500);
	}

}
