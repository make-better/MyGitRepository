#ifndef _BSP_DMA_MTM_H_
#define _BSP_DMA_MTM_H_
#include "stm32f10x.h"

#define BUFFER_SIZE    	 		32
#define RCC_AHBPeriph_DMA   RCC_AHBPeriph_DMA1
#define DMA_Channel         DMA1_Channel2
#define DMA_FLAG_TC			    DMA1_FLAG_TC2

void DMA_Configer(void);	
uint8_t Buffercmp(const uint16_t* pBuffer, 
                  uint16_t* pBuffer1, uint16_t BufferLength);																	




#endif // _BSP_DMA_MTM_H_


