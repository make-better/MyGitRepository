#include "stm32f10x.h"
#include "bsp_breath.h"

extern uint32_t color_value;

void delay(uint32_t n)
{
	while(n--);
}

int main(void)
{
	color_value = 0XFFFF00;//»ÆÉ«
	TIM_Breath_Init();
	while(1)
	{
	}

}
