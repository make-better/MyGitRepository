#include "bsp_spi.h"
#include <stdio.h>

static __IO uint32_t  FLASHTimeOut = SPIT_LONG_TIMEOUT;   

static void SPI_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//���˿�ʱ��
	FLASH_SPI_GPIO_APBxClkCmd(FLASH_SPI_GPIO_CLK,ENABLE);
	//����MOSI�˿�
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MOSI_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(FLASH_SPI_MOSI_GPIO_PORT,&GPIO_InitStructure);
	//����MISO�˿�
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MISO_GPIO_PIN;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(FLASH_SPI_MISO_GPIO_PORT,&GPIO_InitStructure);
	//����SCK�˿�
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_SCK_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(FLASH_SPI_SCK_GPIO_PORT,&GPIO_InitStructure);
	//����CS�˿�
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(FLASH_SPI_CS_PORT,&GPIO_InitStructure);
	FLASH_SPI_CS_HIGH;
}

static void SPI_FLASH_Config(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	//��SPIʱ��
	FLASH_SPI_APBxClkCmd(FLASH_SPI_CLK,ENABLE);
	//����SPI�ṹ��
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CRCPolynomial = 0;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	
	SPI_Init(FLASH_SPIx,&SPI_InitStructure);
	SPI_Cmd(FLASH_SPIx,ENABLE);
}




//SPI_FLASH��ʼ������
void FLSAH_SPI_Config(void)
{
	SPI_GPIO_Config();
	SPI_FLASH_Config();
}

//SPI ����һ���ֽ�,����һ���ֽ�
uint8_t SPI_Send_Byte(u8 data)
{
	FLASHTimeOut = SPIT_LONG_TIMEOUT;
	while(SPI_I2S_GetFlagStatus(FLASH_SPIx,SPI_I2S_FLAG_TXE) == RESET)
	{
		if(FLASHTimeOut-- == 0){FLASH_DEBUG("���ͳ�ʱ");return 0;}		
	}
	SPI_I2S_SendData(FLASH_SPIx,data);
	FLASHTimeOut = SPIT_LONG_TIMEOUT;
	while(SPI_I2S_GetFlagStatus(FLASH_SPIx,SPI_I2S_FLAG_RXNE) == RESET)
	{
		if(FLASHTimeOut-- == 0){FLASH_DEBUG("���ճ�ʱ");return 0;}
	}
	return SPI_I2S_ReceiveData(FLASH_SPIx);
}

//SPI��һ���ֽ�
uint8_t SPI_Read_Byte(void)
{
	return SPI_Send_Byte(Dummy_Byte);
}

//����flash���豸id
u32 GET_FLASH_DEVICE_ID(void)
{
	uint32_t id=0;
	FLASH_SPI_CS_LOW;
	SPI_Send_Byte(0xAB);
  SPI_Send_Byte(Dummy_Byte);
  SPI_Send_Byte(Dummy_Byte);
  SPI_Send_Byte(Dummy_Byte);
	id = SPI_Send_Byte(Dummy_Byte);
	FLASH_SPI_CS_HIGH;
	return id;
}
//����flash���豸id
u32 GET_FLASH_JEDEC_ID(void)
{
	uint32_t id=0;
	FLASH_SPI_CS_LOW;
	SPI_Send_Byte(0x9f);
	id |= SPI_Read_Byte();
	id <<= 8;
	id |= SPI_Read_Byte();
	id <<= 8;
	id |= SPI_Read_Byte();
	FLASH_SPI_CS_HIGH;
	return id;
}

//�����������
void Flash_Sector_Erase(uint32_t address)
{
	Flash_Write_Enable();
	Flash_WaitForWrite();
	FLASH_SPI_CS_LOW;
	SPI_Send_Byte(Sector_Erase);
	SPI_Send_Byte(address>>16&0xff);
	SPI_Send_Byte(address>>8&0xff);
	SPI_Send_Byte(address&0xff);
	FLASH_SPI_CS_HIGH;
	Flash_WaitForWrite();
}
//дʹ��
void Flash_Write_Enable(void)
{
	FLASH_SPI_CS_LOW;
	SPI_Send_Byte(Write_Enable);
	FLASH_SPI_CS_HIGH;
}

//���ݶ�������
void Flash_Read_Data(uint32_t address,uint8_t *databuffer,uint32_t numtoread)
{
	FLASH_SPI_CS_LOW;
	SPI_Send_Byte(Read_Data);
	SPI_Send_Byte(address>>16&0xff);
	SPI_Send_Byte(address>>8&0xff);
	SPI_Send_Byte(address&0xff);
	while(numtoread--) 
	{
		*databuffer = SPI_Read_Byte();
		databuffer++;
	}
	FLASH_SPI_CS_HIGH;
}

//�ȴ�FLASH�ڲ��������
void Flash_WaitForWrite(void)
{
	uint8_t status=0;
	FLASH_SPI_CS_LOW;
	SPI_Send_Byte(Read_Status_Register1);
	do
	{
		status = SPI_Read_Byte();
	}while((status&0x01) == 0x01);
	FLASH_SPI_CS_HIGH;
}


//����д�����
void Flash_Write_Data(uint32_t address,uint8_t *databuffer,uint16_t numtowrite)
{
	uint16_t i=0;
	Flash_Write_Enable();
	FLASH_SPI_CS_LOW;
	SPI_Send_Byte(Page_Program);
	SPI_Send_Byte(address>>16&0xff);
	SPI_Send_Byte(address>>8&0xff);
	SPI_Send_Byte(address&0xff);
	for(i=0;i<numtowrite;i++) 
	{
		SPI_Send_Byte(databuffer[i]);
	}
	FLASH_SPI_CS_HIGH;
	Flash_WaitForWrite();
}

//����д�����
void Flash_Buffer_Write(uint32_t address,uint8_t *databuffer,uint32_t numtowrite)
{
	uint8_t pagenum = 0;
	uint8_t singlenum = 0;
	pagenum = numtowrite/256;
	singlenum = numtowrite%256;
	while(pagenum--) //��д��ҳ��
	{
		Flash_Write_Data(address,databuffer,(uint16_t)256);
		address += 256;
		databuffer += 256;
	}
	Flash_Write_Data(address,databuffer,(uint16_t)singlenum);
}

