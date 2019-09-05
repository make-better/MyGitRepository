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
		if(GPIO_ReadInputDataBit(LED_GPIO_PORT,DIN_GPIO_PIN) == 1)
		{
			LED_G(ON);
		}
		else
			LED_G(OFF);
	}

}
