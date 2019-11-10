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

/*����uxTopReadyPriority*/
static volatile UBaseType_t uxTopReadyPriority = tskIDLE_PRIORITY;
/*ȫ�����������*/
static volatile UBaseType_t uxCurrentNumberOfTasks 	= ( UBaseType_t ) 0U;
/*����������ȼ��ľ�������ͨ�÷���*/
#if(configUSE_PORT_OPTIMISED_TASK_SELECTION == 0)
    /*uxTopReadyPriority����Ǿ��������������ȼ�*/
		#define taskRECORD_READY_PRIORITY( uxPriority)\
		{\
			if( (uxPriority) > uxTopReadyPriority )  \
			{  \
				uxTopReadyPriority = (uxPriority);   \
			}  \
		} /*taskRECORD_READY_PRIORITY*/
		
/*-----------------------------------------------------------*/
		#define taskSELECT_HIGHEST_PRIORITY_TASK() \
		{\
			UBaseType_t uxTopPriority = uxTopReadyPriority;\
			/*Ѱ�Ұ������������������ȼ��Ķ���*/\
			while( listLIST_IS_EMPTY( &( pxReadyTasksLists [ uxTopPriority ] ) ) )\
			{\
				--uxTopPriority;\
			}\
			/*��ȡ���ȼ���ߵľ��������TCB,Ȼ����µ�pxCurrentTCB*/\
			listGET_OWNER_OF_NEXT_ENTRY( pxCurrentTCB, &( pxReadyTasksLists [ uxTopPriority ] ) );\
		  /*����uxTopReadyPriority*/\
			uxTopReadyPriority = uxTopPriority; \
		}/*taskSELECT_HIGHEST_PRIORITY_TASK()*/
/*-----------------------------------------------------------*/
/*�������궨��ֻ����ѡ���Ż�����ʱ���ã����ﶨ��Ϊ��*/
#define taskRESET_READY_PRIORITY( uxPriority )
#define portRESET_READY_PRIORITY( uxPriority, uxTopReadyPriority )

/*����������ȼ��ľ������񣺸��ݴ������ܹ��Ż���ķ���*/		
#else /*configUSE_PORT_OPTIMISED_TASK_SELECTION*/
    #define taskRECORD_READY_PRIORITY( uxPriority ) \
		        portRECORD_READY_PRIORITY( uxPriority, uxTopReadyPriority )

/*-----------------------------------------------------------*/
		
		#define taskSELECT_HIGHEST_PRIORITY_TASK() \
		{\
			UBaseType_t uxTopPriority;\
			/*Ѱ��������ȼ�*/   \
			portGET_HIGHEST_PRIORITY( uxTopPriority, uxTopReadyPriority );  \
			/*��ȡ���ȼ���ߵľ��������TCB,Ȼ����µ�pxCurrentTCB*/  \
			listGET_OWNER_OF_NEXT_ENTRY(pxCurrentTCB, &(pxReadyTasksLists [ uxTopPriority ])); \
		} /*taskSELECT_HIGHEST_PRIORITY_TASK()*/
/*----------------------------------------------------------*/
		#if 0
		    #define taskRESET_READY_PRIORITY( uxPriority)\
		    {\
					if(listCURRENT_LIST_LENGTH(&(pxReadyTasksLists[(uxPriority)])) == (UBaseType_t)0)\
				  {\
						portRESET_READY_PRIORITY((uxPriority),(uxTopReadyPriority));\
					}\
				}
		#else
				#define taskRESET_READY_PRIORITY(uxPriority)\
				{\
					portRESET_READY_PRIORITY((uxPriority),(uxTopReadyPriority));\
				}
		
		#endif
#endif  /*configUSE_PORT_OPTIMISED_TASK_SELECTION*/
				
/*���������ӵ������б�*/
#define prvAddTaskToReadyList(pxTCB)\
          taskRECORD_READY_PRIORITY( (pxTCB)->uxPriority );\
          vListInsertEnd( &(pxReadyTasksLists[(pxTCB)->uxPriority]),\
                          &( (pxTCB)->xStateListItem ) );				
				

/*Ϊ������������ڴ�*/
void vApplicationGetIdleTaskMemory(TCB_t **ppxIdleTaskTCBBuffer,
	                                 StackType_t **ppxIdleTaskStackBuffer,
	                                 uint32_t *pulIdleTaskStackSize  );


