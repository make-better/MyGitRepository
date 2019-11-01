#include "FreeRTOS.h"
#include "task.h"
/*��������б�*/
List_t pxReadyTasksLists[ configMAX_PRIORITIES ];
/*��ǰ������ƿ�ָ��*/
TCB_t * volatile pxCurrentTCB = NULL;
/*Task1 Task2 ����main.c*/
extern TCB_t Task1TCB;
extern TCB_t Task2TCB;

static void prvInitialiseNewTask( TaskFunction_t pxTaskCode,
	                                const char * const pcName,
																	const uint32_t ulStackDepth,
																	void * const pvParameters,
																	TaskHandle_t * const pxCreatedTask,
																	TCB_t *pxNewTCB
	                                )
{
	StackType_t *pxTopOfStack;
	UBaseType_t x;
	
	/*��ȡջ����ַ*/
	pxTopOfStack = pxNewTCB->pxStack + ( ulStackDepth - (uint32_t) 1 );
	/*������8�ֽڶ���*/ /*��յ���λ,��֤һ��ֻ������8���ֽڻ�8�ı���*/
	pxTopOfStack = (StackType_t *)(((uint32_t)pxTopOfStack)&(~((uint32_t)0x0007)));
	
	/*����������ִ洢��TCB��*/
	for(x = (UBaseType_t) 0;x<(UBaseType_t)configMAX_TASK_NAME_LEN;x++)
	{
		pxNewTCB->pcTaskName[x] = pcName[x];
		if(pcName[x] == 0x00)
		{
			break;
		}
	}
	/*�������ֵĳ��Ȳ��ܳ���configMAX_TASK_NAME_LEN*/
	pxNewTCB->pcTaskName[configMAX_TASK_NAME_LEN-1] = '\0';
	/*��ʼ��TCB�е�xStateListItem�ڵ�,��ֵΪNULL*/
	vListInitialiseItem(&(pxNewTCB->xStateListItem));
	/*����xStateListItem�ڵ��ӵ����*/
	listSET_LIST_ITEM_OWNER( &(pxNewTCB->xStateListItem),pxNewTCB);
	/*��ʼ������ջ*/
	pxNewTCB->pxTopOfStack = pxPortInitialiseStack(pxTopOfStack,pxTaskCode,pvParameters);
	
	/*��������ָ��������ƿ�*/
	if((void *)pxCreatedTask != NULL)
	{
		*pxCreatedTask = (TaskHandle_t)pxNewTCB;
	}
}
#if(configSUPPORT_STATIC_ALLOCATION == 1)
TaskHandle_t xTaskCreateStatic( TaskFunction_t pxTaskCode,
                                const char * const pcName,
															  const uint32_t ulStackDepth,
																void * const pvParameters,
																StackType_t * const puxStackBuffer,
                                TCB_t * const pxTaskBuffer )
{
	TCB_t *pxNewTCB;
  TaskHandle_t xReturn;	
	
	if( ( pxTaskBuffer != NULL ) && ( puxStackBuffer != NULL ) )
	{
		pxNewTCB = (TCB_t * )pxTaskBuffer;
		pxNewTCB->pxStack = ( StackType_t * ) puxStackBuffer;
		/*�����µ�����*/
		prvInitialiseNewTask( pxTaskCode,   /*�������*/
		                      pcName,       /*�������ƣ��ַ�����ʽ*/
		                      ulStackDepth, /*����ջ��С����λΪ��*/
		                      pvParameters, /*�����β�*/
		                      &xReturn,     /*������*/
		                      pxNewTCB);
	}else
	{
		xReturn = NULL;
	}
	
	/*������������������񴴽��ɹ�����ʱxReturnӦ��ָ��������ƿ�*/
	return xReturn;
}
/*�����б��ʼ��*/
void prvInitialiseTaskLists(void)
{
	UBaseType_t uxPriority;
	
	for( uxPriority = (UBaseType_t) 0U;
	     uxPriority < (UBaseType_t) configMAX_PRIORITIES;
	     uxPriority++ )
	{
		vListInitialise(&(pxReadyTasksLists[ uxPriority ]));
	}
}

void vTaskStartScheduler(void)
{
	/*�ֶ�ָ����һ�����е�����*/
	pxCurrentTCB = &Task1TCB;
	/*����������*/
	if(xPortStartScheduler() != pdFALSE)
	{
		/*�����������ɹ����򲻻᷵�أ���������������*/
	}
}

void vTaskSwitchContext(void)
{
	/*�������������л�*/
	if(pxCurrentTCB == &Task1TCB)
	{
		pxCurrentTCB = &Task2TCB;
	}else
	{
		pxCurrentTCB = &Task1TCB;
	}
}

#endif //configSUPPORT_STATIC_ALLOCATION



















