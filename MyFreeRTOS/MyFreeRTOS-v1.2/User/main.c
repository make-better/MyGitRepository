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
portBASE_TYPE flag3 = 0;
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


/*task3*/
TaskHandle_t Task3_Handle;
#define TASK3_STACK_SIZE   128
StackType_t Task3Stack[TASK3_STACK_SIZE];
TCB_t Task3TCB;

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
		delay(100);
		flag1 = 0;
		delay(100);
	}

}
/*����2*/
void Task2_Entry(void *p_arg)
{
	for(;;)
	{
		flag2 = 1;
		delay(100);
		flag2 = 0;
		delay(100);
	}
}

/*����3*/
void Task3_Entry(void *p_arg)
{
	for(;;)
	{
		flag3 = 1;
		vTaskDelay(1);
		flag3 = 0;
		vTaskDelay(1);
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
																		(UBaseType_t)2,               /*�������ȼ�*/
																		(StackType_t *)Task1Stack,    /*����ջ��ʼ��ַ*/
	                                  (TCB_t *)&Task1TCB);          /*������ƿ�*/

	Task2_Handle =                                                  /*������*/
	xTaskCreateStatic((TaskFunction_t)Task2_Entry,                  /*�������*/
	                                  (char *)"Task2",              /*�������ƣ��ַ�����ʽ*/
																		(uint32_t)TASK2_STACK_SIZE,  /*����ջ��С����λΪ��*/
																		(void *)NULL,                 /*�����β�*/
																		(UBaseType_t)2,               /*�������ȼ�*/
																		(StackType_t *)Task2Stack,    /*����ջ��ʼ��ַ*/
	                                  (TCB_t *)&Task2TCB);          /*������ƿ�*/

	Task3_Handle =                                                  /*������*/
	xTaskCreateStatic((TaskFunction_t)Task3_Entry,                  /*�������*/
	                                  (char *)"Task3",              /*�������ƣ��ַ�����ʽ*/
																		(uint32_t)TASK3_STACK_SIZE,  /*����ջ��С����λΪ��*/
																		(void *)NULL,                 /*�����β�*/
																		(UBaseType_t)3,               /*�������ȼ�*/
																		(StackType_t *)Task3Stack,    /*����ջ��ʼ��ַ*/
	                                  (TCB_t *)&Task3TCB);          /*������ƿ�*/
	/* ������������ǰ���ر��ж� */                                  
    portDISABLE_INTERRUPTS();
 /*��������������ʼ��������ȣ������ɹ��򲻷���*/
 vTaskStartScheduler();

 for(;;)
 {
	 //
 }
}