static void prvInitialiseNewTask( TaskFunction_t pxTaskCode,
	                                const char * const pcName,
																	const uint32_t ulStackDepth,
																	void * const pvParameters,
																	/*�������ȼ�����ֵԽ�����ȼ�Խ��*/
																  UBaseType_t uxPriority,
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
	
	/*��ʼ�����ȼ�*/
	if(uxPriority >= (UBaseType_t) configMAX_PRIORITIES )
	{
		uxPriority = (UBaseType_t) configMAX_PRIORITIES - (UBaseType_t) 1U;
	}
	pxNewTCB->uxPriority = uxPriority;
	
	/*��ʼ������ջ*/
	pxNewTCB->pxTopOfStack = pxPortInitialiseStack(pxTopOfStack,pxTaskCode,pvParameters);
	
	/*��������ָ��������ƿ�*/
	if((void *)pxCreatedTask != NULL)
	{
		*pxCreatedTask = (TaskHandle_t)pxNewTCB;
	}
}


/*��ʼ��������ص��б�*/
void prvInitialiseTaskLists(void)
{
	UBaseType_t uxPriority;
	for(uxPriority = (UBaseType_t)0U; uxPriority < (UBaseType_t)configMAX_PRIORITIES ;uxPriority++ )
	{
		vListInitialise(&(pxReadyTasksLists[uxPriority]));
	}
}


static void prvAddNewTaskToReadyList(TCB_t *pxNewTCB)
{
	/*�����ٽ��*/
	taskENTER_CRITICAL();
	{
		/*ȫ�������������һ*/
		uxCurrentNumberOfTasks++;
		/*���pxCurrentTCBΪ�գ���pxCurrentTCBָ���´���������*/
		if( pxCurrentTCB == NULL )
		{
			pxCurrentTCB = pxNewTCB;
			
			/*����ǵ�һ�δ�����������Ҫ��ʼ��������ص��б�*/
			if(uxCurrentNumberOfTasks == (UBaseType_t) 1 )
			{
				/*��ʼ��������ص��б�*/
				prvInitialiseTaskLists();
			}
		}
		else /*���pxCurrentTCB��Ϊ�գ�
			     �������������ȼ���pxCurrentTCBָ��������ȼ������TCB*/
		{
			if(pxCurrentTCB->uxPriority <= pxNewTCB->uxPriority)
			{
				pxCurrentTCB = pxNewTCB;
			}
		}
		/*���������ӵ������б�*/
		prvAddTaskToReadyList(pxNewTCB);
	}
	/*�˳��ٽ��*/
	taskEXIT_CRITICAL();
}

#if(configSUPPORT_STATIC_ALLOCATION == 1)
TaskHandle_t xTaskCreateStatic( TaskFunction_t pxTaskCode,
                                const char * const pcName,
															  const uint32_t ulStackDepth,
																void * const pvParameters,
																/*�������ȼ�����ֵԽ�����ȼ�Խ��*/
																UBaseType_t uxPriority,
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
		                      uxPriority,   /*�������ȼ�*/
		                      &xReturn,     /*������*/
		                      pxNewTCB);
		/*���������ӵ������б�*/
		prvAddNewTaskToReadyList(pxNewTCB);
	}else
	{
		xReturn = NULL;
	}
	
	/*������������������񴴽��ɹ�����ʱxReturnӦ��ָ��������ƿ�*/
	return xReturn;
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
/*�������������*/
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
											 /*�������ȼ���ֵԽ�����ȼ�Խ��*/
										 (UBaseType_t)tskIDLE_PRIORITY,
										 (StackType_t *)pxIdleTaskStackBuffer,/*����ջ��ʼ��ַ*/
										 (TCB_t *)pxIdleTaskTCBBuffer );/*������ƿ�*/
	/*���������ӵ������б�*/
//	vListInsertEnd(&(pxReadyTasksLists[0]),
//										 &(((TCB_t *)pxIdleTaskTCBBuffer)->xStateListItem));
		
	/****************������������ end*********************/
	
  /*�ֶ�ָ����һ�����е�����*/
//	pxCurrentTCB = &Task1TCB;
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


#if 1
void vTaskSwitchContext(void)
{
	/*��ȡ���ȼ���ߵľ��������TCB��Ȼ����µ�pxCurrentTCB*/
	taskSELECT_HIGHEST_PRIORITY_TASK();
}
#else
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
#endif


void vTaskDelay(const TickType_t xTicksToDelay)
{
	TCB_t *pxTCB = NULL;
	
	/*��ȡ��ǰ�����TCB*/
	pxTCB = pxCurrentTCB;
	
	/*������ʱʱ��*/
	pxTCB->xTicksToDelay = xTicksToDelay;
	
	/*������Ӿ����б��Ƴ�*/
	//uxListRemove(&(pxTCB->xStateListItem));//���ڲ���
	taskRESET_READY_PRIORITY(pxTCB->uxPriority);
	
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
			
			/*��ʱʱ�䵽�����������*/
			if(pxTCB->xTicksToDelay == 0)
			{
				taskRECORD_READY_PRIORITY(pxTCB->uxPriority);
			}
		}
	}
	/*�����л�*/
	portYIELD();
}

#endif //configSUPPORT_STATIC_ALLOCATION
