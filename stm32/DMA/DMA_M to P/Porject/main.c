#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_dma_mtp.h"

extern uint8_t array[BUFFER_SIZE];
void delay(unsigned int n)
{
	while(n--);
}


int main(void)
{
	uint16_t i=0;
	//初始化LED
	LED_Configer();
	//初始化DMA
	DMA_Configer();
	//填充数组
	for(;i<BUFFER_SIZE;i++)
	{
		array[i]='Q';
	}	
	//初始化USART
  USART_Config();
	//使USART向DMA发出TX请求
	USART_DMACmd(DEBUG_USARTx,USART_DMAReq_Tx,ENABLE);
	
	while(1)
	{
		LED_G(ON);
		delay(0xfffff);
		LED_B(ON);
		delay(0xfffff);
		LED_R(ON);
		delay(0xfffff);
		LED_B(OFF);
		delay(0xfffff);
		LED_G(OFF);
		delay(0xfffff);
		LED_R(OFF);
		delay(0xfffff);
	}

}
