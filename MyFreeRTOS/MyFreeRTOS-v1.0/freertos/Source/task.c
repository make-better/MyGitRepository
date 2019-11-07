#include "FreeRTOS.h"
#include "task.h"
#include "list.h"
/*任务就绪列表*/
List_t pxReadyTasksLists[ configMAX_PRIORITIES ];
/*当前任务控制块指针*/
TCB_t * volatile pxCurrentTCB = NULL;
/*Task1 Task2 IdleTaskTCB 来自main.c*/
extern TCB_t Task1TCB;
extern TCB_t Task2TCB;
extern TCB_t IdleTaskTCB;
/*空闲任务句柄来自main.c*/
extern TaskHandle_t xIdleTaskHandle;

extern TickType_t xTickCount;

/*定义uxTopReadyPriority*/
static volatile UBaseType_t uxTopReadyPriority = tskIDLE_PRIORITY;
/*全局任务计数器*/
static volatile UBaseType_t uxCurrentNumberOfTasks 	= ( UBaseType_t ) 0U;
/*查找最高优先级的就绪任务：通用方法*/
#if(configUSE_PORT_OPTIMISED_TASK_SELECTION == 0)
    /*uxTopReadyPriority存的是就绪任务的最高优先级*/
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
			/*寻找包含就绪任务的最高优先级的队列*/\
			while( listLIST_IS_EMPTY( &( pxReadyTasksLists [ uxTopPriority ] ) ) )\
			{\
				--uxTopPriority;\
			}\
			/*获取优先级最高的就绪任务的TCB,然后更新到pxCurrentTCB*/\
			listGET_OWNER_OF_NEXT_ENTRY( pxCurrentTCB, &( pxReadyTasksLists [ uxTopPriority ] ) );\
		  /*更新uxTopReadyPriority*/\
			uxTopReadyPriority = uxTopPriority; \
		}/*taskSELECT_HIGHEST_PRIORITY_TASK()*/
/*-----------------------------------------------------------*/
/*这两个宏定义只有在选择优化方法时才用，这里定义为空*/
#define taskRESET_READY_PRIORITY( uxPriority )
#define portRESET_READY_PRIORITY( uxPriority, uxTopReadyPriority )

/*查找最高优先级的就绪任务：根据处理器架构优化后的方法*/		
#else /*configUSE_PORT_OPTIMISED_TASK_SELECTION*/
    #define taskRECORD_READY_PRIORITY( uxPriority ) \
		        portRECORD_READY_PRIORITY( uxPriority, uxTopReadyPriority )

