#ifndef _BSP_I2C_H_
#define _BSP_I2C_H_

#include "stm32f10x.h"
#include <stdio.h>

#define  EEPROM_I2C_GPIO_APBxClkCmd     RCC_APB2PeriphClockCmd
#define  EEPROM_I2C_GPIO_CLK            RCC_APB2Periph_GPIOB
#define  EEPROM_I2C_SCL_GPIO_PORT       GPIOB   
#define  EEPROM_I2C_SCL_GPIO_PIN        GPIO_Pin_6
#define  EEPROM_I2C_SDA_GPIO_PORT       GPIOB
#define  EEPROM_I2C_SDA_GPIO_PIN        GPIO_Pin_7
#define  EEPROM_I2C_APBxClkCmd          RCC_APB1PeriphClockCmd
#define  EEPROM_I2C_CLK                 RCC_APB1Periph_I2C1
#define  EEPROM_I2Cx                    I2C1
#define  EEPROM_Address                 0xa0
#define  EEPROM_I2C_READ_Address        0xa0
#define  EEPROM_I2C_WRITE_Address       0xa1

/* STM32 I2C 快速模式 */
#define I2C_Speed              400000  //*

/* 这个地址只要与STM32外挂的I2C器件地址不一样即可 */
#define I2Cx_OWN_ADDRESS7      0X0A   

/* AT24C01/02每页有8个字节 */
#define I2C_PageSize           8
/*等待超时时间*/
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))

/*信息输出*/
#define EEPROM_DEBUG_ON         1
#define EEPROM_INFO(fmt,arg...)           printf("<<-EEPROM-INFO->> "fmt"\n",##arg)
#define EEPROM_ERROR(fmt,arg...)          printf("<<-EEPROM-ERROR->> "fmt"\n",##arg)
#define EEPROM_DEBUG(fmt,arg...)          do{\
                                          if(EEPROM_DEBUG_ON)\
                                          printf("<<-EEPROM-DEBUG->> [%s][%d]"fmt"\n",__FILE__,__LINE__, ##arg);\
                                          }while(0)

void EEPROM_I2C_Config(void);
uint8_t EEPROM_I2C_ByteWrite(uint8_t writeaddress,uint8_t *data);
uint8_t I2C_EE_WaitEepromStandbyState(void);
uint8_t EEPROM_I2C_Read(uint8_t readaddress,uint8_t *data,uint16_t num);
void EEPROM_IIC_EASY_TEST(void);
uint8_t EEPROM_I2C_Page_Write(uint8_t writeaddress,uint8_t *data,uint8_t num);
uint8_t IIC_EEPROM_BufferWrite(uint8_t writeaddress,uint8_t *data,uint16_t num);


#endif  // _BSP_I2C_H_
