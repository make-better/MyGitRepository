#include "bsp_usart.h"
#include "bsp_ili_lcd.h"
#include "fonts.h"

void delay(uint32_t x)
{
	uint8_t i=255,j=250,k=200;
	while(x--)
	for(;i>0;i--)
	for(;j>0;j--)
	for(;k>0;k--);
}

static uint8_t i=0;
char buffer[50];
int main(void)
{
	ILI9341_Init();
	USART_Config();
	printf("LCD“∫æß320*240œ‘ æ∆˜ µ—È\r\n");
	let_us_try();
	while(1)
	{
	sprintf(buffer,"≤‚ ‘£∫%d £°",i);
	i++;
	delay(0xffff);
	ILI9341_DispStringLine_EN_CH(LINE(9),buffer);
	}

}
