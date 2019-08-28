#include "bsp_usart.h"
#include "bsp_ili_lcd.h"
#include "fonts.h"
#include "bsp_xpt2046.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "game.h"


int main(void)
{
	LED_Configer();
	ILI9341_Init();
	XPT2046_Init();
	USART_Config();
	printf("XPT���ƴ���ʵ��\r\n");
	ILI9341_GramScan ( 3 );
	//��FLASH���ȡУ����������FLASH�޲�������ʹ��ģʽ3����У��
	Calibrate_or_Get_TouchParaWithFlash(3,0);
	Screan_Init(LCD_SCAN_MODE);
	Game_Space();
	while(1)
	{
		XPT2046_TouchEvenHandler();  
	}

}
