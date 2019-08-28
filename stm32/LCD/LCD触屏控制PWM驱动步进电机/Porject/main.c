#include "bsp_ili_lcd.h"
#include "fonts.h"
#include "bsp_xpt2046.h"
#include "bsp_led.h"
#include "game.h"
#include "bsp_advance_tim.h"
#include <stdio.h>
#include "bsp_gpio.h"
int main(void)
{
	char string[20];
	LED_Configer();
	SDC_GPIO_Init();
	ILI9341_Init();
	XPT2046_Init();
	Advance_TIM_Init();
	ILI9341_GramScan ( 6 );
	//从FLASH里获取校正参数，若FLASH无参数，则使用模式3进行校正
	Calibrate_or_Get_TouchParaWithFlash(3,0);
	Screan_Init(LCD_SCAN_MODE);
	sprintf(string,"PWM:关 方向:正 脱机:关 ");
	LCD_SetColors(CL_WHITE,CL_BLACK);
	LCD_SetFont(&Font8x16);
  ILI9341_DispString_EN_CH(40,LINE(0),string);
	while(1)
	{
		XPT2046_TouchEvenHandler();  
	}
}
