#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_general_Tim.h"
#define N 200
extern TIM_OCInitTypeDef	 TIM_OCInitStructure;

void delay(uint32_t n)
{
	while(n--);
}

int main(void)
{
	uint16_t i=0,j=0,k=0;
  TIM_Init(); 
	while(1)
	{		
		for(i=0;i<N;i++)
		{
			TIM_OCInitStructure.TIM_Pulse = i;
			TIM_OC3Init(TIMx,&TIM_OCInitStructure);//B0 G
			for(j=0;j<N;j++)
			{
				TIM_OCInitStructure.TIM_Pulse = j;
				TIM_OC4Init(TIMx,&TIM_OCInitStructure);//B1 B
				for(k=0;k<N;k++)
				{
					TIM_OCInitStructure.TIM_Pulse = k;
					TIM_OC2Init(TIMx,&TIM_OCInitStructure);//B5 R
					delay(0xFF);
				}
			}
		}
	}

}
