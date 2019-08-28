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


FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_flash;                /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */
BYTE ReadBuffer[100]={0};        /* 读缓冲区 */

int main(void)
{
	LED_Configer();
	ILI9341_Init();
	XPT2046_Init();
	USART_Config();
	ILI9341_GramScan ( 3 );
	//从FLASH里获取校正参数，若FLASH无参数，则使用模式3进行校正
	Calibrate_or_Get_TouchParaWithFlash(3,0);
	Screan_Init(LCD_SCAN_MODE);
	//检查并挂载文件系统
	Fatfs_exist_test();
	Game_read_from_flash();
	Game_Space();
	while(1)
	{
		XPT2046_TouchEvenHandler();  
	}

}
