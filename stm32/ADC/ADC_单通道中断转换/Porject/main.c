#include "bsp_usart.h"
#include "bsp_adc.h"

extern uint16_t ADC_Temp_Value;
float ADC_Value=0;


void delay(uint32_t n)
{
	while(n--);
}
	

int main(void)
{
	USART_Config();
	ADCx_TEST_Init();
	printf("ADC��ͨ���ɼ�ʵ��\n");
	while(1)
	{
		ADC_Value = (float)ADC_Temp_Value/4096*3.3;
		printf("���ԭʼ���ݣ�%x\n",ADC_Temp_Value);
		printf("������ݣ�%f\n",ADC_Value);
		delay(0xffffe);
	}

}
