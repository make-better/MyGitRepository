
/*
**********************************************************************************************
*																			ͷ�ļ�
**********************************************************************************************
*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "bsp_led.h"
#include "bsp_key.h"
/*
**********************************************************************************************
*																			������
**********************************************************************************************
*/
 /* ���������� */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* ��Ϣ���������� */
static TaskHandle_t Receive_Task_Handle = NULL;	
/* ��Ϣ���������� */
static TaskHandle_t Send_Task_Handle = NULL;	

/*
**********************************************************************************************
*																			�ں˶�����
**********************************************************************************************
*/
QueueHandle_t Test_Queue = NULL;


/*
**********************************************************************************************
*																			�궨��
**********************************************************************************************
*/
#define QUEUE_LEN 4
#define QUEUE_SIZE 4
/*
**********************************************************************************************
*																			����
**********************************************************************************************
*/

/*
**********************************************************************************************
*																			��������
**********************************************************************************************
*/

static void BSP_Init(void);
static void Receive_Task(void* parameter);
static void Send_Task(void* parameter);
static void AppTaskCreate(void);
/*
**********************************************************************************************
*																			main����
**********************************************************************************************
*/
int main(void)
{
	BaseType_t xReturn = pdPASS; /*������Ϣ����ֵ*/
	/*������Ӳ����ʼ��*/
	BSP_Init();
	/*����AppTaskCreate����*/
	xReturn = xTaskCreate((TaskFunction_t) AppTaskCreate,  //������
	                                   (const char *) "AppTaskCreate",  //��������
																		 (uint16_t) 512,                 //��ջ��С
																		 (void *)NULL,                   //�������
																		 (UBaseType_t) 1,                //�������ȼ�
	                                   (TaskHandle_t *)&AppTaskCreate_Handle);//������
	if(NULL != AppTaskCreate_Handle)
	{
		vTaskStartScheduler();//��������
	}else
	{
		return -1;
	}
		
	while(1)
	{

	}

}
 /** 
 * @brief �弶��ʼ������
 * @param NULL 
 * @retval NULL
 */ 
static void BSP_Init(void)
{
	/*STM32�ж����ȼ���Ϊ4����Χ0~15����Ҫ�ٷ�����*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	/*LED��ʼ��*/
	LED_Configer();
	/*������ʼ��*/
	Key_Configer();
	/*���ڳ�ʼ��*/
	USART_Config();
}

 /** 
 * @brief Receive�������յ���Ϣ���ô������
 * @param[in]   *parameter  
 * @retval VOID
 */ 
static void Receive_Task(void* parameter)
{
	BaseType_t xReturn = pdTRUE;  /*������Ϣ����ֵ*/
	uint32_t r_queue;  /*������Ϣ�ı���*/
	while(1)
	{
		xReturn = xQueueReceive( Test_Queue, /*��Ϣ���о��*/
		                         &r_queue,      /*���͵�����*/
		                         portMAX_DELAY);/*�ȴ�ʱ�䣬һֱ��*/
		
		if(pdTRUE == xReturn)
			printf("���ν��յ���������%d\n",r_queue);
		else
			printf("���ν��ճ���������룺0x%lx\n",xReturn);
	}
}
 /** 
 * @brief Send ������Ϣ
 * @param[in]   *parameter  
 * @retval VOID
 */ 
static void Send_Task(void* parameter)
{
	BaseType_t xReturn = pdPASS;  /*������Ϣ����ֵ*/
	uint32_t send_data1 = 1;
	uint32_t send_data2 = 2;
	while(1)
	{
		if(ON == Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN))
		{/*key1 ������*/
			printf("������Ϣsend_data1\n");
			xReturn = xQueueSend( Test_Queue, /*��Ϣ���о��*/
			                      &send_data1, /*Ҫ���͵���Ϣ*/
			                      0);       /*�ȴ�ʱ�� ���ȴ�*/
			if(xReturn == pdPASS)
			{
				printf("��Ϣsend_data1���ͳɹ�\n\n");
			}
		}
		if(ON == Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN))
		{/*key2 ������*/
			printf("������Ϣsend_data2\n");
			xReturn = xQueueSend( Test_Queue, /*��Ϣ���о��*/
			                      &send_data2, /*Ҫ���͵���Ϣ*/
			                      0);       /*�ȴ�ʱ�� ���ȴ�*/
			if(xReturn == pdPASS)
			{
				printf("��Ϣsend_data2���ͳɹ�\n\n");
			}
		}
		vTaskDelay(20);/*��ʱ*/
	}
}
 /** 
 * �������е�����
 * @param  ��  
 * @retval ��
 */ 
static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;
	taskENTER_CRITICAL();   //�����ٽ���
	/*����Test_Queue*/
	Test_Queue = xQueueCreate( (UBaseType_t) QUEUE_LEN, /*��Ϣ���еĳ���*/
	                           (UBaseType_t) QUEUE_SIZE); /*��Ϣ���еĴ�С*/
	if(NULL != Test_Queue)
     printf("��Ϣ����Test_Queue�����ɹ�\n");
	
	/*����Receive_Task����*/
	xReturn = xTaskCreate((TaskFunction_t) Receive_Task,  //������
	                      (const char *) "Receive_Task",  //��������
												(uint16_t) 512,                 //��ջ��С
												(void *)NULL,                   //�������
												(UBaseType_t) 2,                //�������ȼ�
												(TaskHandle_t *)&Receive_Task_Handle);
	if(NULL != Receive_Task_Handle)
		printf("Receive_Task���񴴽��ɹ�\n");
	else
		printf("Receive_Task���񴴽�ʧ��\n");
	
	/*����Send_Task����*/
	xReturn = xTaskCreate((TaskFunction_t) Send_Task,  //������
	                      (const char *) "Send_Task",  //��������
												(uint16_t) 512,                 //��ջ��С
												(void *)NULL,                   //�������
												(UBaseType_t) 3,                //�������ȼ�
												(TaskHandle_t *)&Send_Task_Handle);
	if(NULL != Send_Task_Handle)
		printf("Send_Task���񴴽��ɹ�\n");
	else
		printf("Send_Task���񴴽�ʧ��\n");
	
	vTaskDelete(AppTaskCreate_Handle); /*ɾ��AppTaskCreate*/
	
	taskEXIT_CRITICAL();         //�˳��ٽ���
}
