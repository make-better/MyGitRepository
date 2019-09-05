#include "bsp_adc.h"
#include "stm32f10x_dma.h"

uint32_t ADC_Temp_Value[NumOfAdc];

static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	ADCx_GPIO_RCC_FUN(ADCx_GPIO_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = ADC1_GPIO_PIN|ADC2_GPIO_PIN;
	GPIO_Init(ADCx_GPIO_PORT,&GPIO_InitStructure);
}
static void ADCx_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStruct;
	
	RCC_APB2PeriphClockCmd(ADCx_RCC,ENABLE);
	/*---------------------------------*/
	RCC_AHBPeriph_DMA_FUN(RCC_AHBPeriph_DMA,ENABLE);
	
	DMA_DeInit(DMA_Channel);
	
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)(&( ADCx_1->DR ));
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)ADC_Temp_Value;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStruct.DMA_BufferSize = NumOfAdc;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	
	DMA_Init(DMA_Channel, &DMA_InitStruct);
	DMA_Cmd(DMA_Channel, ENABLE);
	/*----------------ADC1-----------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_FastInterl;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_NbrOfChannel = NumOfAdc;
	ADC_Init(ADCx_1,&ADC_InitStructure);
	//����Ϊ8��Ƶ
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
	ADC_RegularChannelConfig(ADCx_1,ADCx_Channel1,1,ADC_SampleTime_55Cycles5);
	//ADC_RegularChannelConfig(ADCx_1,ADCx_Channel2,2,ADC_SampleTime_55Cycles5);
	// ʹ�� ADC1 DMA ����
  ADC_DMACmd(ADCx_1, ENABLE);
	
	/*--------------------ADC1У׼------------------------*/
	ADC_Cmd(ADCx_1, ENABLE);
	// ��ʼ�� ADC У׼�Ĵ���
	ADC_ResetCalibration(ADCx_1);
	// �ȴ�У׼�Ĵ�����ʼ�����
	while (ADC_GetResetCalibrationStatus(ADCx_1));
	// ADC ��ʼУ׼
	ADC_StartCalibration(ADCx_1);
	// �ȴ�У׼���
	while (ADC_GetCalibrationStatus(ADCx_1));
	/*---------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_FastInterl;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_NbrOfChannel = NumOfAdc;
	ADC_Init(ADCx_2,&ADC_InitStructure);
	
	//����Ϊ8��Ƶ
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	//����˳��
	ADC_RegularChannelConfig(ADCx_2,ADCx_Channel1,1,ADC_SampleTime_55Cycles5);
	//ADC_RegularChannelConfig(ADCx_2,ADCx_Channel5,2,ADC_SampleTime_55Cycles5);
	/* ʹ�� ADCx_2 ���ⲿ����ת�� */
	ADC_ExternalTrigConvCmd(ADC2, ENABLE);
  /* ----------------ADCx_2 У׼--------------------- */

	
	ADC_Cmd(ADCx_2, ENABLE);
	// ��ʼ�� ADC У׼�Ĵ���
	ADC_ResetCalibration(ADCx_2);
	// �ȴ�У׼�Ĵ�����ʼ�����
	while (ADC_GetResetCalibrationStatus(ADCx_2));
	// ADC ��ʼУ׼
	ADC_StartCalibration(ADCx_2);
	// �ȴ�У׼���
	while (ADC_GetCalibrationStatus(ADCx_2));
	// ����û�в����ⲿ����������ʹ��������� ADC ת��
	ADC_SoftwareStartConvCmd(ADCx_1, ENABLE);	
}

void ADCx_TEST_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
}
