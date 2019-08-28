#include "bsp_usart.h"
#include "bsp_i2c.h"


int main(void)
{
	
	uint8_t n[256]={0};
	uint8_t read[256]={0};
	uint16_t i;
	uint16_t startAddress=0; //д�����ʼ��ַ
	uint16_t N=20;     //N��ʾд���ֽ���
	USART_Config();
	EEPROM_I2C_Config();
	Usart_SendStr( DEBUG_USARTx,"EEPROM_I2C��д����\n");
//	EEPROM_I2C_Read(0,&n,1);
//	n++;
//	printf("���ǵ�%d�ο���\n",n);
//	EEPROM_I2C_ByteWrite(0,&n);
//	EEPROM_IIC_EASY_TEST();
	for(i=0;i<500;i++)
	{
		n[i] = i;
	}
	IIC_EEPROM_BufferWrite(startAddress,n,N);
	printf("\r\n����д�����\r\n");
	I2C_EE_WaitEepromStandbyState();
	EEPROM_I2C_Read(startAddress,read,N);
	for(i=0;i<N;i++)
	{
		if(n[i]!=read[i])
		{
			printf("��д����\n");
		}
		printf("%d \t",read[i]);
	}
	printf("\n");
	while(1)
	{
		
	}

}
