
/*
**********************************************************************************************
*																			头文件
**********************************************************************************************
*/
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_led.h"

/*
**********************************************************************************************
*																			任务句柄
**********************************************************************************************
*/
 /* 创建任务句柄 */
static TaskHandle_t AppTaskCreate_Handle;
/* LED任务句柄 */
static TaskHandle_t LED_Task_Handle;	

/*
**********************************************************************************************
*																			变量
**********************************************************************************************
*/
/*空闲任务任务堆栈*/
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];
/*定时器任务任务堆栈*/
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];
/*AppTaskCreate 任务堆栈*/
static StackType_t AppTaskCreate_Stack[128];
/*LED任务堆栈*/
static StackType_t LED_Task_Stack[128];

/*空闲任务控制块*/
static StaticTask_t Idle_Task_TCB;
/*定时器任务控制块*/
static StaticTask_t Timer_Task_TCB;
/*AppTaskCreate 任务控制块*/
static StaticTask_t AppTaskCreate_TCB;
/*LED 任务控制块*/
static StaticTask_t LED_Task_TCB;
/*
**********************************************************************************************
*																			函数声明
**********************************************************************************************
*/

static void BSP_Init(void);

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
	                                 StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize);
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
	                                 StackType_t **ppxTimerTaskStackBuffer,
                                   uint32_t *pulTimerTaskStackSize);
static void AppTaskCreate(void);
/*
**********************************************************************************************
*																			main函数
**********************************************************************************************
*/
int main(void)
{
	/*开发板硬件初始化*/
	BSP_Init();
	/*创建AppTaskCreate任务*/
	AppTaskCreate_Handle = xTaskCreateStatic((TaskFunction_t) AppTaskCreate,  //任务函数
	                                          (const char *) "AppTaskCreate",  //任务名称
																						(uint32_t) 128,                 //堆栈大小
																						(void *)NULL,                   //任务参数
																						(UBaseType_t) 3,                //任务优先级
																						(StackType_t *)AppTaskCreate_Stack,//任务堆栈
	                                          (StaticTask_t *)&AppTaskCreate_TCB);//任务控制块
	if(NULL != AppTaskCreate_Handle)
	{
		vTaskStartScheduler();//启动任务
	}
	while(1)
	{

	}

}
 /** 
 * 板级初始化函数
 * @param NULL 
 * @retval NULL
 */ 
static void BSP_Init(void)
{
	/*STM32中断优先级组为4，范围0~15，不要再分组了*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	/*LED初始化*/
	LED_Configer();
	
	LED_G(ON);
	
	/*串口初始化*/
	USART_Config();
}

 /** 
 * 使LED以500ms的间隔闪烁
 * @param[in]   *parameter  
 * @retval VOID
 */ 
static void LED_Task(void* parameter)
{
	while(1)
	{
		LED_G(ON);
		vTaskDelay(500);
		LED_G(OFF);
		vTaskDelay(500);
	}
}

 /** 
 * 获取空闲任务的任务堆栈和任务控制块内存 
 * @param[in]   ppxIdleTaskTCBBuffer   : 任务控制块内存
 *              ppxIdleTaskStackBuffer : 任务堆栈内存
 *              pulIdleTaskStackSize   : 任务堆栈大小   
 * @retval void
 */ 
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
	                                 StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
	*ppxIdleTaskTCBBuffer = &Idle_Task_TCB; /*任务控制块内存*/
	*ppxIdleTaskStackBuffer = Idle_Task_Stack; /*任务堆栈内存*/
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;/*任务堆栈大小*/
}
 /** 
 * 获取定时器任务的任务堆栈和任务控制块内存 
 * @param[in]   ppxTimerTaskTCBBuffer   : 任务控制块内存
 *              ppxTimerTaskStackBuffer : 任务堆栈内存
 *              pulTimerTaskStackSize   : 任务堆栈大小   
 * @retval void
 */ 
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
	                                 StackType_t **ppxTimerTaskStackBuffer,
                                   uint32_t *pulTimerTaskStackSize)
{
	*ppxTimerTaskTCBBuffer = &Timer_Task_TCB; /*任务控制块内存*/
	*ppxTimerTaskStackBuffer = Timer_Task_Stack; /*任务堆栈内存*/
	*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;/*任务堆栈大小*/
}
 /** 
 * 创建所有的任务
 * @param  无  
 * @retval 无
 */ 
static void AppTaskCreate(void)
{
	taskENTER_CRITICAL();   //进入临界区
	/*创建LED_Task任务*/
	LED_Task_Handle = xTaskCreateStatic((TaskFunction_t) LED_Task,  //任务函数
	                                          (const char *) "LED_Task",  //任务名称
																						(uint32_t) 128,                 //堆栈大小
																						(void *)NULL,                   //任务参数
																						(UBaseType_t) 4,                //任务优先级
																						(StackType_t *)LED_Task_Stack,//任务堆栈
	                                          (StaticTask_t *)&LED_Task_TCB);
	if(NULL != LED_Task_Handle)
		printf("LED_Task任务创建成功\n");
	else
		printf("LED_Task任务创建失败\n");
	
	vTaskDelete(AppTaskCreate_Handle); /*删除AppTaskCreate*/
	
	taskEXIT_CRITICAL();         //退出临界区
}
