#include "bsp_usart.h"
#include "bsp_adc.h"

extern uint32_t ADC_Temp_Value[NumOfAdc];
float ADC_Value[NumOfAdc];


void delay(uint32_t n)
{
	while(n--);
}
	

int main(void)
{
	uint8_t i=0;
	USART_Config();
	ADCx_TEST_Init();
	printf("ADC单通道采集实验\n");
	while(1)
	{
		for(i=0;i<NumOfAdc;i++)
		{
			printf("ADC1_DR：%x\n",ADC_Temp_Value[i]);
			ADC_Value[i] = (float)(ADC_Temp_Value[i]&0xffff)/4096*3.3;
			printf("ADC1采样电压%d：%f\n",i+1,ADC_Value[i]);
			ADC_Value[i] = (float)((ADC_Temp_Value[i]&0xffff0000)>>16)/4096*3.3;
			printf("ADC2采样电压%d：%f\n",i+1,ADC_Value[i]);
		}
		printf("\n");
		delay(0xffffee);
	}

}
