/*
*****************************************************
*                包含的头文件
*****************************************************
*/
#include "FreeRTOS.h"
#include "list.h"
#include "task.h"
/*
*****************************************************
*                  全局变量
*****************************************************
*/
/*任务就绪表*/
extern List_t pxReadyTasksLists[ configMAX_PRIORITIES ];

portBASE_TYPE flag1 = 0;
portBASE_TYPE flag2 = 0;
/*
*****************************************************
*                  任务控制块 & STACK
*****************************************************
*/
/*定义任务栈*/
#define TASK1_STACK_SIZE       128
StackType_t Task1Stack[TASK1_STACK_SIZE];

#define TASK2_STACK_SIZE       128
StackType_t Task2Stack[TASK2_STACK_SIZE];

/*定义空闲任务的栈*/
#define configMINIMAL_STACK_SIZE             ((unsigned short)128)
StackType_t IdleTaskStack[configMINIMAL_STACK_SIZE];

/*定义任务控制块*/
TCB_t Task1TCB;
TCB_t Task2TCB;

/*定义空闲任务的任务控制块*/
TCB_t IdleTaskTCB;


/*任务句柄*/
TaskHandle_t Task1_Handle;
TaskHandle_t Task2_Handle;
TaskHandle_t xIdleTaskHandle;
/*为空闲任务分配内存*/
void vApplicationGetIdleTaskMemory(TCB_t **ppxIdleTaskTCBBuffer,
	                                 StackType_t **ppxIdleTaskStackBuffer,
	                                 uint32_t *pulIdleTaskStackSize  )
{
	*ppxIdleTaskTCBBuffer = &IdleTaskTCB;
	*ppxIdleTaskStackBuffer = IdleTaskStack;
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

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
#if 0
		flag1 = 1;
		delay(100);
		flag1 = 0;
		delay(100);
		/*任务切换，手动*/
		taskYIELD();
#else
    flag1 = 1;
		vTaskDelay(2);
		flag1 = 0;
		vTaskDelay(2);
#endif		
	}

}
/*任务2*/
void Task2_Entry(void *p_arg)
{
	for(;;)
	{
#if 0
		flag2 = 1;
		delay(100);
		flag2 = 0;
		delay(100);
		/*任务切换，手动*/
		taskYIELD();
#else
		flag2 = 1;
		vTaskDelay(2);
		flag2 = 0;
		vTaskDelay(2);
#endif
	}
}
/*
******************************************************
*										main函数
******************************************************
*/
int main(void)
{
	/*初始化任务相关的列表，如就序列表*/
	prvInitialiseTaskLists();
	
	Task1_Handle =                                                  /*任务句柄*/
	xTaskCreateStatic((TaskFunction_t)Task1_Entry,                  /*任务入口*/
	                                  (char *)"Task1",              /*任务名称，字符串形式*/
																		(uint32_t)TASK1_STACK_SIZE,  /*任务栈大小，单位为字*/
																		(void *)NULL,                 /*任务形参*/
																		(StackType_t *)Task1Stack,    /*任务栈起始地址*/
	                                  (TCB_t *)&Task1TCB);          /*任务控制块*/
	/*将任务添加到就序列表*/
	vListInsertEnd(&(pxReadyTasksLists[1]),
								 &(((TCB_t *)(&Task1TCB))->xStateListItem));
	Task2_Handle =                                                  /*任务句柄*/
	xTaskCreateStatic((TaskFunction_t)Task2_Entry,                  /*任务入口*/
	                                  (char *)"Task2",              /*任务名称，字符串形式*/
																		(uint32_t)TASK2_STACK_SIZE,  /*任务栈大小，单位为字*/
																		(void *)NULL,                 /*任务形参*/
																		(StackType_t *)Task2Stack,    /*任务栈起始地址*/
	                                  (TCB_t *)&Task2TCB);          /*任务控制块*/
 /*将任务添加到就序列表*/
 vListInsertEnd(&(pxReadyTasksLists[2]),
								 &(((TCB_t *)(&Task2TCB))->xStateListItem));
 /*启动调度器，开始多任务调度，启动成功则不返回*/
 vTaskStartScheduler();

 for(;;)
 {
	 //
 }
}


