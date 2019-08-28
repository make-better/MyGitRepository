#include "bsp_adc.h"

static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	ADCx_GPIO_RCC_FUN(ADCx_GPIO_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = ADCx_GPIO_PIN;
	GPIO_Init(ADCx_GPIO_PORT,&GPIO_InitStructure);
}

static void ADCx_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = ADCx_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

static void ADCx_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	RCC_APB2PeriphClockCmd(ADCx_RCC,ENABLE);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC_x,&ADC_InitStructure);
	//设置为8分频
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	//开中断
	ADC_ITConfig(ADC_x,ADC_IT_EOC,ENABLE);
	//配置顺序
	ADC_RegularChannelConfig(ADC_x,ADCx_Channel,1,ADC_SampleTime_55Cycles5);
	ADC_Cmd(ADC_x, ENABLE);
	// 初始化 ADC 校准寄存器
	ADC_ResetCalibration(ADC_x);
	// 等待校准寄存器初始化完成
	while (ADC_GetResetCalibrationStatus(ADC_x));

	// ADC 开始校准
	ADC_StartCalibration(ADC_x);
	// 等待校准完成
	while (ADC_GetCalibrationStatus(ADC_x));

	// 由于没有采用外部触发，所以使用软件触发 ADC 转换
	ADC_SoftwareStartConvCmd(ADC_x, ENABLE);
}

void ADCx_TEST_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_NVIC_Config();
	ADCx_Mode_Config();
}
