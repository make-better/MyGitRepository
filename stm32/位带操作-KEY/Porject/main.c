#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_key.h"

#define GPIO_IDR_addr1                (GPIOA_BASE + 8)
#define GPIO_IDR_addr2                (GPIOC_BASE + 8)
#define GPIO_ODR_addr                (GPIOB_BASE + 12)
// 把“位带地址+位序号”转换成别名地址的宏
#define BITREAD(GPIO_IDR_addr, n)    *(unsigned int *)((GPIO_IDR_addr & 0xF0000000)+0x02000000+((GPIO_IDR_addr & 0x00FFFFFF)<<5)+(n<<2))
#define BITWRITE(GPIO_IDR_addr, n)   *(unsigned int *)((GPIO_ODR_addr & 0xF0000000)+0x02000000+((GPIO_ODR_addr & 0x00FFFFFF)<<5)+(n<<2))	

void delay(unsigned int n)
{
	while(n--);
}


int main(void)
{
	LED_Configer();
	Key_Configer();
	while(1)
	{
//		if(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN))
//			LED_G_TOGGLE;
//		if(Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN))
//			LED_R_TOGGLE;
		if(BITREAD(GPIO_IDR_addr1, 0) == ON)
		{
			while(BITREAD(GPIO_IDR_addr1, 0) == ON);
			BITWRITE(GPIO_IDR_addr, 1) ^= 1;  
		}
		
		if(BITREAD(GPIO_IDR_addr2, 13) == ON)
		{
			while(BITREAD(GPIO_IDR_addr2, 13) == ON);
			BITWRITE(GPIO_IDR_addr, 5) ^= 1;  
		}
	}

}
