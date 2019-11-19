
/*
**********************************************************************************************
*																			ͷ�ļ�
**********************************************************************************************
*/
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_led.h"
#include "bsp_key.h"
/*
**********************************************************************************************
*																			������
**********************************************************************************************
*/
 /* ���������� */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* LED1������ */
static TaskHandle_t LED1_Task_Handle = NULL;	
/* ����ɨ�������� */
static TaskHandle_t KEY_Task_Handle = NULL;	

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
static void LED1_Task(void* parameter);
static void KEY_Task(void* parameter);
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
 * �弶��ʼ������
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
 * ʹLED��500ms�ļ����˸
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
 * k1 k2 ����ɨ��
 * @param[in]   *parameter  
 * @retval VOID
 */ 
static void KEY_Task(void* parameter)
{
	while(1)
	{
		if(ON == Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN))
		{/*key1 ������*/
			printf("����LED1����\n");
			vTaskSuspend(LED1_Task_Handle);/*��������LED1*/
		}
		if(ON == Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN))
		{/*key2 ������*/
			printf("�ָ�LED1����\n");
			vTaskResume(LED1_Task_Handle);/*��������LED1*/
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
	/*����LED2_Task����*/
	xReturn = xTaskCreate((TaskFunction_t) LED1_Task,  //������
	                      (const char *) "LED1_Task",  //��������
												(uint16_t) 512,                 //��ջ��С
												(void *)NULL,                   //�������
												(UBaseType_t) 2,                //�������ȼ�
												(TaskHandle_t *)&LED1_Task_Handle);
	if(NULL != LED1_Task_Handle)
		printf("LED1_Task���񴴽��ɹ�\n");
	else
		printf("LED1_Task���񴴽�ʧ��\n");
	
	/*����LED2_Task����*/
	xReturn = xTaskCreate((TaskFunction_t) KEY_Task,  //������
	                      (const char *) "KEY_Task",  //��������
												(uint16_t) 512,                 //��ջ��С
												(void *)NULL,                   //�������
												(UBaseType_t) 1,                //�������ȼ�
												(TaskHandle_t *)&KEY_Task_Handle);
	if(NULL != KEY_Task_Handle)
		printf("KEY_Task���񴴽��ɹ�\n");
	else
		printf("KEY_Task���񴴽�ʧ��\n");
	
	vTaskDelete(AppTaskCreate_Handle); /*ɾ��AppTaskCreate*/
	
	taskEXIT_CRITICAL();         //�˳��ٽ���
}
