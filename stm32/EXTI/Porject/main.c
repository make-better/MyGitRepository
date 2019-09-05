#include "stm32f10x.h"
#include "bsp_led.h"
//#include "bsp_key.h"
#include "bsp_exti.h"

void delay(unsigned int n)
{
	while(n--);
}


int main(void)
{
	LED_Configer();
//	Key_Configer();
	EXTI_Configer();
//	LED_G(ON);
	while(1)
	{
		
	}

}
