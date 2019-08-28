#include "bsp_usart.h"
#include "bsp_ili_lcd.h"
#include "fonts.h"
#include "bsp_xpt2046.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "game.h"
#include "ff.h"
#include "./flash/bsp_spi_flash.h"
#include "./app/app.h"


FATFS fs;													/* FatFs�ļ�ϵͳ���� */
FIL fnew;													/* �ļ����� */
FRESULT res_flash;                /* �ļ�������� */
UINT fnum;            					  /* �ļ��ɹ���д���� */
BYTE ReadBuffer[100]={0};        /* �������� */

int main(void)
{
	LED_Configer();
	ILI9341_Init();
	XPT2046_Init();
	USART_Config();
	ILI9341_GramScan ( 3 );
	//��FLASH���ȡУ����������FLASH�޲�������ʹ��ģʽ3����У��
	Calibrate_or_Get_TouchParaWithFlash(3,0);
	Screan_Init(LCD_SCAN_MODE);
	//��鲢�����ļ�ϵͳ
	Fatfs_exist_test();
	Game_read_from_flash();
	Game_Space();
	while(1)
	{
		XPT2046_TouchEvenHandler();  
	}

}
