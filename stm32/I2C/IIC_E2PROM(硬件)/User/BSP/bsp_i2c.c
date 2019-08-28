#include "bsp_i2c.h"
#include <stdio.h>

static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;   


//I2C����ӦGPIO��ʼ������
void EEPROM_I2C_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
	
	// ��I2CGPIO��ʱ��
	EEPROM_I2C_GPIO_APBxClkCmd(EEPROM_I2C_GPIO_CLK, ENABLE);
	//��I2C��ʱ��
	EEPROM_I2C_APBxClkCmd(EEPROM_I2C_CLK,ENABLE);
	//����SCL
	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
	//����SDA
	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SDA_GPIO_PIN;
	GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
	
	//����I2C
	I2C_InitStructure.I2C_AcknowledgedAddress = 0xa0;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = I2Cx_OWN_ADDRESS7;
	I2C_Init(EEPROM_I2Cx,&I2C_InitStructure);
	I2C_Cmd(EEPROM_I2Cx, ENABLE);
	
}

//�ȴ�EEPROM�ڲ�����д����ɣ��ɹ�����1�����򷵻�0
uint8_t I2C_EE_WaitEepromStandbyState(void)
{
 vu16 SR1_Tmp = 0;

 do {
 /* ������ʼ�ź� */
 I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);
 I2CTimeout = I2CT_LONG_TIMEOUT;
 /* �� I2C1 SR1 �Ĵ��� */
 SR1_Tmp = I2C_ReadRegister(EEPROM_I2Cx, I2C_Register_SR1);

 /* ���� EEPROM ��ַ + д���� */
 I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_Address,I2C_Direction_Transmitter);
 I2CTimeout--;
	if(I2CTimeout==0)
	{
		EEPROM_DEBUG("EEPROM�ڲ�д�볬ʱ");
		return 0;
	}		
 }while ((!(I2C_ReadRegister(EEPROM_I2Cx, I2C_Register_SR1) & 0x0002))&&I2CTimeout);

 /* ��� AF λ */
 I2C_ClearFlag(EEPROM_I2Cx, I2C_FLAG_AF);
 /* ����ֹͣ�ź� */
 I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);
 return 1;
 }

 
