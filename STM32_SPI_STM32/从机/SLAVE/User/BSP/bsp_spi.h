#ifndef _BSP_SPI_H_
#define _BSP_SPI_H_

#include "stm32f10x.h"
#include <stdio.h>

#define  SPI_GPIO_APBxClkCmd     RCC_APB2PeriphClockCmd
#define  SPI_GPIO_CLK            RCC_APB2Periph_GPIOA

#define  SPI_MOSI_GPIO_PORT      GPIOA
#define  SPI_MOSI_GPIO_PIN       GPIO_Pin_7

#define  SPI_MISO_GPIO_PORT      GPIOA
#define  SPI_MISO_GPIO_PIN       GPIO_Pin_6

#define  SPI_SCK_GPIO_PORT       GPIOA
#define  SPI_SCK_GPIO_PIN        GPIO_Pin_5

#define  SPI_APBxClkCmd          RCC_APB2PeriphClockCmd
#define  SPI_CLK                 RCC_APB2Periph_SPI1
#define  SPIX                    SPI1

#define  SPI_CS_PORT             GPIOA
#define  SPI_CS_PIN              GPIO_Pin_4

#define  SPIX_IRQ								 SPI1_IRQn
#define  SPIX_IRQHandler         SPI1_IRQHandler


//选择flash
#define  SPI_CS_HIGH             GPIO_SetBits(SPI_CS_PORT,SPI_CS_PIN);
#define  SPI_CS_LOW              GPIO_ResetBits(SPI_CS_PORT,SPI_CS_PIN);
/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT              ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT              ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

/*信息输出*/
#define DEBUG_ON         1
#define INFO(fmt,arg...)           printf("<<-INFO->> "fmt"\n",##arg)
#define ERROR(fmt,arg...)          printf("<<-ERROR->> "fmt"\n",##arg)
#define DEBUG(fmt,arg...)          do{\
                                          if(DEBUG_ON)\
                                          printf("<<-DEBUG->> [%s][%d]"fmt"\n",__FILE__,__LINE__, ##arg);\
                                          }while(0)

void SPI_M_M_Init(void); 
uint8_t SPI_Send_Byte(u16 data); 
uint8_t SPI_Read_Byte(void);
																					
#endif  // _BSP_SPI_H_
