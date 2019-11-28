
/*
**********************************************************************************************
*																			ͷ�ļ�
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
*																			������
**********************************************************************************************
*/
static TaskHandle_t AppTaskCreate_Handle = NULL; /*����������*/
static TaskHandle_t Take_Task_Handle = NULL;  /*�ź�����ȡ������*/
static TaskHandle_t Give_Task_Handle = NULL;     /*�ź����ͷ�������*/

/*
**********************************************************************************************
*																			�ں˶�����
**********************************************************************************************
*/
SemaphoreHandle_t CountSem_Handle = NULL;

/*
**********************************************************************************************
*																			��������
**********************************************************************************************
*/
static void BSP_Init(void);
static void Take_Task(void *parameter);
static void Give_Task(void *parameter);
static void AppTaskCreate(void);
/*
**********************************************************************************************
*																			main����
**********************************************************************************************
*/
int main(void)
{
  BaseType_t xReturn = pdPASS; //������Ϣ����ֵ
	/*������Ӳ����ʼ��*/
	BSP_Init();
	/*����AppTaskCreate����*/
	xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,
	                      (const char *) "AppTaskCreate",
												(uint16_t)  512,
												(void*)  NULL,
												(UBaseType_t) 1,
												(TaskHandle_t*)&AppTaskCreate_Handle);
	if(xReturn == pdPASS)
	{
		vTaskStartScheduler(); //��������
	}
	else
		return -1;
	while(1)
	{

	}

}

/*********************************
* @name   AppTaskCreate
* @brief  �������������
* @param  ��
* @return ��
**********************************/
static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;   //������Ϣ����ֵ
	taskENTER_CRITICAL();        //�����ٽ��
	
	/*����CountSem*/
	CountSem_Handle = xSemaphoreCreateCounting(5,5);
	
	if(NULL != CountSem_Handle)
	{
		printf("CountSem_Handle �����ź��������ɹ�\r\n");
	}
	
	/*���� Take_Task ����*/
	xReturn = xTaskCreate((TaskFunction_t)Take_Task,
	                      (const char *) "Take_Task",
												(uint16_t)  512,
												(void*)  NULL,
												(UBaseType_t) 2,
												(TaskHandle_t*)&Take_Task_Handle);
	
	if(pdPASS == xReturn)
	{
		printf("���� Take_Task �����ɹ�\r\n");
	}
	
	/*���� Give_Task ����*/
	xReturn = xTaskCreate((TaskFunction_t)Give_Task,
	                      (const char *) "Give_Task",
												(uint16_t)  512,
												(void*)  NULL,
												(UBaseType_t) 3,
												(TaskHandle_t*)&Give_Task_Handle);
	
	if(pdPASS == xReturn)
	{
		printf("���� Give_Task �����ɹ�\r\n");
	}
	
	vTaskDelete(AppTaskCreate_Handle);   //ɾ��AppTaskCreate����
	
	taskEXIT_CRITICAL();         //�˳��ٽ���
	
}

/*********************************
* @name   Take_Task
* @brief  Take���񣬰���KEY1��ȡһ���ź���
* @param  ��
* @return ��
**********************************/
static void Take_Task(void *parameter)
{
	BaseType_t xReturn = pdPASS;  
	while(1)
	{
		if(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON)
		{
			xReturn = xSemaphoreTake(CountSem_Handle,0); // ��ȡһ�������ź���,���ȴ�
				
			if(pdTRUE == xReturn)
			{
				printf("KEY1 �����£��ɹ����뵽ͣ��λ\r\n");
			}
			else
			{
				printf("KEY1 �����£���ͣ��λ����\r\n");
			}
		}
		vTaskDelay(20);
	}
}

/*********************************
* @name   Give_Task
* @brief  Give_Task���񣬰�һ��key2�ͷ�һ���ź���
* @param  ��
* @return ��
**********************************/
static void Give_Task(void *parameter)
{
	BaseType_t xReturn = pdPASS;  
	while(1)
	{
		if(Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON)
		{
			xReturn = xSemaphoreGive(CountSem_Handle); // �ͷż����ź���
				
			if(pdTRUE == xReturn)
			{
				printf("KEY2 �����£��ͷ�1��ͣ��λ��\r\n");
			}
			else
			{
				printf("KEY2 �����£�ͣ�������ˣ�û��ͣ��λ�����ͷ��ˡ�\r\n");
			}
		}
		vTaskDelay(20);
	}
}

/*********************************
* @name   BSP_Init
* @brief  �弶�����ʼ��
* @param  ��
* @return ��
**********************************/
static void BSP_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	/*LED��ʼ��*/
	LED_Configer();
	/*���ڳ�ʼ��*/
	USART_Config();
	/*������ʼ��*/
	Key_Configer();
}





