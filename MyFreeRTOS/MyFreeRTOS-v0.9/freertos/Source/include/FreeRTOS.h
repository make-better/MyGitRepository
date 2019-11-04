#ifndef FREERTOS_H
#define FREERTOS_H
#include "FreeRTOSConfig.h"
#include "projdefs.h"
#include "portable.h"
#include "list.h"



typedef struct tskTaskControlBlock
{
	volatile StackType_t    *pxTopOfStack;      /*ջ��*/
	ListItem_t              xStateListItem;     /*����ڵ�*/
	StackType_t             *pxStack;           /*����ջ��ʼ��ַ*/
	char                    pcTaskName[configMAX_TASK_NAME_LEN];/*�������ƣ��ַ�����ʽ*/
	TickType_t xTicksToDelay;                   /*������ʱ*/
}tskTCB;

typedef tskTCB TCB_t;

StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack,
                                    TaskFunction_t pxCode,
                                    void *pvParameters );





#endif


