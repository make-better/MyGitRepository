#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_systick.h"




int main(void)
{
	LED_Configer();
	SysTick_Init();
	while(1)
	{
		LED_G(ON);
	  delay_ms(50);
		LED_B(ON);
		delay_ms(50);
		LED_R(ON);
		delay_ms(50);
		LED_B(OFF);
		delay_ms(50);
		LED_G(OFF);
		delay_ms(50);
		LED_R(OFF);
		delay_ms(50);
	}

}
