
/*
**********************************************************************************************
*																			ͷ�ļ�
**********************************************************************************************
*/
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_led.h"

/*
**********************************************************************************************
*																			������
**********************************************************************************************
*/
 /* ���������� */
static TaskHandle_t AppTaskCreate_Handle;
/* LED������ */
static TaskHandle_t LED_Task_Handle;	

/*
**********************************************************************************************
*																			����
**********************************************************************************************
*/
/*�������������ջ*/
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];
/*��ʱ�����������ջ*/
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];
/*AppTaskCreate �����ջ*/
static StackType_t AppTaskCreate_Stack[128];
/*LED�����ջ*/
static StackType_t LED_Task_Stack[128];

/*����������ƿ�*/
static StaticTask_t Idle_Task_TCB;
/*��ʱ��������ƿ�*/
static StaticTask_t Timer_Task_TCB;
/*AppTaskCreate ������ƿ�*/
static StaticTask_t AppTaskCreate_TCB;
/*LED ������ƿ�*/
static StaticTask_t LED_Task_TCB;
/*
**********************************************************************************************
*																			��������
**********************************************************************************************
*/

static void BSP_Init(void);

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
	                                 StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize);
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
	                                 StackType_t **ppxTimerTaskStackBuffer,
                                   uint32_t *pulTimerTaskStackSize);
static void AppTaskCreate(void);
/*
**********************************************************************************************
*																			main����
**********************************************************************************************
*/
int main(void)
{
	/*������Ӳ����ʼ��*/
	BSP_Init();
	/*����AppTaskCreate����*/
	AppTaskCreate_Handle = xTaskCreateStatic((TaskFunction_t) AppTaskCreate,  //������
	                                          (const char *) "AppTaskCreate",  //��������
																						(uint32_t) 128,                 //��ջ��С
																						(void *)NULL,                   //�������
																						(UBaseType_t) 3,                //�������ȼ�
																						(StackType_t *)AppTaskCreate_Stack,//�����ջ
	                                          (StaticTask_t *)&AppTaskCreate_TCB);//������ƿ�
	if(NULL != AppTaskCreate_Handle)
	{
		vTaskStartScheduler();//��������
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
	
	LED_G(ON);
	
	/*���ڳ�ʼ��*/
	USART_Config();
}

 /** 
 * ʹLED��500ms�ļ����˸
 * @param[in]   *parameter  
 * @retval VOID
 */ 
static void LED_Task(void* parameter)
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
 * ��ȡ��������������ջ��������ƿ��ڴ� 
 * @param[in]   ppxIdleTaskTCBBuffer   : ������ƿ��ڴ�
 *              ppxIdleTaskStackBuffer : �����ջ�ڴ�
 *              pulIdleTaskStackSize   : �����ջ��С   
 * @retval void
 */ 
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
	                                 StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
	*ppxIdleTaskTCBBuffer = &Idle_Task_TCB; /*������ƿ��ڴ�*/
	*ppxIdleTaskStackBuffer = Idle_Task_Stack; /*�����ջ�ڴ�*/
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;/*�����ջ��С*/
}
 /** 
 * ��ȡ��ʱ������������ջ��������ƿ��ڴ� 
 * @param[in]   ppxTimerTaskTCBBuffer   : ������ƿ��ڴ�
 *              ppxTimerTaskStackBuffer : �����ջ�ڴ�
 *              pulTimerTaskStackSize   : �����ջ��С   
 * @retval void
 */ 
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
	                                 StackType_t **ppxTimerTaskStackBuffer,
                                   uint32_t *pulTimerTaskStackSize)
{
	*ppxTimerTaskTCBBuffer = &Timer_Task_TCB; /*������ƿ��ڴ�*/
	*ppxTimerTaskStackBuffer = Timer_Task_Stack; /*�����ջ�ڴ�*/
	*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;/*�����ջ��С*/
}
 /** 
 * �������е�����
 * @param  ��  
 * @retval ��
 */ 
static void AppTaskCreate(void)
{
	taskENTER_CRITICAL();   //�����ٽ���
	/*����LED_Task����*/
	LED_Task_Handle = xTaskCreateStatic((TaskFunction_t) LED_Task,  //������
	                                          (const char *) "LED_Task",  //��������
																						(uint32_t) 128,                 //��ջ��С
																						(void *)NULL,                   //�������
																						(UBaseType_t) 4,                //�������ȼ�
																						(StackType_t *)LED_Task_Stack,//�����ջ
	                                          (StaticTask_t *)&LED_Task_TCB);
	if(NULL != LED_Task_Handle)
		printf("LED_Task���񴴽��ɹ�\n");
	else
		printf("LED_Task���񴴽�ʧ��\n");
	
	vTaskDelete(AppTaskCreate_Handle); /*ɾ��AppTaskCreate*/
	
	taskEXIT_CRITICAL();         //�˳��ٽ���
}
