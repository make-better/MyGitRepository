
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
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* LED1任务句柄 */
static TaskHandle_t LED1_Task_Handle = NULL;	
/* LED2任务句柄 */
static TaskHandle_t LED2_Task_Handle = NULL;	

/*
**********************************************************************************************
*																			变量
**********************************************************************************************
*/

/*
**********************************************************************************************
*																			函数声明
**********************************************************************************************
*/

static void BSP_Init(void);
static void LED1_Task(void* parameter);
static void LED2_Task(void* parameter);
static void AppTaskCreate(void);
/*
**********************************************************************************************
*																			main函数
**********************************************************************************************
*/
int main(void)
{
	BaseType_t xReturn = pdPASS; /*创建信息返回值*/
	/*开发板硬件初始化*/
	BSP_Init();
	/*创建AppTaskCreate任务*/
	xReturn = xTaskCreate((TaskFunction_t) AppTaskCreate,  //任务函数
	                                   (const char *) "AppTaskCreate",  //任务名称
																		 (uint16_t) 512,                 //堆栈大小
																		 (void *)NULL,                   //任务参数
																		 (UBaseType_t) 1,                //任务优先级
	                                   (TaskHandle_t *)&AppTaskCreate_Handle);//任务句柄
	if(NULL != AppTaskCreate_Handle)
	{
		vTaskStartScheduler();//启动任务
	}else
	{
		return -1;
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
	
	/*串口初始化*/
	USART_Config();
}

 /** 
 * 使LED以500ms的间隔闪烁
 * @param[in]   *parameter  
 * @retval VOID
 */ 
static void LED1_Task(void* parameter)
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
 * 使LED以300ms的间隔闪烁
 * @param[in]   *parameter  
 * @retval VOID
 */ 
static void LED2_Task(void* parameter)
{
	while(1)
	{
		LED_R(ON);
		vTaskDelay(300);
		LED_R(OFF);
		vTaskDelay(300);
	}
}
 /** 
 * 创建所有的任务
 * @param  无  
 * @retval 无
 */ 
static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;
	taskENTER_CRITICAL();   //进入临界区
	/*创建LED2_Task任务*/
	xReturn = xTaskCreate((TaskFunction_t) LED1_Task,  //任务函数
	                      (const char *) "LED1_Task",  //任务名称
												(uint16_t) 512,                 //堆栈大小
												(void *)NULL,                   //任务参数
												(UBaseType_t) 2,                //任务优先级
												(TaskHandle_t *)&LED1_Task_Handle);
	if(NULL != LED1_Task_Handle)
		printf("LED1_Task任务创建成功\n");
	else
		printf("LED1_Task任务创建失败\n");
	
	/*创建LED2_Task任务*/
	xReturn = xTaskCreate((TaskFunction_t) LED2_Task,  //任务函数
	                      (const char *) "LED2_Task",  //任务名称
												(uint16_t) 512,                 //堆栈大小
												(void *)NULL,                   //任务参数
												(UBaseType_t) 3,                //任务优先级
												(TaskHandle_t *)&LED2_Task_Handle);
	if(NULL != LED2_Task_Handle)
		printf("LED2_Task任务创建成功\n");
	else
		printf("LED2_Task任务创建失败\n");
	
	vTaskDelete(AppTaskCreate_Handle); /*删除AppTaskCreate*/
	
	taskEXIT_CRITICAL();         //退出临界区
}
