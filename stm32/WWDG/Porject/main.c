#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_exti.h"
#include "bsp_wwdg.h"



void delay(unsigned int n)
{
	while(n--);
}


int main(void)
{
	uint8_t wwwdg_wr,wwdg_tr;
	LED_Configer();
	LED_B(ON);
	WWDG_Config(0x5f,0x7f,WWDG_Prescaler_8);
	wwwdg_wr = WWDG->CFR & 0X7F;
	while(1)
	{
		wwdg_tr = WWDG->CR & 0X7F;
		if(wwdg_tr<wwwdg_wr)
		{
			//LED_R(OFF);
			WWDG_Feed();
		}
	}

}

