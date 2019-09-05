#include "bsp_usart.h"
#include "bsp_adc.h"

extern uint16_t ADC_Temp_Value[NumOfAdc];
float ADC_Value[NumOfAdc]={0,0,0,0,0};


void delay(uint32_t n)
{
	while(n--);
}
	

int main(void)
{
	uint8_t i=0;
	USART_Config();
	ADCx_TEST_Init();
	printf("ADC��ͨ���ɼ�ʵ��\n");
	while(1)
	{
		for(i=0;i<5;i++)
		{
			ADC_Value[i] = (float)ADC_Temp_Value[i]/4096*3.3;
		//printf("ԭʼ����%d��%x\n",i,ADC_Temp_Value[i]);
		printf("������ѹ%d��%f\n",i+1,ADC_Value[i]);
		}
		printf("\n");
		delay(0xffffee);
	}

}
