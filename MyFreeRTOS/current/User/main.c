/*
*****************************************************
*                包含的头文件
*****************************************************
*/
#include "FreeRTOS.h"
#include "list.h"
/*
*****************************************************
*                  全局变量
*****************************************************
*/
/*定义任务控制块*/
TCB_t Task1TCB;
TCB_t Task2TCB;


portBASE_TYPE flag1 = 0;
portBASE_TYPE flag2 = 0;

/*软件延时*/
void delay(uint32_t count)
{
	for(;count!=0;count--);
}
/*任务1*/
void Task1_Entry(void *p_arg)
{
	for(;;)
	{
		flag1 = 1;
		delay(100);
		flag1 = 0;
		delay(100);
	}
}
/*任务2*/
void Task2_Entry(void *p_arg)
{
	for(;;)
	{
		flag2 = 1;
		delay(100);
		flag2 = 0;
		delay(100);
	}
}
/*
******************************************************
*										main函数
******************************************************
*/
int main(void)
{

}



