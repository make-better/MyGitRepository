#include "bsp_usart.h"
#include "bsp_spi.h"
#define N   300      
uint16_t readbuffer[N];
uint16_t writebuffer[N];
uint32_t addr = 0;
int main(void)
{
	uint32_t readid;
	uint32_t i=0;
	USART_Config();
	FLSAH_SPI_Config();
	readid = GET_FLASH_DEVICE_ID();
	printf("\r\n FLASH ID : 0x%x",readid);
	readid = GET_FLASH_JEDEC_ID();
	printf("\r\n FLASH JEDEC ID : 0x%x",readid);
	printf("\r\n Flash 清除扇区\r\n");
	Flash_Sector_Erase(addr);
	printf("\r\n Flash 写扇区\r\n");
	for(i=0;i<N;i++)
	{
		writebuffer[i]=i;
	}
	Flash_Buffer_Write(addr,(void *)writebuffer,sizeof(writebuffer));
	printf("\r\n Flash 读扇区\r\n");
	Flash_Read_Data(addr,(void *)readbuffer,sizeof(readbuffer));
	for(i=0;i<N;i++)
	{
		printf("%d ",readbuffer[i]);
	}
	printf("\n");
	while(1)
	{		
	}
}
