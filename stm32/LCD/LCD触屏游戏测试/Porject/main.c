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
	printf("XPT控制触屏实验\r\n");
	ILI9341_GramScan ( 3 );
	//从FLASH里获取校正参数，若FLASH无参数，则使用模式3进行校正
	Calibrate_or_Get_TouchParaWithFlash(3,0);
	Screan_Init(LCD_SCAN_MODE);
	Game_Space();
	while(1)
	{
		XPT2046_TouchEvenHandler();  
	}

}
