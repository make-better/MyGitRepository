#include "bsp_spi.h"
#include <stdio.h>

uint16_t DataBuf[16] = {0};
static __IO uint32_t  TimeOut = SPIT_LONG_TIMEOUT;   

static void SPI_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//���˿�ʱ��
	SPI_GPIO_APBxClkCmd(SPI_GPIO_CLK,ENABLE);
	//����MOSI�˿�
	GPIO_InitStructure.GPIO_Pin = SPI_MOSI_GPIO_PIN;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI_MOSI_GPIO_PORT,&GPIO_InitStructure);
	//����MISO�˿�
	GPIO_InitStructure.GPIO_Pin = SPI_MISO_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����������ܼ�һ����!!
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(SPI_MISO_GPIO_PORT,&GPIO_InitStructure);
	//����SCK�˿�
	GPIO_InitStructure.GPIO_Pin = SPI_SCK_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI_SCK_GPIO_PORT,&GPIO_InitStructure);
	//����CS�˿�
	GPIO_InitStructure.GPIO_Pin = SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI_CS_PORT,&GPIO_InitStructure);
	
}

static void SPI_Config(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	//��SPIʱ��
	SPI_APBxClkCmd(SPI_CLK,ENABLE);
	//����SPI�ṹ��
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;//�ӻ�ģʽ
	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;//����NSSΪ��ʱ����CLK���Ű��������źŲ��ȶ�
	
	SPI_Init(SPIX,&SPI_InitStructure);
	//���������ж�
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


//SPI_FLASH��ʼ������
void SPI_M_M_Init(void)
{
	SPI_GPIO_Config();
	SPI_Config();
	SPI_NVIC_Configuration();
}

//SPI ����һ���ֽ�,����һ���ֽ�
uint8_t SPI_Send_Byte(u16 data)
{
	TimeOut = SPIT_LONG_TIMEOUT;
	while(SPI_I2S_GetFlagStatus(SPIX,SPI_I2S_FLAG_TXE) == RESET)
	{
		if(TimeOut-- == 0){DEBUG("���ͳ�ʱ");return 0;}		
	}
	//printf("����\n");
	SPI_I2S_SendData(SPIX,data);//��dataд��SPIX->DR
//	while(SPI_I2S_GetFlagStatus(SPIX,SPI_I2S_FLAG_RXNE) == RESET)
//	{
//		if(TimeOut-- == 0){DEBUG("���ճ�ʱ");return 0;}		
//	}
	return SPI_I2S_ReceiveData(SPIX);
}

