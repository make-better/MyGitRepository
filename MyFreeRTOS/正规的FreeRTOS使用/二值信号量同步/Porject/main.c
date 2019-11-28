
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
static TaskHandle_t Receive_Task_Handle = NULL;  /*信号量接收任务句柄*/
static TaskHandle_t Send_Task_Handle = NULL;     /*信号量释放任务句柄*/

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
static void Send_Task(void *parameter);
static void Receive_Task(void *parameter);
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
	
	/*创建BinarySem*/
	BinarySem_Handle = xSemaphoreCreateBinary();
	
	if(NULL != BinarySem_Handle)
	{
		printf("BinarySem_Handle 二值信号量创建成功\r\n");
	}
	
	/*创建 Receive_Task 任务*/
	xReturn = xTaskCreate((TaskFunction_t)Receive_Task,
	                      (const char *) "Receive_Task",
												(uint16_t)  512,
												(void*)  NULL,
												(UBaseType_t) 2,
												(TaskHandle_t*)&Receive_Task_Handle);
	
	if(pdPASS == xReturn)
	{
		printf("任务 Receive_Task 创建成功\r\n");
	}
	
	/*创建 Send_Task 任务*/
	xReturn = xTaskCreate((TaskFunction_t)Send_Task,
	                      (const char *) "Send_Task",
												(uint16_t)  512,
												(void*)  NULL,
												(UBaseType_t) 3,
												(TaskHandle_t*)&Send_Task_Handle);
	
	if(pdPASS == xReturn)
	{
		printf("任务 Send_Task 创建成功\r\n");
	}
	
	vTaskDelete(AppTaskCreate_Handle);   //删除AppTaskCreate任务
	
	taskEXIT_CRITICAL();         //退出临界区
	
}

/*********************************
* @name   Receive_Task
* @brief  Receive任务，有信号量就闪一次LED
* @param  无
* @return 无
**********************************/
static void Receive_Task(void *parameter)
{
	BaseType_t xReturn = pdPASS;  
	while(1)
	{
		/*获取二值信号量，没有就一直等*/
		xReturn = xSemaphoreTake(BinarySem_Handle, //二值信号量句柄
		                         portMAX_DELAY);   //等待时间
		if(pdTRUE == xReturn)
		{
			printf("BinarySem_Handle二值信号量获取成功\r\n");
			LED1_TOGGLE;
		}
	}
}

/*********************************
* @name   Send_Task
* @brief  Send_Task任务，按一次key1释放一次信号量 BinarySem_Handle
* @param  无
* @return 无
**********************************/
static void Send_Task(void *parameter)
{
	BaseType_t xReturn = pdPASS;  
	while(1)
	{
		if(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON)
		{
			xReturn = xSemaphoreGive(BinarySem_Handle); // 释放二值信号量
				
			if(pdTRUE == xReturn)
			{
				printf("BinarySem_Handle二值信号量释放成功\r\n");
			}
			else
			{
				printf("BinarySem_Handle二值信号量释放失败\r\n");
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