//EEPROM���ֽ�д�����д���ַ��*д�����ݣ����ɹ�����1������0
uint8_t EEPROM_I2C_ByteWrite(uint8_t writeaddress,uint8_t *data)
{
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//������æµ��ȴ�
	while((I2C_GetFlagStatus(EEPROM_I2Cx,I2C_FLAG_BUSY)== SET)&&I2CTimeout)
	{
		if((I2CTimeout--)==0)
		{
			EEPROM_DEBUG("I2C����æµ");
			return 0;
		}
	}
	
	//������ʼ�źŲ��ȴ�EV5
	I2C_GenerateSTART(EEPROM_I2Cx,ENABLE);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while((I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)&&I2CTimeout)
	{
		if((I2CTimeout--)==0)
		{
			EEPROM_DEBUG("��ʼ�ź�����Ӧ");
			return 0;
		}
	}
	
	//����EEPROM 7λд��ַ��ַ���ȴ�EV6
	I2C_Send7bitAddress(EEPROM_I2Cx,EEPROM_Address,I2C_Direction_Transmitter);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while((I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS)&&I2CTimeout)
		{
		if((I2CTimeout--)==0)
		{
			EEPROM_DEBUG("EEPROMд��ַ����Ӧ");
			return 0;
		}
	}
		
	//����EEPROM�ڴ��ַ���ȴ�EV8
	I2C_SendData(EEPROM_I2Cx,writeaddress);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while((I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)&&I2CTimeout)
		{
		if((I2CTimeout--)==0)
			{
				EEPROM_DEBUG("EEPROM�ڴ��ַ����Ӧ");
				return 0;
			}
		}

	//����Ҫд�������,�ȴ�EV8
	I2C_SendData(EEPROM_I2Cx,*data);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while((I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS)&&I2CTimeout)
		{
		if((I2CTimeout--)==0)
			{
				EEPROM_DEBUG("EEPROMд������Ӧ");
				return 0;
			}
		}

	//����ֹͣ�ź�
	I2C_GenerateSTOP(EEPROM_I2Cx,ENABLE);
	return 1;
	
}
//EEPROM���ݶ�ȡ����Ҫ���ĵ�ַ��*���ݴ�Ŵ�����ȡ��λ����,�ɹ�����1������0
uint8_t EEPROM_I2C_Read(uint8_t readaddress,uint8_t *data,uint16_t num)
{
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//������æµ��ȴ�
	while((I2C_GetFlagStatus(EEPROM_I2Cx,I2C_FLAG_BUSY)== SET)&&I2CTimeout)
	{
		if((I2CTimeout--)==0)
		{
			EEPROM_DEBUG("I2C����æµ");
			return 0;
		}
	}	
	//������ʼ�źŲ��ȴ�EV5
	I2C_GenerateSTART(EEPROM_I2Cx,ENABLE);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while((I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)&&I2CTimeout)
	{
		if((I2CTimeout--)==0)
		{
			EEPROM_DEBUG("��ʼ�ź�����Ӧ");
			return 0;
		}
	}
	
	
	//����EEPROM 7λд��ַ��ַ���ȴ�EV6
	I2C_Send7bitAddress(EEPROM_I2Cx,EEPROM_Address,I2C_Direction_Transmitter);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while((I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS)&&I2CTimeout)
	{
		if((I2CTimeout--)==0)
		{
			EEPROM_DEBUG("EEPROMд��ַ����Ӧ");
			return 0;
		}
	}
	
	
	/*ͨ���������� PE λ��� EV6 �¼� */
  I2C_Cmd(EEPROM_I2Cx, ENABLE);
	
	//����EEPROM�ڴ��ַ���ȴ�EV8
	I2C_SendData(EEPROM_I2Cx,readaddress);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while((I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)&&I2CTimeout)
		{
		if((I2CTimeout--)==0)
			{
				EEPROM_DEBUG("EEPROM�ڴ��ַ����Ӧ");
				return 0;
			}
		}
	//���͵ڶ�����ʼ�źŲ��ȴ�EV5
	I2C_GenerateSTART(EEPROM_I2Cx,ENABLE);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while((I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)&&I2CTimeout)
	{
		if((I2CTimeout--)==0)
		{
			EEPROM_DEBUG("��ʼ�ź�����Ӧ");
			return 0;
		}
	}
	
	//����EEPROM����ַ�ȴ�EV6
	I2C_Send7bitAddress(EEPROM_I2Cx,EEPROM_Address,I2C_Direction_Receiver);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while((I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != SUCCESS)&&I2CTimeout)
	{
		if((I2CTimeout--)==0)
		{
			EEPROM_DEBUG("EEPROM����ַ����Ӧ");
			return 0;
		}
	}
	
	/* ��ȡnum ������*/
 while (num)
 {
	 /*��num=1����ʾ�Ѿ����յ����һ�������ˣ�
	 ���ͷ�Ӧ���źţ���������*/
	 if (num == 1)
	 {
	 /* ���ͷ�Ӧ���ź� */
	 I2C_AcknowledgeConfig(EEPROM_I2Cx, DISABLE);

	 /* ����ֹͣ�ź� */
	 I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);
	 }
	 //�ȴ�EV7
	 I2CTimeout = I2CT_FLAG_TIMEOUT;
	 while ((I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS)&&I2CTimeout)
	{
		if((I2CTimeout--)==0)
		{
			EEPROM_DEBUG("����ӦEV7");
			return 0;
		}
	}

	 /*ͨ�� I2C�����豸�ж�ȡһ���ֽڵ����� */
	 *data = I2C_ReceiveData(EEPROM_I2Cx);
	 /* �洢���ݵ�ָ��ָ����һ����ַ */
	 data++;
	 /* ���������Լ� */
	 num--;
  }
	//ʹ��ACK
 I2C_AcknowledgeConfig(EEPROM_I2Cx, ENABLE);
	return 1;
}


