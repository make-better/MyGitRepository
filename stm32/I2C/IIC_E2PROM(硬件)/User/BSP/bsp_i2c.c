#include "bsp_i2c.h"
#include <stdio.h>

static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;   


//I2C及相应GPIO初始化函数
void EEPROM_I2C_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
	
	// 打开I2CGPIO的时钟
	EEPROM_I2C_GPIO_APBxClkCmd(EEPROM_I2C_GPIO_CLK, ENABLE);
	//打开I2C的时钟
	EEPROM_I2C_APBxClkCmd(EEPROM_I2C_CLK,ENABLE);
	//配置SCL
	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
	//配置SDA
	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SDA_GPIO_PIN;
	GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
	
	//配置I2C
	I2C_InitStructure.I2C_AcknowledgedAddress = 0xa0;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = I2Cx_OWN_ADDRESS7;
	I2C_Init(EEPROM_I2Cx,&I2C_InitStructure);
	I2C_Cmd(EEPROM_I2Cx, ENABLE);
	
}

//等待EEPROM内部数据写入完成，成功返回1，否则返回0
uint8_t I2C_EE_WaitEepromStandbyState(void)
{
 vu16 SR1_Tmp = 0;

 do {
 /* 发送起始信号 */
 I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);
 I2CTimeout = I2CT_LONG_TIMEOUT;
 /* 读 I2C1 SR1 寄存器 */
 SR1_Tmp = I2C_ReadRegister(EEPROM_I2Cx, I2C_Register_SR1);

 /* 发送 EEPROM 地址 + 写方向 */
 I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_Address,I2C_Direction_Transmitter);
 I2CTimeout--;
	if(I2CTimeout==0)
	{
		EEPROM_DEBUG("EEPROM内部写入超时");
		return 0;
	}		
 }while ((!(I2C_ReadRegister(EEPROM_I2Cx, I2C_Register_SR1) & 0x0002))&&I2CTimeout);

 /* 清除 AF 位 */
 I2C_ClearFlag(EEPROM_I2Cx, I2C_FLAG_AF);
 /* 发送停止信号 */
 I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);
 return 1;
 }

 
//EEPROM单字节写入程序（写入地址，*写入数据），成功返回1，否则0
uint8_t EEPROM_I2C_ByteWrite(uint8_t writeaddress,uint8_t *data)
{
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//若总线忙碌则等待
	while((I2C_GetFlagStatus(EEPROM_I2Cx,I2C_FLAG_BUSY)== SET)&&I2CTimeout)
	{
		if((I2CTimeout--)==0)
		{
			EEPROM_DEBUG("I2C总线忙碌");
			return 0;
		}
	}
	
	//发送起始信号并等待EV5
	I2C_GenerateSTART(EEPROM_I2Cx,ENABLE);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while((I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)&&I2CTimeout)
	{
		if((I2CTimeout--)==0)
		{
			EEPROM_DEBUG("起始信号无响应");
			return 0;
		}
	}
	
	//发送EEPROM 7位写地址地址，等待EV6
	I2C_Send7bitAddress(EEPROM_I2Cx,EEPROM_Address,I2C_Direction_Transmitter);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while((I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS)&&I2CTimeout)
		{
		if((I2CTimeout--)==0)
		{
			EEPROM_DEBUG("EEPROM写地址无响应");
			return 0;
		}
	}
		
	//发送EEPROM内存地址，等待EV8
	I2C_SendData(EEPROM_I2Cx,writeaddress);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while((I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)&&I2CTimeout)
		{
		if((I2CTimeout--)==0)
			{
				EEPROM_DEBUG("EEPROM内存地址无响应");
				return 0;
			}
		}

	//发送要写入的数据,等待EV8
	I2C_SendData(EEPROM_I2Cx,*data);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while((I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS)&&I2CTimeout)
		{
		if((I2CTimeout--)==0)
			{
				EEPROM_DEBUG("EEPROM写入无响应");
				return 0;
			}
		}

	//发送停止信号
	I2C_GenerateSTOP(EEPROM_I2Cx,ENABLE);
	return 1;
	
}
//EEPROM数据读取程序（要读的地址，*数据存放处，读取的位数）,成功返回1，否则0
uint8_t EEPROM_I2C_Read(uint8_t readaddress,uint8_t *data,uint16_t num)
{
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//若总线忙碌则等待
	while((I2C_GetFlagStatus(EEPROM_I2Cx,I2C_FLAG_BUSY)== SET)&&I2CTimeout)
	{
		if((I2CTimeout--)==0)
		{
			EEPROM_DEBUG("I2C总线忙碌");
			return 0;
		}
	}	
	//发送起始信号并等待EV5
	I2C_GenerateSTART(EEPROM_I2Cx,ENABLE);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while((I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)&&I2CTimeout)
	{
		if((I2CTimeout--)==0)
		{
			EEPROM_DEBUG("起始信号无响应");
			return 0;
		}
	}
	
	
	//发送EEPROM 7位写地址地址，等待EV6
	I2C_Send7bitAddress(EEPROM_I2Cx,EEPROM_Address,I2C_Direction_Transmitter);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while((I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS)&&I2CTimeout)
	{
		if((I2CTimeout--)==0)
		{
			EEPROM_DEBUG("EEPROM写地址无响应");
			return 0;
		}
	}
	
	
	/*通过重新设置 PE 位清除 EV6 事件 */
  I2C_Cmd(EEPROM_I2Cx, ENABLE);
	
	//发送EEPROM内存地址，等待EV8
	I2C_SendData(EEPROM_I2Cx,readaddress);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while((I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)&&I2CTimeout)
		{
		if((I2CTimeout--)==0)
			{
				EEPROM_DEBUG("EEPROM内存地址无响应");
				return 0;
			}
		}
	//发送第二次起始信号并等待EV5
	I2C_GenerateSTART(EEPROM_I2Cx,ENABLE);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while((I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)&&I2CTimeout)
	{
		if((I2CTimeout--)==0)
		{
			EEPROM_DEBUG("起始信号无响应");
			return 0;
		}
	}
	
	//发送EEPROM读地址等待EV6
	I2C_Send7bitAddress(EEPROM_I2Cx,EEPROM_Address,I2C_Direction_Receiver);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while((I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != SUCCESS)&&I2CTimeout)
	{
		if((I2CTimeout--)==0)
		{
			EEPROM_DEBUG("EEPROM读地址无响应");
			return 0;
		}
	}
	
	/* 读取num 个数据*/
 while (num)
 {
	 /*若num=1，表示已经接收到最后一个数据了，
	 发送非应答信号，结束传输*/
	 if (num == 1)
	 {
	 /* 发送非应答信号 */
	 I2C_AcknowledgeConfig(EEPROM_I2Cx, DISABLE);

	 /* 发送停止信号 */
	 I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);
	 }
	 //等待EV7
	 I2CTimeout = I2CT_FLAG_TIMEOUT;
	 while ((I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS)&&I2CTimeout)
	{
		if((I2CTimeout--)==0)
		{
			EEPROM_DEBUG("无响应EV7");
			return 0;
		}
	}

	 /*通过 I2C，从设备中读取一个字节的数据 */
	 *data = I2C_ReceiveData(EEPROM_I2Cx);
	 /* 存储数据的指针指向下一个地址 */
	 data++;
	 /* 接收数据自减 */
	 num--;
  }
	//使能ACK
 I2C_AcknowledgeConfig(EEPROM_I2Cx, ENABLE);
	return 1;
}


