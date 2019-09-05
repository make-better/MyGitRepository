#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_key.h"

void delay(unsigned int n)
{
	while(n--);
}


int main(void)
{
	LED_Configer();
	Key_Configer();
	while(1)
	{
		if(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN))
			LED_G_TOGGLE;
		if(Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN))
			LED_R_TOGGLE;

	}

}
