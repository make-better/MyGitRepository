
/*
**********************************************************************************************
*																			头文件
**********************************************************************************************
*/
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_led.h"
#include "bsp_key.h"
/*
**********************************************************************************************
*																			任务句柄
**********************************************************************************************
*/
 /* 创建任务句柄 */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* LED1任务句柄 */
static TaskHandle_t LED1_Task_Handle = NULL;	
/* 按键扫描任务句柄 */
static TaskHandle_t KEY_Task_Handle = NULL;	

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
static void KEY_Task(void* parameter);
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
	/*按键初始化*/
	Key_Configer();
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
 * k1 k2 按键扫描
 * @param[in]   *parameter  
 * @retval VOID
 */ 
static void KEY_Task(void* parameter)
{
	while(1)
	{
		if(ON == Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN))
		{/*key1 被按下*/
			printf("挂起LED1任务\n");
			vTaskSuspend(LED1_Task_Handle);/*挂起任务LED1*/
		}
		if(ON == Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN))
		{/*key2 被按下*/
			printf("恢复LED1任务\n");
			vTaskResume(LED1_Task_Handle);/*挂起任务LED1*/
		}
		vTaskDelay(20);/*延时*/
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
	xReturn = xTaskCreate((TaskFunction_t) KEY_Task,  //任务函数
	                      (const char *) "KEY_Task",  //任务名称
												(uint16_t) 512,                 //堆栈大小
												(void *)NULL,                   //任务参数
												(UBaseType_t) 1,                //任务优先级
												(TaskHandle_t *)&KEY_Task_Handle);
	if(NULL != KEY_Task_Handle)
		printf("KEY_Task任务创建成功\n");
	else
		printf("KEY_Task任务创建失败\n");
	
	vTaskDelete(AppTaskCreate_Handle); /*删除AppTaskCreate*/
	
	taskEXIT_CRITICAL();         //退出临界区
}
