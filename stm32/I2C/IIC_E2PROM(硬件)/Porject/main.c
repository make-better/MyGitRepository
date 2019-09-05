#include "bsp_usart.h"
#include "bsp_i2c.h"


int main(void)
{
	
	uint8_t n[256]={0};
	uint8_t read[256]={0};
	uint16_t i;
	uint16_t startAddress=0; //写入的起始地址
	uint16_t N=20;     //N表示写入字节数
	USART_Config();
	EEPROM_I2C_Config();
	Usart_SendStr( DEBUG_USARTx,"EEPROM_I2C读写测试\n");
//	EEPROM_I2C_Read(0,&n,1);
//	n++;
//	printf("这是第%d次开机\n",n);
//	EEPROM_I2C_ByteWrite(0,&n);
//	EEPROM_IIC_EASY_TEST();
	for(i=0;i<500;i++)
	{
		n[i] = i;
	}
	IIC_EEPROM_BufferWrite(startAddress,n,N);
	printf("\r\n数组写入完毕\r\n");
	I2C_EE_WaitEepromStandbyState();
	EEPROM_I2C_Read(startAddress,read,N);
	for(i=0;i<N;i++)
	{
		if(n[i]!=read[i])
		{
			printf("读写有误\n");
		}
		printf("%d \t",read[i]);
	}
	printf("\n");
	while(1)
	{
		
	}

}
