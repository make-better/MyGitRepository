#if 0


#endif


#include"stm32f10x.h"
int main(void)
{
	//��GPIOB�Ķ˿�ʱ��
	*(unsigned int *)0x40021018 |=(1<<3);
	//����IOΪ���
	*(unsigned int *)0x40010c00 |=(1<<4*0);
	//����ODR�Ĵ���
	*(unsigned int *)0x40010c0c &=~(1<<0);
}
void SystemInit(void)
{
}