
/*
**********************************************************************************************
*																			头文件
**********************************************************************************************
*/
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_key.h"
#include "bsp_usart.h"
#include "event_groups.h"
#include "bsp_led.h"
/*
**********************************************************************************************
*																			任务句柄
**********************************************************************************************
*/
static TaskHandle_t AppTaskCreate_Handle = NULL;
static TaskHandle_t LED_Task_Handle = NULL;
static TaskHandle_t KEY_Task_Handle = NULL;
/*
**********************************************************************************************
*																			内核对象句柄
**********************************************************************************************
*/
static EventGroupHandle_t Event_Handle = NULL;

/*
**********************************************************************************************
*																			全局变量声明
**********************************************************************************************
*/

/*
**********************************************************************************************
*																			宏定义
**********************************************************************************************
*/
#define KEY1_EVENT		(0x01<<0)//事件掩码的0位
#define KEY2_EVENT		(0x01<<1)//事件掩码的1位
/*
**********************************************************************************************
*																			函数声明
**********************************************************************************************
*/
static void AppTaskCreate(void);
static void LED_Task(void* pvParameters);
static void KEY_Task(void* pvParameters);
static void BSP_Init(void);
/*
**********************************************************************************************
*																			main函数
**********************************************************************************************
*/
int main(void)
{
	BaseType_t xReturn = pdPASS;
	//开发板初始化
	BSP_Init();
	printf("FreeRTOS 事件 测试程序\n");
	xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate, //任务入口
												(const char*)"AppTaskCreate",	//任务名称
												(uint16_t)512,						//堆栈大小
												(void *)NULL,							//任务参数
												(UBaseType_t)1,						//任务优先级
												(TaskHandle_t *)&AppTaskCreate_Handle);//任务控制块
	if(pdPASS == xReturn)
	{
		vTaskStartScheduler();//启动任务，开启调度
	}
	else
	{
		printf("创建任务失败\n");
	}
		
	while(1)
	{

	}

}
/**
**************************************************
* @name : AppTaskCreate
* @brief : 创建所有任务
* @para : no
* @return : no
**************************************************
*/
static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;
	taskENTER_CRITICAL(); //进入临界区
	
	/*创建 Event_Handle*/
	Event_Handle = xEventGroupCreate();
	if(NULL != Event_Handle)
		printf("Event_Handle事件创建成功\r\n");
	
	/*创建LED_Task任务*/
	xReturn = xTaskCreate((TaskFunction_t)LED_Task, //任务入口
												(const char*)"LED_Task",	//任务名称
												(uint16_t)512,						//堆栈大小
												(void *)NULL,							//任务参数
												(UBaseType_t)2,						//任务优先级
												(TaskHandle_t *)&LED_Task_Handle);//任务控制块
	if(pdPASS == xReturn)
	{
		printf("创建LED_Task任务成功\r\n");
	}
	
	/*创建KEY_Task任务*/
	xReturn = xTaskCreate((TaskFunction_t)KEY_Task, //任务入口
												(const char*)"KEY_Task",	//任务名称
												(uint16_t)512,						//堆栈大小
												(void *)NULL,							//任务参数
												(UBaseType_t)3,						//任务优先级
												(TaskHandle_t *)&KEY_Task_Handle);//任务控制块
	if(pdPASS == xReturn)
	{
		printf("创建KEY_Task任务成功\r\n");
	}
	
	vTaskDelete(AppTaskCreate_Handle);//删除
	
	taskEXIT_CRITICAL();//退出临界区
}
/**
**************************************************
* @name : LED_Task
* @brief : LED_Task任务主体
* @para : no
* @return : no
**************************************************
*/
static void LED_Task(void* pvParameters)
{
	EventBits_t r_event; //事件接收变量
	while(1)
	{
		r_event = xEventGroupWaitBits(Event_Handle, //事件对象句柄
													KEY1_EVENT|KEY2_EVENT,//感兴趣的事件
													pdTRUE,			//退出时清除事件位
													pdTRUE,			//满足感兴趣的所有事件
													portMAX_DELAY);//超时时间 一直等
		if((r_event & (KEY1_EVENT|KEY2_EVENT)) == (KEY1_EVENT|KEY2_EVENT))
		{//接收完成并且正确
			printf("KEY1 KEY2都按下\n");
			LED_G_Toggle;
		}
		else
		{
			printf("事件错误\n");
		}
	}
}

/**
**************************************************
* @name : KEY_Task
* @brief : KEY_Task体
* @para : no
* @return : no
**************************************************
*/
static void KEY_Task(void* pvParameters)
{
	while(1)
	{
		//如果KEY1被按下
		if(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON)
		{
			printf("key1被按下\n");
			//触发事件1
			xEventGroupSetBits(Event_Handle,KEY1_EVENT);
		}
		//如果KEY2被按下
		if(Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON)
		{
			printf("key2被按下\n");
			//触发事件2
			xEventGroupSetBits(Event_Handle,KEY2_EVENT);
		}
		vTaskDelay(20);
	}
}

/**
**************************************************
* @name : BSP_Init
* @brief : 初始化外设
* @para : no
* @return : no
**************************************************
*/
static void BSP_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	Key_Configer();
	LED_Configer();
	USART_Config();
}