#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_advance_tim.h"
void delay(uint32_t n)
{
	while(n--);
}
uint16_t time=0;
int main(void)
{
	LED_Configer();
	Advance_TIM_Init();
	while(1)
	{
		
	}

}
