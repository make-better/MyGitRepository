
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
static TaskHandle_t LowPriority_Task_Handle = NULL;  /*低优先级任务句柄*/
static TaskHandle_t MidPriority_Task_Handle = NULL;  /*中优先级任务句柄*/
static TaskHandle_t HighPriority_Task_Handle = NULL; /*高优先级任务句柄*/
/*
**********************************************************************************************
*																			内核对象句柄
**********************************************************************************************
*/
SemaphoreHandle_t BinarySem_Handle = NULL;

/*
**********************************************************************************************
*																			函数声明
**********************************************************************************************
*/
static void BSP_Init(void);
static void LowPriority_Task(void *parameter);
static void MidPriority_Task(void *parameter);
static void HighPriority_Task(void *parameter);
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
												(uint16_t)  128,
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
	
	/*创建BinarySem*/
	BinarySem_Handle = xSemaphoreCreateBinary();
	
	if(NULL != BinarySem_Handle)
	{
		printf("BinarySem_Handle 二值信号量创建成功\r\n");
	}
	xReturn = xSemaphoreGive(BinarySem_Handle);//释放二值信号量
	/*创建 LowPriority_Task 任务*/
	xReturn = xTaskCreate((TaskFunction_t)LowPriority_Task,
	                      (const char *) "LowPriority_Task",
												(uint16_t)  128,
												(void*)  NULL,
												(UBaseType_t) 2,
												(TaskHandle_t*)&LowPriority_Task_Handle);
	
	if(pdPASS == xReturn)
	{
		printf("任务 LowPriority_Task 创建成功\r\n");
	}
	
	/*创建 MidPriority_Task 任务*/
	xReturn = xTaskCreate((TaskFunction_t)MidPriority_Task,
	                      (const char *) "MidPriority_Task",
												(uint16_t)  128,
												(void*)  NULL,
												(UBaseType_t) 3,
												(TaskHandle_t*)&MidPriority_Task_Handle);
	
	if(pdPASS == xReturn)
	{
		printf("任务 MidPriority_Task 创建成功\r\n");
	}
	
	/*创建 HighPriority_Task 任务*/
	xReturn = xTaskCreate((TaskFunction_t)HighPriority_Task,
	                      (const char *) "HighPriority_Task",
												(uint16_t)  128,
												(void*)  NULL,
												(UBaseType_t) 4,
												(TaskHandle_t*)&HighPriority_Task_Handle);
	
	if(pdPASS == xReturn)
	{
		printf("任务 HighPriority_Task 创建成功\r\n");
	}
	
	vTaskDelete(AppTaskCreate_Handle);   //删除AppTaskCreate任务
	
	taskEXIT_CRITICAL();         //退出临界区
	
}

/*********************************
* @name   LowPriority_Task
* @brief  LowPriority_Task任务
* @param  无
* @return 无
**********************************/
static void LowPriority_Task(void *parameter)
{
	static uint32_t i;
	BaseType_t xReturn = pdPASS;  
	while(1)
	{
		printf("LowPriority_Task获取信号量\n");
		xReturn = xSemaphoreTake(BinarySem_Handle,portMAX_DELAY); // 获取一个计数信号量,一直等待
				
		if(pdTRUE == xReturn)
		{
			printf("LowPriority_Task Running\r\n");
		}
		
		for(i = 0;i<200000;i++)//模拟低优先级任务占用信号量
		{
			taskYIELD();//发起任务调度，嘲讽HighPriority_Task
		}
		
		printf("LowPriority_Task 释放信号量\r\n");
		xReturn = xSemaphoreGive(BinarySem_Handle);//释放二值信号量
		
		LED1_TOGGLE;
		
		vTaskDelay(500);
	}
}

/*********************************
* @name   MidPriority_Task
* @brief  MidPriority_Task任务
* @param  无
* @return 无
**********************************/
static void MidPriority_Task(void *parameter)
{  
	while(1)
	{
		printf("MidPriority_Task Running\r\n");
		vTaskDelay(1000);
	}
}
/*********************************
* @name   HighPriority_Task
* @brief  HighPriority_Task
* @param  无
* @return 无
**********************************/
static void HighPriority_Task(void *parameter)
{  
	BaseType_t xReturn = pdPASS; 
	while(1)
	{
		printf("HighPriority_Task 获取信号量\n");
		/*获取二值信号量BinarySem_Handle,没有就一直等*/
		xReturn = xSemaphoreTake(BinarySem_Handle,  /*二值信号量句柄*/
															portMAX_DELAY);  /*一直等*/
		if(xReturn == pdPASS)
		{
			printf("HighPriority_Task Running\r\n");
		}
		LED1_TOGGLE;
		xReturn = xSemaphoreGive(BinarySem_Handle);/*释放二值信号量*/
		vTaskDelay(500);
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