void EEPROM_IIC_EASY_TEST(void)
{
	uint8_t i=0;
	uint8_t a[8]={1,2,3,4,5,6,7,8};
  uint8_t b[8]={0};
	printf("写入程序执行\n");
	EEPROM_I2C_Page_Write(8,a,8);
	printf("写入完毕\n");
	I2C_EE_WaitEepromStandbyState();
	printf("读取程序执行\n");
	EEPROM_I2C_Read(8,b,8);
	printf("读取完毕\n");
	for(i=0;i<8;i++)
	{
		printf("数据%d: %d\n",i,b[i]);
	}
}

//EEPROM多字节写入程序（写入地址，*写入数据,写入几个）一次最多写入8个字节,首地址与8对齐,成功返回1，否则0
uint8_t EEPROM_I2C_Page_Write(uint8_t writeaddress,uint8_t *data,uint8_t num)
{
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//若总线忙碌则等待
	while((I2C_GetFlagStatus(EEPROM_I2Cx,I2C_FLAG_BUSY)== SET)&&I2CTimeout)
	{
		if((I2CTimeout--)==0)
		{
			EEPROM_DEBUG("I2C总线忙碌");
			return 0;
		}
	}
	
	//发送起始信号并等待EV5
	I2C_GenerateSTART(EEPROM_I2Cx,ENABLE);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while((I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)&&I2CTimeout)
	{
		if((I2CTimeout--)==0)
		{
			EEPROM_DEBUG("起始信号无响应");
			return 0;
		}
	}
	
	
	//发送EEPROM写地址，等待EV6
	I2C_Send7bitAddress(EEPROM_I2Cx,EEPROM_Address,I2C_Direction_Transmitter);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while((I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS)&&I2CTimeout)
	{
		if((I2CTimeout--)==0)
		{
			EEPROM_DEBUG("EEPROM写地址无响应");
			return 0;
		}
	}
	
	//发送EEPROM内存的地址,等待EV8
	I2C_SendData(EEPROM_I2Cx,writeaddress);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while((I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)&&I2CTimeout)
		{
		if((I2CTimeout--)==0)
			{
				EEPROM_DEBUG("EEPROM内存地址无响应");
				return 0;
			}
		}
		
	while(num)
	{
		//发送要写入的数据
		I2C_SendData(EEPROM_I2Cx,*data);
		I2CTimeout = I2CT_FLAG_TIMEOUT;
		//等待EV8
		while(I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS && I2CTimeout)
		{
		if((I2CTimeout--)==0)
			{
				EEPROM_DEBUG("EEPROM页写入失败");
				return 0;
			}
		}
		num--;
		data++;
	}
	//发送停止信号
	I2C_GenerateSTOP(EEPROM_I2Cx,ENABLE);
	return 1;
}

//连续写入
uint8_t IIC_EEPROM_BufferWrite(uint8_t writeaddress,uint8_t *data,uint16_t num)
{
//	if(writeaddress+num>256)
//	{
//		printf("地址越界\n");
//	}
	uint8_t page_num,single_byte;
		while((writeaddress%I2C_PageSize != 0)&&num)//单字节写入，直到地址增长为8的整数或写完
		{
			I2C_EE_WaitEepromStandbyState();
			EEPROM_I2C_ByteWrite(writeaddress,data);
			writeaddress++;
			data++;
			num--;
		}
		page_num = num/I2C_PageSize;          //判断还可以写几页
		single_byte = num%I2C_PageSize;       //判断有几个字节需要单独写入
		while(page_num--)          //进行连续页写入
		{
			I2C_EE_WaitEepromStandbyState();
			EEPROM_I2C_Page_Write(writeaddress,data,I2C_PageSize);
			writeaddress += I2C_PageSize;
			data += I2C_PageSize;
		}
		while(single_byte--)      //将剩余的数据逐个写入
		{
			I2C_EE_WaitEepromStandbyState();
			EEPROM_I2C_ByteWrite(writeaddress,data);
			writeaddress++;
			data++;
		}
		return 1;
	
}

