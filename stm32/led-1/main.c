#if 0


#endif


#include"stm32f10x.h"
int main(void)
{
	//打开GPIOB的端口时钟
	*(unsigned int *)0x40021018 |=(1<<3);
	//配置IO为输出
	*(unsigned int *)0x40010c00 |=(1<<4*0);
	//控制ODR寄存器
	*(unsigned int *)0x40010c0c &=~(1<<0);
}
void SystemInit(void)
{
}