#include "bsp_dma_mtm.h"


const uint16_t aSRC_Const_Buffer[BUFFER_SIZE]= {
                                    0x0304,0x0508,0x090C,0x0F10,
                                    0x1114,0x1518,0x191C,0x1F20,
                                    0x2122,0x2728,0x2B2C,0x2D30,
                                    0x3334,0x3738,0x393C,0x3F40,
                                    0x4142,0x4548,0x494C,0x4D50,
                                    0x5154,0x5758,0x5B5C,0x5D60,
                                    0x6164,0x6768,0x696C,0x6F70,
                                    0x7174,0x7778,0x797C,0x7F80};
uint16_t aDST_Buffer[BUFFER_SIZE];
																		
void DMA_Configer(void)
{
	DMA_InitTypeDef DMA_InitStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA,ENABLE);
	
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)aSRC_Const_Buffer;
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)aDST_Buffer;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStruct.DMA_BufferSize = BUFFER_SIZE;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;
	
	DMA_Init(DMA_Channel, &DMA_InitStruct);
	DMA_ClearFlag(DMA_FLAG_TC);
	DMA_Cmd(DMA_Channel, ENABLE);
	
}

uint8_t Buffercmp(const uint16_t* pBuffer, 
                  uint16_t* pBuffer1, uint16_t BufferLength)
{
  /* 数据长度递减 */
  while(BufferLength--)
  {
    /* 判断两个数据源是否对应相等 */
    if(*pBuffer != *pBuffer1)
    {
      /* 对应数据源不相等马上退出函数，并返回0 */
      return 0;
    }
    /* 递增两个数据源的地址指针 */
    pBuffer++;
    pBuffer1++;
  }
  /* 完成判断并且对应数据相对 */
  return 1;  
}




