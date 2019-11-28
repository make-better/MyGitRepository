
/*
**********************************************************************************************
*																			头文件
**********************************************************************************************
*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
     /*bsp*/
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_usart.h"


/*
**********************************************************************************************
*																			任务句柄
**********************************************************************************************
*/
static TaskHandle_t AppTaskCreate_Handle = NULL; /*创建任务句柄*/
static TaskHandle_t Take_Task_Handle = NULL;  /*信号量获取任务句柄*/
static TaskHandle_t Give_Task_Handle = NULL;     /*信号量释放任务句柄*/

/*
**********************************************************************************************
*																			内核对象句柄
**********************************************************************************************
*/
SemaphoreHandle_t CountSem_Handle = NULL;

/*
**********************************************************************************************
*																			函数声明
**********************************************************************************************
*/
static void BSP_Init(void);
static void Take_Task(void *parameter);
static void Give_Task(void *parameter);
static void AppTaskCreate(void);
/*
**********************************************************************************************
*																			main函数
**********************************************************************************************
*/
int main(void)
{
  BaseType_t xReturn = pdPASS; //创建信息返回值
	/*开发板硬件初始化*/
	BSP_Init();
	/*创建AppTaskCreate任务*/
	xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,
	                      (const char *) "AppTaskCreate",
												(uint16_t)  512,
												(void*)  NULL,
												(UBaseType_t) 1,
												(TaskHandle_t*)&AppTaskCreate_Handle);
	if(xReturn == pdPASS)
	{
		vTaskStartScheduler(); //开启调度
	}
	else
		return -1;
	while(1)
	{

	}

}

/*********************************
* @name   AppTaskCreate
* @brief  创建任务的任务
* @param  无
* @return 无
**********************************/
static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;   //创建信息返回值
	taskENTER_CRITICAL();        //进入临界段
	
	/*创建CountSem*/
	CountSem_Handle = xSemaphoreCreateCounting(5,5);
	
	if(NULL != CountSem_Handle)
	{
		printf("CountSem_Handle 计数信号量创建成功\r\n");
	}
	
	/*创建 Take_Task 任务*/
	xReturn = xTaskCreate((TaskFunction_t)Take_Task,
	                      (const char *) "Take_Task",
												(uint16_t)  512,
												(void*)  NULL,
												(UBaseType_t) 2,
												(TaskHandle_t*)&Take_Task_Handle);
	
	if(pdPASS == xReturn)
	{
		printf("任务 Take_Task 创建成功\r\n");
	}
	
	/*创建 Give_Task 任务*/
	xReturn = xTaskCreate((TaskFunction_t)Give_Task,
	                      (const char *) "Give_Task",
												(uint16_t)  512,
												(void*)  NULL,
												(UBaseType_t) 3,
												(TaskHandle_t*)&Give_Task_Handle);
	
	if(pdPASS == xReturn)
	{
		printf("任务 Give_Task 创建成功\r\n");
	}
	
	vTaskDelete(AppTaskCreate_Handle);   //删除AppTaskCreate任务
	
	taskEXIT_CRITICAL();         //退出临界区
	
}

/*********************************
* @name   Take_Task
* @brief  Take任务，按下KEY1获取一次信号量
* @param  无
* @return 无
**********************************/
static void Take_Task(void *parameter)
{
	BaseType_t xReturn = pdPASS;  
	while(1)
	{
		if(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON)
		{
			xReturn = xSemaphoreTake(CountSem_Handle,0); // 获取一个计数信号量,不等待
				
			if(pdTRUE == xReturn)
			{
				printf("KEY1 被按下，成功申请到停车位\r\n");
			}
			else
			{
				printf("KEY1 被按下，但停车位已满\r\n");
			}
		}
		vTaskDelay(20);
	}
}

/*********************************
* @name   Give_Task
* @brief  Give_Task任务，按一次key2释放一次信号量
* @param  无
* @return 无
**********************************/
static void Give_Task(void *parameter)
{
	BaseType_t xReturn = pdPASS;  
	while(1)
	{
		if(Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON)
		{
			xReturn = xSemaphoreGive(CountSem_Handle); // 释放计数信号量
				
			if(pdTRUE == xReturn)
			{
				printf("KEY2 被按下，释放1个停车位。\r\n");
			}
			else
			{
				printf("KEY2 被按下，停车场空了，没有停车位可以释放了。\r\n");
			}
		}
		vTaskDelay(20);
	}
}

/*********************************
* @name   BSP_Init
* @brief  板级外设初始化
* @param  无
* @return 无
**********************************/
static void BSP_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	/*LED初始化*/
	LED_Configer();
	/*串口初始化*/
	USART_Config();
	/*按键初始化*/
	Key_Configer();
}





