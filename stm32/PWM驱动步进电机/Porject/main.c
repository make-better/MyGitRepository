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
	//��FLASH���ȡУ����������FLASH�޲�������ʹ��ģʽ3����У��
	Calibrate_or_Get_TouchParaWithFlash(3,0);
	Screan_Init(LCD_SCAN_MODE);
	LCD_SetColors(CL_WHITE,CL_BLACK);
	LCD_SetFont(&Font8x16);
	Display_Space();
	while(1)
	{
		XPT2046_TouchEvenHandler();  	
		if(Display==1)//�жϺ������һ����ʾ����
		{
			Display_Space();
			if(back>0)		//5��6���������Ҫ�õ�
			{
				Basic_Tim_Init();//����������ʱ��,�ȴ�С��ʱ���ʹ���ת�س�ʼ�Ƕ�
			}	
			Display=0;
		}
		if(time == 500)//�ȴ�С��ʱ���ʹ���ת����Ҫ�ĽǶ�
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
