
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
static TaskHandle_t LowPriority_Task_Handle = NULL;  /*�����ȼ�������*/
static TaskHandle_t MidPriority_Task_Handle = NULL;  /*�����ȼ�������*/
static TaskHandle_t HighPriority_Task_Handle = NULL; /*�����ȼ�������*/
/*
**********************************************************************************************
*																			�ں˶�����
**********************************************************************************************
*/
SemaphoreHandle_t BinarySem_Handle = NULL;

/*
**********************************************************************************************
*																			��������
**********************************************************************************************
*/
static void BSP_Init(void);
static void LowPriority_Task(void *parameter);
static void MidPriority_Task(void *parameter);
static void HighPriority_Task(void *parameter);
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
												(uint16_t)  128,
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
	
	/*����BinarySem*/
	BinarySem_Handle = xSemaphoreCreateBinary();
	
	if(NULL != BinarySem_Handle)
	{
		printf("BinarySem_Handle ��ֵ�ź��������ɹ�\r\n");
	}
	xReturn = xSemaphoreGive(BinarySem_Handle);//�ͷŶ�ֵ�ź���
	/*���� LowPriority_Task ����*/
	xReturn = xTaskCreate((TaskFunction_t)LowPriority_Task,
	                      (const char *) "LowPriority_Task",
												(uint16_t)  128,
												(void*)  NULL,
												(UBaseType_t) 2,
												(TaskHandle_t*)&LowPriority_Task_Handle);
	
	if(pdPASS == xReturn)
	{
		printf("���� LowPriority_Task �����ɹ�\r\n");
	}
	
	/*���� MidPriority_Task ����*/
	xReturn = xTaskCreate((TaskFunction_t)MidPriority_Task,
	                      (const char *) "MidPriority_Task",
												(uint16_t)  128,
												(void*)  NULL,
												(UBaseType_t) 3,
												(TaskHandle_t*)&MidPriority_Task_Handle);
	
	if(pdPASS == xReturn)
	{
		printf("���� MidPriority_Task �����ɹ�\r\n");
	}
	
	/*���� HighPriority_Task ����*/
	xReturn = xTaskCreate((TaskFunction_t)HighPriority_Task,
	                      (const char *) "HighPriority_Task",
												(uint16_t)  128,
												(void*)  NULL,
												(UBaseType_t) 4,
												(TaskHandle_t*)&HighPriority_Task_Handle);
	
	if(pdPASS == xReturn)
	{
		printf("���� HighPriority_Task �����ɹ�\r\n");
	}
	
	vTaskDelete(AppTaskCreate_Handle);   //ɾ��AppTaskCreate����
	
	taskEXIT_CRITICAL();         //�˳��ٽ���
	
}

/*********************************
* @name   LowPriority_Task
* @brief  LowPriority_Task����
* @param  ��
* @return ��
**********************************/
static void LowPriority_Task(void *parameter)
{
	static uint32_t i;
	BaseType_t xReturn = pdPASS;  
	while(1)
	{
		printf("LowPriority_Task��ȡ�ź���\n");
		xReturn = xSemaphoreTake(BinarySem_Handle,portMAX_DELAY); // ��ȡһ�������ź���,һֱ�ȴ�
				
		if(pdTRUE == xReturn)
		{
			printf("LowPriority_Task Running\r\n");
		}
		
		for(i = 0;i<200000;i++)//ģ������ȼ�����ռ���ź���
		{
			taskYIELD();//����������ȣ�����HighPriority_Task
		}
		
		printf("LowPriority_Task �ͷ��ź���\r\n");
		xReturn = xSemaphoreGive(BinarySem_Handle);//�ͷŶ�ֵ�ź���
		
		LED1_TOGGLE;
		
		vTaskDelay(500);
	}
}

/*********************************
* @name   MidPriority_Task
* @brief  MidPriority_Task����
* @param  ��
* @return ��
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
* @param  ��
* @return ��
**********************************/
static void HighPriority_Task(void *parameter)
{  
	BaseType_t xReturn = pdPASS; 
	while(1)
	{
		printf("HighPriority_Task ��ȡ�ź���\n");
		/*��ȡ��ֵ�ź���BinarySem_Handle,û�о�һֱ��*/
		xReturn = xSemaphoreTake(BinarySem_Handle,  /*��ֵ�ź������*/
															portMAX_DELAY);  /*һֱ��*/
		if(xReturn == pdPASS)
		{
			printf("HighPriority_Task Running\r\n");
		}
		LED1_TOGGLE;
		xReturn = xSemaphoreGive(BinarySem_Handle);/*�ͷŶ�ֵ�ź���*/
		vTaskDelay(500);
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





