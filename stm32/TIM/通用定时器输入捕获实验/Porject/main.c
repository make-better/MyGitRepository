#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_general_Tim.h"
#include "bsp_usart.h"
extern TIM_ICUserValueType TIM_ICUserValueStructure;
void delay(uint32_t n)
{
	while(n--);
}
int main(void)
{
	double time1;

// TIM ������������ʱ��
	uint32_t TIM_PscCLK = 72000000 / (71+1);
	USART_Config();
	LED_Configer();
  TIM_Init(); 
	printf("\r\n���벶��ʵ��\r\n");
	while(1)
	{	
		if (TIM_ICUserValueStructure.Capture_FinishFlag == 1) 
		{
		 // ����ߵ�ƽʱ��ļ�������ֵ
		 time1 = TIM_ICUserValueStructure.Capture_Period * (0XFFFF+1) +
		 (TIM_ICUserValueStructure.Capture_CcrValue+1);
		 // ��ӡ�ߵ�ƽ����ʱ��
			printf ( "\r\n ��øߵ�ƽ����ʱ��time2: %f s\r\n",time1/TIM_PscCLK);
		 TIM_ICUserValueStructure.Capture_FinishFlag = 0;
		}
	}
}

