#include "stm32f10x.h"
#include "bsp_led.h"

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
		delay(0x5fffff);
		LED_B(ON);
		delay(0x5fffff);
//		LED_R(ON);
//		delay(0xfffff);
		LED_B(OFF);
		delay(0x5fffff);
		LED_G(OFF);
		delay(0x5fffff);
//		LED_R(OFF);
//		delay(0xfffff);
	}

}
