#include "bsp_usart.h"
#include "bsp_spi.h"
uint16_t TmpDataBuf[16];
void delay(int n)
{
	while(n--);
}

int main(void)
{
	SPI_M_M_Init();
	USART_Config();
	
	printf("¿ª»ú!!!\n");

	while(1)
	{

	}

}