/*-----------------------------------------------------------*/
		
		#define taskSELECT_HIGHEST_PRIORITY_TASK() \
		{\
			UBaseType_t uxTopPriority;\
			/*寻找最高优先级*/   \
			portGET_HIGHEST_PRIORITY( uxTopPriority, uxTopReadyPriority );  \
			/*获取优先级最高的就绪任务的TCB,然后更新到pxCurrentTCB*/  \
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
				
/*将任务添加到就序列表*/
#define prvAddTaskToReadyList(pxTCB)\
          taskRECORD_READY_PRIORITY( (pxTCB)->uxPriority );\
          vListInsertEnd( &(pxReadyTasksLists[(pxTCB)->uxPriority]),\
                          &( (pxTCB)->xStateListItem ) );				
				

/*为空闲任务分配内存*/
void vApplicationGetIdleTaskMemory(TCB_t **ppxIdleTaskTCBBuffer,
	                                 StackType_t **ppxIdleTaskStackBuffer,
	                                 uint32_t *pulIdleTaskStackSize  );


static void prvInitialiseNewTask( TaskFunction_t pxTaskCode,
	                                const char * const pcName,
																	const uint32_t ulStackDepth,
																	void * const pvParameters,
																	/*任务优先级，数值越大，优先级越高*/
																  UBaseType_t uxPriority,
																	TaskHandle_t * const pxCreatedTask,
																	TCB_t *pxNewTCB
	                                )
{
	StackType_t *pxTopOfStack;
	UBaseType_t x;
	
	/*获取栈顶地址*/
	pxTopOfStack = pxNewTCB->pxStack + ( ulStackDepth - (uint32_t) 1 );
	/*向下做8字节对齐*/ /*清空低三位,保证一次只能提升8个字节或8的倍数*/
	pxTopOfStack = (StackType_t *)(((uint32_t)pxTopOfStack)&(~((uint32_t)0x0007)));
	
	/*将任务的名字存储在TCB中*/
	for(x = (UBaseType_t) 0;x<(UBaseType_t)configMAX_TASK_NAME_LEN;x++)
	{
		pxNewTCB->pcTaskName[x] = pcName[x];
		if(pcName[x] == 0x00)
		{
			break;
		}
	}
	/*任务名字的长度不能超过configMAX_TASK_NAME_LEN*/
	pxNewTCB->pcTaskName[configMAX_TASK_NAME_LEN-1] = '\0';
	/*初始化TCB中的xStateListItem节点,赋值为NULL*/
	vListInitialiseItem(&(pxNewTCB->xStateListItem));
	/*设置xStateListItem节点的拥有者*/
	listSET_LIST_ITEM_OWNER( &(pxNewTCB->xStateListItem),pxNewTCB);
	
	/*初始化优先级*/
	if(uxPriority >= (UBaseType_t) configMAX_PRIORITIES )
	{
		uxPriority = (UBaseType_t) configMAX_PRIORITIES - (UBaseType_t) 1U;
	}
	pxNewTCB->uxPriority = uxPriority;
	
	/*初始化任务栈*/
	pxNewTCB->pxTopOfStack = pxPortInitialiseStack(pxTopOfStack,pxTaskCode,pvParameters);
	
	/*让任务句柄指向任务控制块*/
	if((void *)pxCreatedTask != NULL)
	{
		*pxCreatedTask = (TaskHandle_t)pxNewTCB;
	}
}


/*初始化任务相关的列表*/
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
	/*进入临界段*/
	taskENTER_CRITICAL();
	{
		/*全局任务计数器加一*/
		uxCurrentNumberOfTasks++;
		/*如果pxCurrentTCB为空，则将pxCurrentTCB指向新创建的任务*/
		if( pxCurrentTCB == NULL )
		{
			pxCurrentTCB = pxNewTCB;
			
			/*如果是第一次创建任务，则需要初始化任务相关的列表*/
			if(uxCurrentNumberOfTasks == (UBaseType_t) 1 )
			{
				/*初始化任务相关的列表*/
				prvInitialiseTaskLists();
			}
		}
		else /*如果pxCurrentTCB不为空，
			     则根据任务的优先级将pxCurrentTCB指向最高优先级任务的TCB*/
		{
			if(pxCurrentTCB->uxPriority <= pxNewTCB->uxPriority)
			{
				pxCurrentTCB = pxNewTCB;
			}
		}
		/*将任务添加到就序列表*/
		prvAddTaskToReadyList(pxNewTCB);
	}
	/*退出临界段*/
	taskEXIT_CRITICAL();
}

#if(configSUPPORT_STATIC_ALLOCATION == 1)
TaskHandle_t xTaskCreateStatic( TaskFunction_t pxTaskCode,
                                const char * const pcName,
															  const uint32_t ulStackDepth,
																void * const pvParameters,
																/*任务优先级，数值越大，优先级越高*/
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
		/*创建新的任务*/
		prvInitialiseNewTask( pxTaskCode,   /*任务入口*/
		                      pcName,       /*任务名称，字符串形式*/
		                      ulStackDepth, /*任务栈大小，单位为字*/
		                      pvParameters, /*任务形参*/
		                      uxPriority,   /*任务优先级*/
		                      &xReturn,     /*任务句柄*/
		                      pxNewTCB);
		/*将任务添加到就序列表*/
		prvAddNewTaskToReadyList(pxNewTCB);
	}else
	{
		xReturn = NULL;
	}
	
	/*返回任务句柄，如果任务创建成功，此时xReturn应该指向任务控制块*/
	return xReturn;
}

