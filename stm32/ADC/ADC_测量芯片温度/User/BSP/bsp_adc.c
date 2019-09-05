#include "bsp_adc.h"
#include "stm32f10x_dma.h"

uint16_t ADC_Temp_Value=0;

static void ADCx_GPIO_Config(void)
{	
	ADCx_GPIO_RCC_FUN(ADCx_GPIO_CLK,ENABLE);
}
static void ADCx_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStruct;
	
	RCC_APB2PeriphClockCmd(ADCx_RCC,ENABLE);
	/*---------------------------------*/
	RCC_AHBPeriph_DMA_FUN(RCC_AHBPeriph_DMA,ENABLE);
	
	DMA_DeInit(DMA_Channel);
	
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)(&( ADC_x->DR ));
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)&ADC_Temp_Value;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStruct.DMA_BufferSize = 1;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	
	DMA_Init(DMA_Channel, &DMA_InitStruct);
	DMA_Cmd(DMA_Channel, ENABLE);
	/*---------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC_x,&ADC_InitStructure);
	//����Ϊ8��Ƶ
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	//����˳��
	ADC_RegularChannelConfig(ADC_x,ADCx_Channel,1,ADC_SampleTime_55Cycles5);
	
	ADC_TempSensorVrefintCmd(ENABLE);
	
	// ʹ�� ADC DMA ����
  ADC_DMACmd(ADC_x, ENABLE);
	ADC_Cmd(ADC_x, ENABLE);
	// ��ʼ�� ADC У׼�Ĵ���
	ADC_ResetCalibration(ADC_x);
	// �ȴ�У׼�Ĵ�����ʼ�����
	while (ADC_GetResetCalibrationStatus(ADC_x));

	// ADC ��ʼУ׼
	ADC_StartCalibration(ADC_x);
	// �ȴ�У׼���
	while (ADC_GetCalibrationStatus(ADC_x));

	// ����û�в����ⲿ����������ʹ��������� ADC ת��
	ADC_SoftwareStartConvCmd(ADC_x, ENABLE);
}

void ADCx_TEST_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
}