void EEPROM_IIC_EASY_TEST(void)
{
	uint8_t i=0;
	uint8_t a[8]={1,2,3,4,5,6,7,8};
  uint8_t b[8]={0};
	printf("д�����ִ��\n");
	EEPROM_I2C_Page_Write(8,a,8);
	printf("д�����\n");
	I2C_EE_WaitEepromStandbyState();
	printf("��ȡ����ִ��\n");
	EEPROM_I2C_Read(8,b,8);
	printf("��ȡ���\n");
	for(i=0;i<8;i++)
	{
		printf("����%d: %d\n",i,b[i]);
	}
}

//EEPROM���ֽ�д�����д���ַ��*д������,д�뼸����һ�����д��8���ֽ�,�׵�ַ��8����,�ɹ�����1������0
uint8_t EEPROM_I2C_Page_Write(uint8_t writeaddress,uint8_t *data,uint8_t num)
{
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//������æµ��ȴ�
	while((I2C_GetFlagStatus(EEPROM_I2Cx,I2C_FLAG_BUSY)== SET)&&I2CTimeout)
	{
		if((I2CTimeout--)==0)
		{
			EEPROM_DEBUG("I2C����æµ");
			return 0;
		}
	}
	
	//������ʼ�źŲ��ȴ�EV5
	I2C_GenerateSTART(EEPROM_I2Cx,ENABLE);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while((I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)&&I2CTimeout)
	{
		if((I2CTimeout--)==0)
		{
			EEPROM_DEBUG("��ʼ�ź�����Ӧ");
			return 0;
		}
	}
	
	
	//����EEPROMд��ַ���ȴ�EV6
	I2C_Send7bitAddress(EEPROM_I2Cx,EEPROM_Address,I2C_Direction_Transmitter);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while((I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS)&&I2CTimeout)
	{
		if((I2CTimeout--)==0)
		{
			EEPROM_DEBUG("EEPROMд��ַ����Ӧ");
			return 0;
		}
	}
	
	//����EEPROM�ڴ�ĵ�ַ,�ȴ�EV8
	I2C_SendData(EEPROM_I2Cx,writeaddress);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while((I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)&&I2CTimeout)
		{
		if((I2CTimeout--)==0)
			{
				EEPROM_DEBUG("EEPROM�ڴ��ַ����Ӧ");
				return 0;
			}
		}
		
	while(num)
	{
		//����Ҫд�������
		I2C_SendData(EEPROM_I2Cx,*data);
		I2CTimeout = I2CT_FLAG_TIMEOUT;
		//�ȴ�EV8
		while(I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS && I2CTimeout)
		{
		if((I2CTimeout--)==0)
			{
				EEPROM_DEBUG("EEPROMҳд��ʧ��");
				return 0;
			}
		}
		num--;
		data++;
	}
	//����ֹͣ�ź�
	I2C_GenerateSTOP(EEPROM_I2Cx,ENABLE);
	return 1;
}

//����д��
uint8_t IIC_EEPROM_BufferWrite(uint8_t writeaddress,uint8_t *data,uint16_t num)
{
//	if(writeaddress+num>256)
//	{
//		printf("��ַԽ��\n");
//	}
	uint8_t page_num,single_byte;
		while((writeaddress%I2C_PageSize != 0)&&num)//���ֽ�д�룬ֱ����ַ����Ϊ8��������д��
		{
			I2C_EE_WaitEepromStandbyState();
			EEPROM_I2C_ByteWrite(writeaddress,data);
			writeaddress++;
			data++;
			num--;
		}
		page_num = num/I2C_PageSize;          //�жϻ�����д��ҳ
		single_byte = num%I2C_PageSize;       //�ж��м����ֽ���Ҫ����д��
		while(page_num--)          //��������ҳд��
		{
			I2C_EE_WaitEepromStandbyState();
			EEPROM_I2C_Page_Write(writeaddress,data,I2C_PageSize);
			writeaddress += I2C_PageSize;
			data += I2C_PageSize;
		}
		while(single_byte--)      //��ʣ����������д��
		{
			I2C_EE_WaitEepromStandbyState();
			EEPROM_I2C_ByteWrite(writeaddress,data);
			writeaddress++;
			data++;
		}
		return 1;
	
}