/*空闲任务*/
static portTASK_FUNCTION( prvIdleTask, pvParameters )
{
	/* 防止编译器的警告 */
	( void ) pvParameters;
    
    for(;;)
    {
        /* 空闲任务暂时什么都不做 */
    }
}
/*开启任务调度器*/
void vTaskStartScheduler(void)
{
	/****************创建空闲任务 start*******************/
	TCB_t *pxIdleTaskTCBBuffer = NULL;            /*用于指向空闲任务控制块*/
	StackType_t *pxIdleTaskStackBuffer = NULL;    /*用于空闲任务栈起始地址*/
	uint32_t ulIdleTaskStackSize;
	
	/*获取空闲任务的内存：任务栈和任务TCB*/
	vApplicationGetIdleTaskMemory( &pxIdleTaskTCBBuffer,
	                               &pxIdleTaskStackBuffer,
	                               &ulIdleTaskStackSize );
	/*创建空闲任务*/
	xIdleTaskHandle =
	xTaskCreateStatic( (TaskFunction_t)prvIdleTask,  /*任务入口*/
	                   (char *)"IDLE",               /*任务名称，字符串格式*/
										 (uint32_t)ulIdleTaskStackSize,/*任务栈大小，单位为字*/
										 (void *)NULL,                 /*任务形参*/
											 /*任务优先级，值越大，优先级越高*/
										 (UBaseType_t)tskIDLE_PRIORITY,
										 (StackType_t *)pxIdleTaskStackBuffer,/*任务栈起始地址*/
										 (TCB_t *)pxIdleTaskTCBBuffer );/*任务控制块*/
	/*将任务添加到就序列表*/
//	vListInsertEnd(&(pxReadyTasksLists[0]),
//										 &(((TCB_t *)pxIdleTaskTCBBuffer)->xStateListItem));
		
	/****************创建空闲任务 end*********************/
	
  /*手动指定第一个运行的任务*/
//	pxCurrentTCB = &Task1TCB;
	/*启动调度器*/
	if(xPortStartScheduler() != pdFALSE)
	{
		/*调度器启动成功，则不会返回，即不会来到这里*/
	}
}

#if 0
void vTaskSwitchContext(void)
{
	/*两个任务轮流切换*/
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
	/*获取优先级最高的就绪任务的TCB，然后更新到pxCurrentTCB*/
	taskSELECT_HIGHEST_PRIORITY_TASK();
}
#else
void vTaskSwitchContext(void)
{
	/*如果当前任务是空闲任务，那么就去尝试执行任务1或者任务2，
	  看看他们的延时时间是否结束，如果任务的延时时间均没有到期，
	  那就返回继续执行空闲任务*/
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
			return;     /*任务延时均没有到期则返回，继续执行空闲任务*/
		}
	}
	else      /*当前任务不是空闲任务则会执行到这里*/
	{
		/*如果当前任务是任务1或者任务2的话，检查下另外一个任务，
		如果另外的任务不在延时中，就切换到该任务
		否则，判断下当前任务是否应该进入延时状态，
		如果是的话，就切换到空闲任务，否则就不进行任何切换*/
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
				return;    /*返回，不进行切换，因为两个任务都处于延时中*/
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
				return;    /*返回，不进行切换，因为两个任务都处于延时中*/
			}
		}
	}
}
#endif


void vTaskDelay(const TickType_t xTicksToDelay)
{
	TCB_t *pxTCB = NULL;
	
	/*获取当前任务的TCB*/
	pxTCB = pxCurrentTCB;
	
	/*设置延时时间*/
	pxTCB->xTicksToDelay = xTicksToDelay;
	
	/*将任务从就绪列表移除*/
	//uxListRemove(&(pxTCB->xStateListItem));//现在不行
	taskRESET_READY_PRIORITY(pxTCB->uxPriority);
	
	/*任务切换*/
	taskYIELD();
}

void xTaskIncrementTick(void)
{
	TCB_t *pxTCB = NULL;
	BaseType_t i = 0;
	
	/*更新系统时基计数器xTickCount,xTickCount是一个在port.c中定义的全局变量*/
	const TickType_t xConstTickCount = xTickCount + 1;
	xTickCount = xConstTickCount;
	
	/*扫描就序列表中所有任务的xTickToDelay,如果不为0，则减1*/
	for(i = 0;i<configMAX_PRIORITIES; i++)
	{
		pxTCB = (TCB_t *)listGET_OWNER_OF_HEAD_ENTRY((&pxReadyTasksLists[i]));
		if(pxTCB->xTicksToDelay > 0)
		{
			pxTCB->xTicksToDelay--;
			
			/*延时时间到，将任务就绪*/
			if(pxTCB->xTicksToDelay == 0)
			{
				taskRECORD_READY_PRIORITY(pxTCB->uxPriority);
			}
		}
	}
	/*任务切换*/
	portYIELD();
}

#endif //configSUPPORT_STATIC_ALLOCATION

