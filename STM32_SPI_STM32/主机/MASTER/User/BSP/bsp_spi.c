#include "bsp_spi.h"
#include <stdio.h>


static __IO uint32_t  TimeOut = SPIT_LONG_TIMEOUT;   

static void SPI_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//开端口时钟
	SPI_GPIO_APBxClkCmd(SPI_GPIO_CLK,ENABLE);
	//配置MOSI端口
	GPIO_InitStructure.GPIO_Pin = SPI_MOSI_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(SPI_MOSI_GPIO_PORT,&GPIO_InitStructure);
	//配置MISO端口
	GPIO_InitStructure.GPIO_Pin = SPI_MISO_GPIO_PIN;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI_MISO_GPIO_PORT,&GPIO_InitStructure);
	//配置SCK端口
	GPIO_InitStructure.GPIO_Pin = SPI_SCK_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(SPI_SCK_GPIO_PORT,&GPIO_InitStructure);
	//配置CS端口
	GPIO_InitStructure.GPIO_Pin = SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_CS_PORT,&GPIO_InitStructure);
	SPI_CS_HIGH;
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
	SPI_InitStructure.SPI_CRCPolynomial = 0;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//主机模式
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	//开启接收中断
	//SPI_I2S_ITConfig(SPIX, SPI_I2S_IT_RXNE, ENABLE);
	
	SPI_Init(SPIX,&SPI_InitStructure);
	SPI_Cmd(SPIX,ENABLE);
}

static void SPI_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
  NVIC_InitStructure.NVIC_IRQChannel = SPIX_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
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
//实际上主机不需要等待从机发送，因为只有在主机发送时，从机才会发送 发送缓冲区 中的东西
uint8_t SPI_Send_Byte(u16 data)
{
	TimeOut = SPIT_LONG_TIMEOUT;
	while(SPI_I2S_GetFlagStatus(SPIX,SPI_I2S_FLAG_TXE) == RESET)//等待发送完毕
	{
		if(TimeOut-- == 0){DEBUG("发送超时");return 0;}	
	}
	SPI_I2S_SendData(SPIX,data);
	TimeOut = SPIT_LONG_TIMEOUT;
	while(SPI_I2S_GetFlagStatus(SPIX,SPI_I2S_FLAG_RXNE) == RESET)//等待接收完毕
	{
		if(TimeOut-- == 0){DEBUG("接收超时");return 0;}		
	}
	return SPI_I2S_ReceiveData(SPIX);
}

//这是一个延时函数
void delay(u32 n)
{
	while(n--);
}
//向另一个mcu发送数据
//num 位置 data 数据
//正常 return 0    异常return 1
uint8_t SPI_Send_Data(u16 num,u16 data)
{
	uint16_t temp = 0;
	printf("发送 num:%d data:%d\n",num,data);
	
	SPI_CS_LOW;//拉低CS
	SPI_Send_Byte(num);
	//加延时是因为从机使用了中断处理数据，相对于SPI来说
	//中断太慢了，所以不加延时函数无法让从机及时回复
	//不过我让从机回复只是为了证明可以双向通行罢了，如果只是发送，完全可以把延时删掉
	delay(0xffff);
	
	temp = SPI_Send_Byte(data);
	if(temp != num)
	{
		printf("num err :%d\n",temp);
		SPI_CS_HIGH;//拉高CS
		return 1;
	}
	
	delay(0xffff);
	
	temp = SPI_Send_Byte(0xff);
	if(temp != data)
	{
		printf("data err :%d\n",temp);
		SPI_CS_HIGH;//拉高CS
		return 1;
	}
	
	SPI_CS_HIGH;//拉高CS
	return 0;
}
