#include "stm32f10x.h"
#include "bsp_breath.h"

void delay(uint32_t n)
{
	while(n--);
}

int main(void)
{
	TIM_Breath_Init();
	while(1)
	{
	}

}
