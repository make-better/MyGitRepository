
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
static TaskHandle_t Receive_Task_Handle = NULL;  /*�ź�������������*/
static TaskHandle_t Send_Task_Handle = NULL;     /*�ź����ͷ�������*/

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
static void Send_Task(void *parameter);
static void Receive_Task(void *parameter);
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
	
	/*����BinarySem*/
	BinarySem_Handle = xSemaphoreCreateBinary();
	
	if(NULL != BinarySem_Handle)
	{
		printf("BinarySem_Handle ��ֵ�ź��������ɹ�\r\n");
	}
	
	/*���� Receive_Task ����*/
	xReturn = xTaskCreate((TaskFunction_t)Receive_Task,
	                      (const char *) "Receive_Task",
												(uint16_t)  512,
												(void*)  NULL,
												(UBaseType_t) 2,
												(TaskHandle_t*)&Receive_Task_Handle);
	
	if(pdPASS == xReturn)
	{
		printf("���� Receive_Task �����ɹ�\r\n");
	}
	
	/*���� Send_Task ����*/
	xReturn = xTaskCreate((TaskFunction_t)Send_Task,
	                      (const char *) "Send_Task",
												(uint16_t)  512,
												(void*)  NULL,
												(UBaseType_t) 3,
												(TaskHandle_t*)&Send_Task_Handle);
	
	if(pdPASS == xReturn)
	{
		printf("���� Send_Task �����ɹ�\r\n");
	}
	
	vTaskDelete(AppTaskCreate_Handle);   //ɾ��AppTaskCreate����
	
	taskEXIT_CRITICAL();         //�˳��ٽ���
	
}

/*********************************
* @name   Receive_Task
* @brief  Receive�������ź�������һ��LED
* @param  ��
* @return ��
**********************************/
static void Receive_Task(void *parameter)
{
	BaseType_t xReturn = pdPASS;  
	while(1)
	{
		/*��ȡ��ֵ�ź�����û�о�һֱ��*/
		xReturn = xSemaphoreTake(BinarySem_Handle, //��ֵ�ź������
		                         portMAX_DELAY);   //�ȴ�ʱ��
		if(pdTRUE == xReturn)
		{
			printf("BinarySem_Handle��ֵ�ź�����ȡ�ɹ�\r\n");
			LED1_TOGGLE;
		}
	}
}

/*********************************
* @name   Send_Task
* @brief  Send_Task���񣬰�һ��key1�ͷ�һ���ź��� BinarySem_Handle
* @param  ��
* @return ��
**********************************/
static void Send_Task(void *parameter)
{
	BaseType_t xReturn = pdPASS;  
	while(1)
	{
		if(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON)
		{
			xReturn = xSemaphoreGive(BinarySem_Handle); // �ͷŶ�ֵ�ź���
				
			if(pdTRUE == xReturn)
			{
				printf("BinarySem_Handle��ֵ�ź����ͷųɹ�\r\n");
			}
			else
			{
				printf("BinarySem_Handle��ֵ�ź����ͷ�ʧ��\r\n");
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





