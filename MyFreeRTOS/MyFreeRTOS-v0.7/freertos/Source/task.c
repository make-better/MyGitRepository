#include "FreeRTOS.h"
#include "task.h"
/*任务就绪列表*/
List_t pxReadyTasksLists[ configMAX_PRIORITIES ];
/*当前任务控制块指针*/
TCB_t * volatile pxCurrentTCB = NULL;
/*Task1 Task2 来自main.c*/
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
	/*初始化任务栈*/
	pxNewTCB->pxTopOfStack = pxPortInitialiseStack(pxTopOfStack,pxTaskCode,pvParameters);
	
	/*让任务句柄指向任务控制块*/
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
		/*创建新的任务*/
		prvInitialiseNewTask( pxTaskCode,   /*任务入口*/
		                      pcName,       /*任务名称，字符串形式*/
		                      ulStackDepth, /*任务栈大小，单位为字*/
		                      pvParameters, /*任务形参*/
		                      &xReturn,     /*任务句柄*/
		                      pxNewTCB);
	}else
	{
		xReturn = NULL;
	}
	
	/*返回任务句柄，如果任务创建成功，此时xReturn应该指向任务控制块*/
	return xReturn;
}
/*就绪列表初始化*/
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
	/*手动指定第一个运行的任务*/
	pxCurrentTCB = &Task1TCB;
	/*启动调度器*/
	if(xPortStartScheduler() != pdFALSE)
	{
		/*调度器启动成功，则不会返回，即不会来到这里*/
	}
}

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

#endif //configSUPPORT_STATIC_ALLOCATION



















