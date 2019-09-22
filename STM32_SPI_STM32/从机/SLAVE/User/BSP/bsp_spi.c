#include "bsp_spi.h"
#include <stdio.h>

uint16_t DataBuf[16] = {0};
static __IO uint32_t  TimeOut = SPIT_LONG_TIMEOUT;   

static void SPI_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//开端口时钟
	SPI_GPIO_APBxClkCmd(SPI_GPIO_CLK,ENABLE);
	//配置MOSI端口
	GPIO_InitStructure.GPIO_Pin = SPI_MOSI_GPIO_PIN;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI_MOSI_GPIO_PORT,&GPIO_InitStructure);
	//配置MISO端口
	GPIO_InitStructure.GPIO_Pin = SPI_MISO_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//这个错误我能记一辈子!!
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(SPI_MISO_GPIO_PORT,&GPIO_InitStructure);
	//配置SCK端口
	GPIO_InitStructure.GPIO_Pin = SPI_SCK_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI_SCK_GPIO_PORT,&GPIO_InitStructure);
	//配置CS端口
	GPIO_InitStructure.GPIO_Pin = SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI_CS_PORT,&GPIO_InitStructure);
	
}

static void SPI_Config(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	//开SPI时钟
	SPI_APBxClkCmd(SPI_CLK,ENABLE);
	//配置SPI结构体
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;//从机模式
	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;//能在NSS为高时忽略CLK，杜邦线连接信号不稳定
	
	SPI_Init(SPIX,&SPI_InitStructure);
	//开启接收中断
	SPI_I2S_ITConfig(SPIX, SPI_I2S_IT_RXNE, ENABLE);
	
	SPI_Cmd(SPIX,ENABLE);
}

static void SPI_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
  NVIC_InitStructure.NVIC_IRQChannel = SPIX_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


//SPI_FLASH初始化函数
void SPI_M_M_Init(void)
{
	SPI_GPIO_Config();
	SPI_Config();
	SPI_NVIC_Configuration();
}

//SPI 发送一个字节,接收一个字节
uint8_t SPI_Send_Byte(u16 data)
{
	TimeOut = SPIT_LONG_TIMEOUT;
	while(SPI_I2S_GetFlagStatus(SPIX,SPI_I2S_FLAG_TXE) == RESET)
	{
		if(TimeOut-- == 0){DEBUG("发送超时");return 0;}		
	}
	//printf("发送\n");
	SPI_I2S_SendData(SPIX,data);//把data写入SPIX->DR
//	while(SPI_I2S_GetFlagStatus(SPIX,SPI_I2S_FLAG_RXNE) == RESET)
//	{
//		if(TimeOut-- == 0){DEBUG("接收超时");return 0;}		
//	}
	return SPI_I2S_ReceiveData(SPIX);
}

