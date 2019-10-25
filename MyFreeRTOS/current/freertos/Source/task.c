#include "task.h"


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



#endif //configSUPPORT_STATIC_ALLOCATION



















