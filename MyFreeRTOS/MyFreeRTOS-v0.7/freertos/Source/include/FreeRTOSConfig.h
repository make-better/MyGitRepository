#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H
#include "portmacro.h"
#define configMAX_TASK_NAME_LEN           16
#define configUSE_16_BIT_TICKS            0
#define configSUPPORT_STATIC_ALLOCATION   1
#define configMAX_PRIORITIES               5

/*********�ж���ص�����ѡ��*********/

/*�ж�������ȼ�*/
#define configKERNEL_INTERRUPT_PRIORITY  255  /* ����λ��Ч��������0xff��������15 */

#define configMAX_SYSCALL_INTERRUPT_PRIORITY 191

/*SVC��PendSV �� SysTick�жϷ�����*/
#define xPortPendSVHandler     PendSV_Handler
#define xPortSysTickHandler    SysTick_Handler

#define   vPortSVCHandler        SVC_Handler


#endif //FREERTOS_CONFIG_H

