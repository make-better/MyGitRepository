#include "bsp_dac.h"
/* 波形数据 -----------------------------------------------*/
const uint16_t Sine12bit[POINT_NUM] = {
	 2048 , 2460 , 2856 , 3218 , 3532 , 3786 , 3969 , 4072 ,
	 4093 , 4031 , 3887 , 3668 , 3382 , 3042 , 2661 , 2255 ,
	 1841 , 1435 , 1054 , 714 , 428 , 209 , 65 , 3 ,
	 24 , 127 , 310 , 564 , 878 , 1240 , 1636 , 2048
};


uint32_t DualSine12bit[POINT_NUM];

//配置DAC
static void DAC_Config(void)
{
	DAC_InitTypeDef DAC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	
	DAC_Init(DAC_Channel_1,&DAC_InitStructure);
	DAC_Init(DAC_Channel_2,&DAC_InitStructure);
	
	DAC_Cmd(DAC_Channel_1,ENABLE);
	DAC_Cmd(DAC_Channel_2,ENABLE);
	DAC_DMACmd(DAC_Channel_2, ENABLE);
}

static void DAC_TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 20-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
	TIM_Cmd(TIM2,ENABLE);
}


//dma配置函数
static void DAC_DMA_Configer(void)
{
	DMA_InitTypeDef DMA_InitStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA,ENABLE);
	
	DMA_InitStruct.DMA_PeripheralBaseAddr = DAC_DHR12RD_ADDRESS;
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)DualSine12bit;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStruct.DMA_BufferSize = POINT_NUM;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	
	DMA_Init(DMA_Channel, &DMA_InitStruct);
	DMA_Cmd(DMA_Channel, ENABLE);
	
}
void DAC_Mode_Init(void)
{
	uint8_t i=0;
	DAC_Config();
	DAC_TIM_Config();
	for(i=0;i<POINT_NUM;i++)
	{
		DualSine12bit[i] = (Sine12bit[i]<<16)+Sine12bit[i];
	}
	DAC_DMA_Configer();
}

