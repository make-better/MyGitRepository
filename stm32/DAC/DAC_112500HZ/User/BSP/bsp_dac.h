#ifndef _BSP_DAC_H_
#define _BSP_DAC_H_
#include "stm32f10x.h"

#define DAC_DHR12RD_ADDRESS 	(DAC_BASE+0x20)

//设置数组大小
#define POINT_NUM    	 		32
//设置DMA
#define RCC_AHBPeriph_DMA   RCC_AHBPeriph_DMA2
#define DMA_Channel         DMA2_Channel4

void DAC_Mode_Init(void);


#endif //_BSP_DAC_H_
