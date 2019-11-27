#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_exti.h"
#include "bsp_basic_tim.h"
#include "bsp_led.h"
#include "bsp_systick.h"
#include "bsp_output_Tim.h"

uint16_t frequence[8] = {0};

int main(void)
{
	USART_Config();	//1-1
	Fre_EXTI_Init();			//1-2
	//TIM_OutPut_Init();
	Tim_Init();
	LED_Configer();
	while(1)
	{
		SysTick_Delay_ms(500);
		for(uint8_t i = 0;i<8;i++)
		{
			printf("fre [%d] : %d\n",i,frequence[i]);
		}
		printf("-------------------------------------\n");
	}

}

