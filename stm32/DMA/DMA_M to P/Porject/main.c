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
	//��ʼ��LED
	LED_Configer();
	//��ʼ��DMA
	DMA_Configer();
	//�������
	for(;i<BUFFER_SIZE;i++)
	{
		array[i]='Q';
	}	
	//��ʼ��USART
  USART_Config();
	//ʹUSART��DMA����TX����
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
