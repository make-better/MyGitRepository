
/*
**********************************************************************************************
*																			头文件
**********************************************************************************************
*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "bsp_led.h"
#include "bsp_key.h"
/*
**********************************************************************************************
*																			任务句柄
**********************************************************************************************
*/
 /* 创建任务句柄 */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* 消息接收任务句柄 */
static TaskHandle_t Receive_Task_Handle = NULL;	
/* 消息发送任务句柄 */
static TaskHandle_t Send_Task_Handle = NULL;	

/*
**********************************************************************************************
*																			内核对象句柄
**********************************************************************************************
*/
QueueHandle_t Test_Queue = NULL;


/*
**********************************************************************************************
*																			宏定义
**********************************************************************************************
*/
#define QUEUE_LEN 4
#define QUEUE_SIZE 4
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
static void Receive_Task(void* parameter);
static void Send_Task(void* parameter);
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
 * @brief 板级初始化函数
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
 * @brief Receive函数，收到消息就用串口输出
 * @param[in]   *parameter  
 * @retval VOID
 */ 
static void Receive_Task(void* parameter)
{
	BaseType_t xReturn = pdTRUE;  /*创建信息返回值*/
	uint32_t r_queue;  /*接收消息的变量*/
	while(1)
	{
		xReturn = xQueueReceive( Test_Queue, /*消息队列句柄*/
		                         &r_queue,      /*发送的内容*/
		                         portMAX_DELAY);/*等待时间，一直等*/
		
		if(pdTRUE == xReturn)
			printf("本次接收到的数据是%d\n",r_queue);
		else
			printf("本次接收出错，错误代码：0x%lx\n",xReturn);
	}
}
 /** 
 * @brief Send 发送消息
 * @param[in]   *parameter  
 * @retval VOID
 */ 
static void Send_Task(void* parameter)
{
	BaseType_t xReturn = pdPASS;  /*创建信息返回值*/
	uint32_t send_data1 = 1;
	uint32_t send_data2 = 2;
	while(1)
	{
		if(ON == Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN))
		{/*key1 被按下*/
			printf("发送消息send_data1\n");
			xReturn = xQueueSend( Test_Queue, /*消息队列句柄*/
			                      &send_data1, /*要发送的消息*/
			                      0);       /*等待时间 不等待*/
			if(xReturn == pdPASS)
			{
				printf("消息send_data1发送成功\n\n");
			}
		}
		if(ON == Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN))
		{/*key2 被按下*/
			printf("发送消息send_data2\n");
			xReturn = xQueueSend( Test_Queue, /*消息队列句柄*/
			                      &send_data2, /*要发送的消息*/
			                      0);       /*等待时间 不等待*/
			if(xReturn == pdPASS)
			{
				printf("消息send_data2发送成功\n\n");
			}
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
	/*创建Test_Queue*/
	Test_Queue = xQueueCreate( (UBaseType_t) QUEUE_LEN, /*消息队列的长度*/
	                           (UBaseType_t) QUEUE_SIZE); /*消息队列的大小*/
	if(NULL != Test_Queue)
     printf("消息队列Test_Queue创建成功\n");
	
	/*创建Receive_Task任务*/
	xReturn = xTaskCreate((TaskFunction_t) Receive_Task,  //任务函数
	                      (const char *) "Receive_Task",  //任务名称
												(uint16_t) 512,                 //堆栈大小
												(void *)NULL,                   //任务参数
												(UBaseType_t) 2,                //任务优先级
												(TaskHandle_t *)&Receive_Task_Handle);
	if(NULL != Receive_Task_Handle)
		printf("Receive_Task任务创建成功\n");
	else
		printf("Receive_Task任务创建失败\n");
	
	/*创建Send_Task任务*/
	xReturn = xTaskCreate((TaskFunction_t) Send_Task,  //任务函数
	                      (const char *) "Send_Task",  //任务名称
												(uint16_t) 512,                 //堆栈大小
												(void *)NULL,                   //任务参数
												(UBaseType_t) 3,                //任务优先级
												(TaskHandle_t *)&Send_Task_Handle);
	if(NULL != Send_Task_Handle)
		printf("Send_Task任务创建成功\n");
	else
		printf("Send_Task任务创建失败\n");
	
	vTaskDelete(AppTaskCreate_Handle); /*删除AppTaskCreate*/
	
	taskEXIT_CRITICAL();         //退出临界区
}
