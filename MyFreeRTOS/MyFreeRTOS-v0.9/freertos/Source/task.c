#include "FreeRTOS.h"
#include "task.h"
#include "list.h"
/*��������б�*/
List_t pxReadyTasksLists[ configMAX_PRIORITIES ];
/*��ǰ������ƿ�ָ��*/
TCB_t * volatile pxCurrentTCB = NULL;
/*Task1 Task2 IdleTaskTCB ����main.c*/
extern TCB_t Task1TCB;
extern TCB_t Task2TCB;
extern TCB_t IdleTaskTCB;
/*��������������main.c*/
extern TaskHandle_t xIdleTaskHandle;


extern TickType_t xTickCount;

/*Ϊ������������ڴ�*/
void vApplicationGetIdleTaskMemory(TCB_t **ppxIdleTaskTCBBuffer,
	                                 StackType_t **ppxIdleTaskStackBuffer,
	                                 uint32_t *pulIdleTaskStackSize  );


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
/*��������*/
static portTASK_FUNCTION( prvIdleTask, pvParameters )
{
	/* ��ֹ�������ľ��� */
	( void ) pvParameters;
    
    for(;;)
    {
        /* ����������ʱʲô������ */
    }
}

void vTaskStartScheduler(void)
{
	/****************������������ start*******************/
	TCB_t *pxIdleTaskTCBBuffer = NULL;            /*����ָ�����������ƿ�*/
	StackType_t *pxIdleTaskStackBuffer = NULL;    /*���ڿ�������ջ��ʼ��ַ*/
	uint32_t ulIdleTaskStackSize;
	
	/*��ȡ����������ڴ棺����ջ������TCB*/
	vApplicationGetIdleTaskMemory( &pxIdleTaskTCBBuffer,
	                               &pxIdleTaskStackBuffer,
	                               &ulIdleTaskStackSize );
	/*������������*/
	xIdleTaskHandle =
	xTaskCreateStatic( (TaskFunction_t)prvIdleTask,  /*�������*/
	                   (char *)"IDLE",               /*�������ƣ��ַ�����ʽ*/
										 (uint32_t)ulIdleTaskStackSize,/*����ջ��С����λΪ��*/
										 (void *)NULL,                 /*�����β�*/
										 (StackType_t *)pxIdleTaskStackBuffer,/*����ջ��ʼ��ַ*/
										 (TCB_t *)pxIdleTaskTCBBuffer );/*������ƿ�*/
	/*��������ӵ������б�*/
	vListInsertEnd(&(pxReadyTasksLists[0]),
										 &(((TCB_t *)pxIdleTaskTCBBuffer)->xStateListItem));
		
	/****************������������ end*********************/
	
  /*�ֶ�ָ����һ�����е�����*/
	pxCurrentTCB = &Task1TCB;
	/*����������*/
	if(xPortStartScheduler() != pdFALSE)
	{
		/*�����������ɹ����򲻻᷵�أ���������������*/
	}
}

#if 0
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
#endif

void vTaskSwitchContext(void)
{
	/*�����ǰ�����ǿ���������ô��ȥ����ִ������1��������2��
	  �������ǵ���ʱʱ���Ƿ����������������ʱʱ���û�е��ڣ�
	  �Ǿͷ��ؼ���ִ�п�������*/
	if(pxCurrentTCB == &IdleTaskTCB)
	{
		if(Task1TCB.xTicksToDelay == 0)
		{
			pxCurrentTCB = &Task1TCB;
		}
		else if(Task2TCB.xTicksToDelay == 0)
		{
			pxCurrentTCB = &Task2TCB;
		}else
		{
			return;     /*������ʱ��û�е����򷵻أ�����ִ�п�������*/
		}
	}
	else      /*��ǰ�����ǿ����������ִ�е�����*/
	{
		/*�����ǰ����������1��������2�Ļ������������һ������
		����������������ʱ�У����л���������
		�����ж��µ�ǰ�����Ƿ�Ӧ�ý�����ʱ״̬��
		����ǵĻ������л����������񣬷���Ͳ������κ��л�*/
		if(pxCurrentTCB == &Task1TCB)
		{
			if(Task2TCB.xTicksToDelay == 0)
			{
				pxCurrentTCB = &Task2TCB;
			}
			else if(pxCurrentTCB->xTicksToDelay != 0)
			{
				pxCurrentTCB = &IdleTaskTCB;
			}
			else
			{
				return;    /*���أ��������л�����Ϊ�������񶼴�����ʱ��*/
			}
		}
		else if(pxCurrentTCB == &Task2TCB)
		{
			if(Task1TCB.xTicksToDelay == 0)
			{
				pxCurrentTCB = &Task1TCB;
			}
			else if(pxCurrentTCB->xTicksToDelay != 0)
			{
				pxCurrentTCB = &IdleTaskTCB;
			}
			else
			{
				return;    /*���أ��������л�����Ϊ�������񶼴�����ʱ��*/
			}
		}
	}
}

void vTaskDelay(const TickType_t xTicksToDelay)
{
	TCB_t *pxTCB = NULL;
	
	/*��ȡ��ǰ�����TCB*/
	pxTCB = pxCurrentTCB;
	
	/*������ʱʱ��*/
	pxTCB->xTicksToDelay = xTicksToDelay;
	
	/*�����л�*/
	taskYIELD();
}

void xTaskIncrementTick(void)
{
	TCB_t *pxTCB = NULL;
	BaseType_t i = 0;
	
	/*����ϵͳʱ��������xTickCount,xTickCount��һ����port.c�ж����ȫ�ֱ���*/
	const TickType_t xConstTickCount = xTickCount + 1;
	xTickCount = xConstTickCount;
	
	/*ɨ������б������������xTickToDelay,�����Ϊ0�����1*/
	for(i = 0;i<configMAX_PRIORITIES; i++)
	{
		pxTCB = (TCB_t *)listGET_OWNER_OF_HEAD_ENTRY((&pxReadyTasksLists[i]));
		if(pxTCB->xTicksToDelay > 0)
		{
			pxTCB->xTicksToDelay--;
		}
	}
	/*�����л�*/
	portYIELD();
}

#endif //configSUPPORT_STATIC_ALLOCATION

