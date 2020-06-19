
/*
**********************************************************************************************
*																			ͷ�ļ�
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
*																			������
**********************************************************************************************
*/
static TaskHandle_t AppTaskCreate_Handle = NULL;
static TaskHandle_t LED_Task_Handle = NULL;
static TaskHandle_t KEY_Task_Handle = NULL;
/*
**********************************************************************************************
*																			�ں˶�����
**********************************************************************************************
*/
static EventGroupHandle_t Event_Handle = NULL;

/*
**********************************************************************************************
*																			ȫ�ֱ�������
**********************************************************************************************
*/

/*
**********************************************************************************************
*																			�궨��
**********************************************************************************************
*/
#define KEY1_EVENT		(0x01<<0)//�¼������0λ
#define KEY2_EVENT		(0x01<<1)//�¼������1λ
/*
**********************************************************************************************
*																			��������
**********************************************************************************************
*/
static void AppTaskCreate(void);
static void LED_Task(void* pvParameters);
static void KEY_Task(void* pvParameters);
static void BSP_Init(void);
/*
**********************************************************************************************
*																			main����
**********************************************************************************************
*/
int main(void)
{
	BaseType_t xReturn = pdPASS;
	//�������ʼ��
	BSP_Init();
	printf("FreeRTOS �¼� ���Գ���\n");
	xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate, //�������
												(const char*)"AppTaskCreate",	//��������
												(uint16_t)512,						//��ջ��С
												(void *)NULL,							//�������
												(UBaseType_t)1,						//�������ȼ�
												(TaskHandle_t *)&AppTaskCreate_Handle);//������ƿ�
	if(pdPASS == xReturn)
	{
		vTaskStartScheduler();//�������񣬿�������
	}
	else
	{
		printf("��������ʧ��\n");
	}
		
	while(1)
	{

	}

}
/**
**************************************************
* @name : AppTaskCreate
* @brief : ������������
* @para : no
* @return : no
**************************************************
*/
static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;
	taskENTER_CRITICAL(); //�����ٽ���
	
	/*���� Event_Handle*/
	Event_Handle = xEventGroupCreate();
	if(NULL != Event_Handle)
		printf("Event_Handle�¼������ɹ�\r\n");
	
	/*����LED_Task����*/
	xReturn = xTaskCreate((TaskFunction_t)LED_Task, //�������
												(const char*)"LED_Task",	//��������
												(uint16_t)512,						//��ջ��С
												(void *)NULL,							//�������
												(UBaseType_t)2,						//�������ȼ�
												(TaskHandle_t *)&LED_Task_Handle);//������ƿ�
	if(pdPASS == xReturn)
	{
		printf("����LED_Task����ɹ�\r\n");
	}
	
	/*����KEY_Task����*/
	xReturn = xTaskCreate((TaskFunction_t)KEY_Task, //�������
												(const char*)"KEY_Task",	//��������
												(uint16_t)512,						//��ջ��С
												(void *)NULL,							//�������
												(UBaseType_t)3,						//�������ȼ�
												(TaskHandle_t *)&KEY_Task_Handle);//������ƿ�
	if(pdPASS == xReturn)
	{
		printf("����KEY_Task����ɹ�\r\n");
	}
	
	vTaskDelete(AppTaskCreate_Handle);//ɾ��
	
	taskEXIT_CRITICAL();//�˳��ٽ���
}
/**
**************************************************
* @name : LED_Task
* @brief : LED_Task��������
* @para : no
* @return : no
**************************************************
*/
static void LED_Task(void* pvParameters)
{
	EventBits_t r_event; //�¼����ձ���
	while(1)
	{
		r_event = xEventGroupWaitBits(Event_Handle, //�¼�������
													KEY1_EVENT|KEY2_EVENT,//����Ȥ���¼�
													pdTRUE,			//�˳�ʱ����¼�λ
													pdTRUE,			//�������Ȥ�������¼�
													portMAX_DELAY);//��ʱʱ�� һֱ��
		if((r_event & (KEY1_EVENT|KEY2_EVENT)) == (KEY1_EVENT|KEY2_EVENT))
		{//������ɲ�����ȷ
			printf("KEY1 KEY2������\n");
			LED_G_Toggle;
		}
		else
		{
			printf("�¼�����\n");
		}
	}
}

/**
**************************************************
* @name : KEY_Task
* @brief : KEY_Task��
* @para : no
* @return : no
**************************************************
*/
static void KEY_Task(void* pvParameters)
{
	while(1)
	{
		//���KEY1������
		if(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON)
		{
			printf("key1������\n");
			//�����¼�1
			xEventGroupSetBits(Event_Handle,KEY1_EVENT);
		}
		//���KEY2������
		if(Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON)
		{
			printf("key2������\n");
			//�����¼�2
			xEventGroupSetBits(Event_Handle,KEY2_EVENT);
		}
		vTaskDelay(20);
	}
}

/**
**************************************************
* @name : BSP_Init
* @brief : ��ʼ������
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