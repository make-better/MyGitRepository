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



#endif //configSUPPORT_STATIC_ALLOCATION



















