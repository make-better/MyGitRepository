#include "stm32f10x.h"
#include "bsp_dac.h"

void delay(unsigned int n)
{
	while(n--);
}


int main(void)
{
	DAC_Mode_Init();
	while(1)
	{
		
	}

}
