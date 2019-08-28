#include "bsp_usart.h"


//uint8_t n[16]={0xff,0xfe,0xfd,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
int main(void)
{
	USART_Config();
	Usart_SendByte(DEBUG_USARTx,'\n');
	//Usart_SendHalfWord( DEBUG_USARTx,64);
	//Usart_SendArray( DEBUG_USARTx,n,10);
	Usart_SendStr( DEBUG_USARTx,"Hello World\n");
	printf("hello world!!!\n");
	while(1)
	{
		
	}

}
