#ifndef FREERTOS_H
#define FREERTOS_H
#include "FreeRTOSConfig.h"
#include "list.h"
#include "task.h"
#include "projdefs.h"

typedef struct tskTaskControlBlock
{
	volatile StackType_t    *pxTopOfStack;      /*ջ��*/
	ListItem_t              xStateListItem;     /*����ڵ�*/
	StackType_t             *pxStack;           /*����ջ��ʼ��ַ*/
	char                    pcTaskName[configMAX_TASK_NAME_LEN];/*�������ƣ��ַ�����ʽ*/
}tskTCB;

typedef tskTCB TCB_t;







#endif


