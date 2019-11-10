/*
*****************************************************
*                ������ͷ�ļ�
*****************************************************
*/
#include "FreeRTOS.h"
#include "list.h"
#include "task.h"
/*
*****************************************************
*                  ȫ�ֱ���
*****************************************************
*/
/*���������*/
extern List_t pxReadyTasksLists[ configMAX_PRIORITIES ];

portBASE_TYPE flag1 = 0;
portBASE_TYPE flag2 = 0;
/*
*****************************************************
*                  ������ƿ� & STACK
*****************************************************
*/
/*��������ջ*/
#define TASK1_STACK_SIZE       128
StackType_t Task1Stack[TASK1_STACK_SIZE];

#define TASK2_STACK_SIZE       128
StackType_t Task2Stack[TASK2_STACK_SIZE];

/*������������ջ*/
#define configMINIMAL_STACK_SIZE             ((unsigned short)128)
StackType_t IdleTaskStack[configMINIMAL_STACK_SIZE];

/*����������ƿ�*/
TCB_t Task1TCB;
TCB_t Task2TCB;

/*������������������ƿ�*/
TCB_t IdleTaskTCB;


/*������*/
TaskHandle_t Task1_Handle;
TaskHandle_t Task2_Handle;
TaskHandle_t xIdleTaskHandle;
/*Ϊ������������ڴ�*/
void vApplicationGetIdleTaskMemory(TCB_t **ppxIdleTaskTCBBuffer,
	                                 StackType_t **ppxIdleTaskStackBuffer,
	                                 uint32_t *pulIdleTaskStackSize  )
{
	*ppxIdleTaskTCBBuffer = &IdleTaskTCB;
	*ppxIdleTaskStackBuffer = IdleTaskStack;
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/*�����ʱ*/
void delay(uint32_t count)
{
	for(;count!=0;count--);
}
/*����1*/
void Task1_Entry(void *p_arg)
{

	for(;;)
	{
    flag1 = 1;
		vTaskDelay(2);
		flag1 = 0;
		vTaskDelay(2);
	}

}
/*����2*/
void Task2_Entry(void *p_arg)
{
	for(;;)
	{
		flag2 = 1;
		vTaskDelay(2);
		flag2 = 0;
		vTaskDelay(2);
	}
}
/*
******************************************************
*										main����
******************************************************
*/
int main(void)
{
	/*��ʼ��������ص��б�������б�*/
	prvInitialiseTaskLists();
	
	Task1_Handle =                                                  /*������*/
	xTaskCreateStatic((TaskFunction_t)Task1_Entry,                  /*�������*/
	                                  (char *)"Task1",              /*�������ƣ��ַ�����ʽ*/
																		(uint32_t)TASK1_STACK_SIZE,  /*����ջ��С����λΪ��*/
																		(void *)NULL,                 /*�����β�*/
																		(UBaseType_t)1,               /*�������ȼ�*/
																		(StackType_t *)Task1Stack,    /*����ջ��ʼ��ַ*/
	                                  (TCB_t *)&Task1TCB);          /*������ƿ�*/
	/*��������ӵ������б�*/
//	vListInsertEnd(&(pxReadyTasksLists[1]),
//								 &(((TCB_t *)(&Task1TCB))->xStateListItem));
	Task2_Handle =                                                  /*������*/
	xTaskCreateStatic((TaskFunction_t)Task2_Entry,                  /*�������*/
	                                  (char *)"Task2",              /*�������ƣ��ַ�����ʽ*/
																		(uint32_t)TASK2_STACK_SIZE,  /*����ջ��С����λΪ��*/
																		(void *)NULL,                 /*�����β�*/
																		(UBaseType_t)2,               /*�������ȼ�*/
																		(StackType_t *)Task2Stack,    /*����ջ��ʼ��ַ*/
	                                  (TCB_t *)&Task2TCB);          /*������ƿ�*/
 /*��������ӵ������б�*/
// vListInsertEnd(&(pxReadyTasksLists[2]),
//								 &(((TCB_t *)(&Task2TCB))->xStateListItem));
	/* ������������ǰ���ر��ж� */                                  
    portDISABLE_INTERRUPTS();
 /*��������������ʼ��������ȣ������ɹ��򲻷���*/
 vTaskStartScheduler();

 for(;;)
 {
	 //
 }
}


