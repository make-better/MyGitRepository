#include "bsp_usart.h"
#include "bsp_led.h"
extern uint16_t temp;

int main(void)
{
	USART_Config();
	LED_Configer();
	printf("串口控制LED程序\n");
	while(1)
	{
		while(temp!='!')
		{
			printf("收到数据：%c\n",temp);
			switch(temp)
			{
				case 'R':LED_R(ON);LED_G(OFF);LED_B(OFF);break;
				case 'G':LED_G(ON);LED_R(OFF);LED_B(OFF);break;
				case 'B':LED_B(ON);LED_R(OFF);LED_G(OFF);break;
				default:LED_R(OFF);LED_G(OFF);LED_B(OFF);break;
			}		
			temp='!';
		}
	}

}
