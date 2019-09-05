#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_dma_mtm.h"

extern const uint16_t aSRC_Const_Buffer[BUFFER_SIZE];
extern uint16_t aDST_Buffer[BUFFER_SIZE];

void delay(unsigned int n)
{
	while(n--);
}


int main(void)
{
	LED_Configer();
	DMA_Configer();
	LED_G(ON);
	LED_R(ON);
	delay(0x2fffff);
	while(DMA_GetFlagStatus(DMA_FLAG_TC)==RESET);
	if(Buffercmp(aSRC_Const_Buffer,aDST_Buffer,BUFFER_SIZE) == 1 )
	{
		LED_G(ON);
		LED_R(OFF);
		LED_B(OFF);
	}
	else
	{
		LED_G(OFF);
		LED_R(ON);
		LED_B(OFF);
	}
	
	while(1)
	{
		
	}

}
