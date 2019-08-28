#ifndef _BSP_SPI_H_
#define _BSP_SPI_H_

#include "stm32f10x.h"
#include <stdio.h>

#define  FLASH_SPI_GPIO_APBxClkCmd     RCC_APB2PeriphClockCmd
#define  FLASH_SPI_GPIO_CLK            RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC
#define  FLASH_SPI_MOSI_GPIO_PORT      GPIOA  
#define  FLASH_SPI_MOSI_GPIO_PIN       GPIO_Pin_7
#define  FLASH_SPI_MISO_GPIO_PORT      GPIOA
#define  FLASH_SPI_MISO_GPIO_PIN       GPIO_Pin_6
#define  FLASH_SPI_SCK_GPIO_PORT       GPIOA
#define  FLASH_SPI_SCK_GPIO_PIN        GPIO_Pin_5
#define  FLASH_SPI_APBxClkCmd          RCC_APB2PeriphClockCmd
#define  FLASH_SPI_CLK                 RCC_APB2Periph_SPI1
#define  FLASH_SPIx                    SPI1

#define  FLASH_SPI_CS_PORT             GPIOC
#define  FLASH_SPI_CS_PIN              GPIO_Pin_0

//FLASH命令
#define Dummy_Byte                     0x00
#define Write_Enable                   0x06
#define Read_Data                      0x03
#define Sector_Erase                   0x20
#define Read_Status_Register1          0x05
#define Page_Program                   0x02

//选择flash
#define  FLASH_SPI_CS_HIGH             GPIO_SetBits(FLASH_SPI_CS_PORT,FLASH_SPI_CS_PIN);
#define  FLASH_SPI_CS_LOW              GPIO_ResetBits(FLASH_SPI_CS_PORT,FLASH_SPI_CS_PIN);
/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT              ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT              ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

/*信息输出*/
#define FLASH_DEBUG_ON         1
#define FLASH_INFO(fmt,arg...)           printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)          do{\
                                          if(FLASH_DEBUG_ON)\
                                          printf("<<-FLASH-DEBUG->> [%s][%d]"fmt"\n",__FILE__,__LINE__, ##arg);\
                                          }while(0)

void FLSAH_SPI_Config(void); 
uint8_t SPI_Send_Byte(u8 data); 
uint8_t SPI_Read_Byte(void);
u32 GET_FLASH_DEVICE_ID(void);
u32 GET_FLASH_JEDEC_ID(void);
void Flash_WaitForWrite(void);
void Flash_Sector_Erase(uint32_t address);
void Flash_Write_Enable(void);
void Flash_Write_Data(uint32_t address,uint8_t *databuffer,uint16_t numtowrite);
void Flash_Read_Data(uint32_t address,uint8_t *databuffer,uint32_t numtoread);
void Flash_Buffer_Write(uint32_t address,uint8_t *databuffer,uint32_t numtowrite);
																					
#endif  // _BSP_SPI_H_
