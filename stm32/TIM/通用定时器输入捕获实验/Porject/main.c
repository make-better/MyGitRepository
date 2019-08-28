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

// TIM 计数器的驱动时钟
	uint32_t TIM_PscCLK = 72000000 / (71+1);
	USART_Config();
	LED_Configer();
  TIM_Init(); 
	printf("\r\n输入捕获实验\r\n");
	while(1)
	{	
		if (TIM_ICUserValueStructure.Capture_FinishFlag == 1) 
		{
		 // 计算高电平时间的计数器的值
		 time1 = TIM_ICUserValueStructure.Capture_Period * (0XFFFF+1) +
		 (TIM_ICUserValueStructure.Capture_CcrValue+1);
		 // 打印高电平脉宽时间
			printf ( "\r\n 测得高电平脉宽时间time2: %f s\r\n",time1/TIM_PscCLK);
		 TIM_ICUserValueStructure.Capture_FinishFlag = 0;
		}
	}
}

