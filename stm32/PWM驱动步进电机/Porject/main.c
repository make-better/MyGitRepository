#include "bsp_ili_lcd.h"
#include "fonts.h"
#include "bsp_xpt2046.h"
#include "bsp_led.h"
#include "game.h"
#include "bsp_advance_tim.h"
#include <stdio.h>
#include "bsp_gpio.h"
#include "bsp_basic_tim.h"
extern uint8_t Display;
extern uint8_t unit;
extern uint8_t back;
uint16_t time = 0;
int main(void)
{
	LED_Configer();
	SDC_GPIO_Init();
	ILI9341_Init();
	XPT2046_Init();
	ILI9341_GramScan ( 6 );
	//从FLASH里获取校正参数，若FLASH无参数，则使用模式3进行校正
	Calibrate_or_Get_TouchParaWithFlash(3,0);
	Screan_Init(LCD_SCAN_MODE);
	LCD_SetColors(CL_WHITE,CL_BLACK);
	LCD_SetFont(&Font8x16);
	Display_Space();
	while(1)
	{
		XPT2046_TouchEvenHandler();  	
		if(Display==1)//中断后进入下一个显示界面
		{
			Display_Space();
			if(back>0)		//5和6的情况下需要用到
			{
				Basic_Tim_Init();//开启基本定时器,等待小段时间后使电机转回初始角度
			}	
			Display=0;
		}
		if(time == 500)//等待小段时间后使电机转回需要的角度
		{
			time=0;
			switch(back)
			{
				case 1:Moto_Contral(0,5);break;
				case 2:Moto_Contral(0,11);break;
				default:break;
			}
			TIM_Cmd(BASIC_TIM,DISABLE);
			TIM_ITConfig(BASIC_TIM,TIM_IT_Update,DISABLE);
			back=0;
		}
	}
}
