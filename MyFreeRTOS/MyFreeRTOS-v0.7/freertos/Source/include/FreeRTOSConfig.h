#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H
#include "portmacro.h"
#define configMAX_TASK_NAME_LEN           16
#define configUSE_16_BIT_TICKS            0
#define configSUPPORT_STATIC_ALLOCATION   1
#define configMAX_PRIORITIES               5

/*********中断相关的配置选项*********/

/*中断最低优先级*/
#define configKERNEL_INTERRUPT_PRIORITY  255  /* 高四位有效，即等于0xff，或者是15 */

#define configMAX_SYSCALL_INTERRUPT_PRIORITY 191

/*SVC、PendSV 和 SysTick中断服务函数*/
#define xPortPendSVHandler     PendSV_Handler
#define xPortSysTickHandler    SysTick_Handler

#define   vPortSVCHandler        SVC_Handler


#endif //FREERTOS_CONFIG_H

