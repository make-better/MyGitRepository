#include "bsp_usart.h"
#include "bsp_ili_lcd.h"
#include "fonts.h"
#include "bsp_xpt2046.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "game.h"
#include "bsp_advance_tim.h"

uint16_t frequency = 8000;
uint16_t duty_cycle = 5;

int main(void)
{
	char string[30];
	LED_Configer();
	ILI9341_Init();
	XPT2046_Init();
	Advance_TIM_Init();
	USART_Config();
	printf("XPT���ƴ���ʵ��\r\n");
	ILI9341_GramScan ( 3 );
	//��FLASH���ȡУ����������FLASH�޲�������ʹ��ģʽ3����У��
	Calibrate_or_Get_TouchParaWithFlash(3,0);
	Screan_Init(LCD_SCAN_MODE);
	sprintf(string,"Ƶ��:%d  Hz ռ�ձ�:%d %%",8000000/frequency,duty_cycle);
	LCD_SetColors(CL_RED,CL_WHITE);
	ILI9341_DispString_EN_CH(10*8,
																LINE(0),
																string);
	while(1)
	{
		XPT2046_TouchEvenHandler();  
	}

}
