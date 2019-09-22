#include "bsp_usart.h"
#include "bsp_spi.h"
#include "bsp_key.h"


int main(void)
{
	int i = 0;
	int n = 0;
	SPI_M_M_Init();
	USART_Config();
	Key_Configer();
	
	printf("开机!!!\n");
	
	while(1)
	{
		if(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN))
		{
			i++;
			if(i>3)
			{
				i = 0;
			}
			if(0 == SPI_Send_Data(i,i+n))
			{
				printf("通信正常 %d,%d\n",i,i+n);
			}else
			{
				printf("通信异常\n");
			}
			n++;
			
		}
		
		if(Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN))
		{
			printf("(||——)\n");
		}
	}

}
