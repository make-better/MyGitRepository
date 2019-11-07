#include "FreeRTOS.h"
#include "task.h"
#include "ARMCM3.h"
#include "FreeRTOSConfig.h"

#define portINITIAL_XPSR             (0x01000000)
#define portSTART_ADDRESS_MASK       ( ( StackType_t ) 0xfffffffeUL )

#define portNVIC_SYSPRI2_REG         (*((volatile uint32_t *) 0xe000ed20))

#define portNVIC_PENDSV_PRI          (((uint32_t) configKERNEL_INTERRUPT_PRIORITY)<<16UL)
#define portNVIC_SYSTICK_PRI          (((uint32_t) configKERNEL_INTERRUPT_PRIORITY)<<24UL)

static UBaseType_t uxCriticalNesting = 0xaaaaaaaa;

TickType_t xTickCount;

/*SysTick 控制寄存器*/
#define portNVIC_SYSTICK_CTRL_REG (*((volatile uint32_t *) 0xe000e010))
/*SysTick 重装载寄存器*/
#define portNVIC_SYSTICK_LOAD_REG (*((volatile uint32_t *) 0xe000e014))

/*SysTick 时钟源选择*/
#ifndef configSYSTICK_CLOCK_HZ
    #define configSYSTICK_CLOCK_HZ configCPU_CLOCK_HZ
    /*确保SysTick的时钟与内核时钟一致*/
    #define portNVIC_SYSTICK_CLK_BIT ( 1UL << 2UL )
#else
    #define portNVIC_SYSTICK_CLK_BIT ( 0 )
#endif

#define portNVIC_SYSTICK_INT_BIT     ( 1UL << 1UL )
#define portNVIC_SYSTICK_ENABLE_BIT  ( 1UL << 0UL )

void vPortSetupTimerInterrupt(void)
{
	/*设置重装载寄存器的值*/
	portNVIC_SYSTICK_LOAD_REG = (configSYSTICK_CLOCK_HZ / configTICK_RATE_HZ) -1UL;
	
	/*设置系统定时器的时钟等于内核时钟
	  使能SysTick 定时器中断
	  使能SysTick 定时器*/
	portNVIC_SYSTICK_CTRL_REG = (portNVIC_SYSTICK_CLK_BIT |
	                             portNVIC_SYSTICK_INT_BIT |
	                             portNVIC_SYSTICK_ENABLE_BIT );
}
/*函数声明*/
void prvStartFirstTask( void );
void vPortSVCHandler( void );
void xPortPendSVHandler( void );


static void prvTaskExitError(void)
{
	/*函数停止在这里*/
	for(;;);
}

StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack,
                                    TaskFunction_t pxCode,
                                    void *pvParameters )
{
    /*异常发生时，自动加载到CPU寄存器的内容*/
    pxTopOfStack--;
    *pxTopOfStack = portINITIAL_XPSR;	             /*xPSRd的bit24必须置1*/
	  pxTopOfStack--;
	  /*任务代码的地址*/
	  *pxTopOfStack = (( StackType_t ) pxCode )&portSTART_ADDRESS_MASK;  /*PC,即任务入口函数*/
	  pxTopOfStack--;
	  *pxTopOfStack = ( StackType_t )prvTaskExitError;                   /*LR,函数返回地址*/
	  pxTopOfStack -= 5; /* R12, R3, R2 and R1 默认初始化为0*/
	  *pxTopOfStack = ( StackType_t )pvParameters;                       /*R0,任务形参*/
	
	  /*异常发生时，手动加载到CPU寄存器的内容*/
	  pxTopOfStack -= 8;/*R11, R10, R9, R8, R7, R6, R5 and R4默认初始化为0*/
	  
	  /*返回栈顶指针，此时pxTopOfStack指向空闲栈*/
	  return pxTopOfStack;
}

BaseType_t xPortStartScheduler(void)
{
	/*配置 PendSV 和 SysTick 的中断优先级为最低*/
	portNVIC_SYSPRI2_REG |= portNVIC_PENDSV_PRI;
	portNVIC_SYSPRI2_REG |= portNVIC_SYSTICK_PRI;
	
	/*初始化SysTick*/
	vPortSetupTimerInterrupt();
	/*启动第一个任务，不在返回*/
	prvStartFirstTask();
	
	/*不应该运行到这里*/
	return 0;
}


__asm void prvStartFirstTask(void)
{
	PRESERVE8
	
	/*在Cortex-M中，0xE000ED08是SCB_VTOR这个寄存器的地址，
	 里面存放的是向量表的起始地址，即MSP的地址*/
	ldr r0, =0xE000ED08
	ldr r0, [r0]   //0x00000000
	ldr r0, [r0]
	
	/*设置主堆栈指针msp的指针*/
	msr msp, r0
	
	/*使能全局中断*/
	cpsie i
	cpsie f
	dsb
	isb
	
	/*调用SVC去启动第一个任务*/
	svc 0
	nop
	nop
}

__asm void vPortSVCHandler(void)
{
	extern pxCurrentTCB;
	
	PRESERVE8
	ldr r3, =pxCurrentTCB
	ldr r1, [r3]
	ldr r0, [r1]
	ldmia r0!, {r4-r11}
	msr psp, r0
	isb
	mov r0, #0
	msr basepri, r0
	orr r14, #0xd
	
	bx r14
}

__asm void xPortPendSVHandler(void)
{
	extern pxCurrentTCB;
	extern vTaskSwitchContext;
	
	PRESERVE8 
	
	mrs r0, psp
	isb
	
	ldr r3, =pxCurrentTCB
	ldr r2, [r3]
	
	stmdb r0!, {r4-r11}
	str r0, [r2]
	
	stmdb sp!, {r3, r14}
	mov r0, #configMAX_SYSCALL_INTERRUPT_PRIORITY
	msr basepri, r0
	dsb
	isb
	bl vTaskSwitchContext
	mov r0, #0
	msr basepri, r0
	ldmia sp!, {r3,r14}
	
	ldr r1, [r3]
	ldr r0, [r1]
	ldmia r0!, {r4-r11}
	msr psp, r0
	isb
	bx r14
	nop
}

/**********进入临界段 不带中断保护 不能嵌套*********/
void vPortEnterCritical(void)
{
	portDISABLE_INTERRUPTS();
	uxCriticalNesting++;
	
	if(uxCriticalNesting == 1)
	{
		//configASSERT((portNVIC_INT_CTRL_REG & portVECTACTIVE_MASK) == 0);
	}
}

void vPortExitCritical(void)
{
	//configASSERT( uxCriticalNesting );
	uxCriticalNesting--;
    
	if( uxCriticalNesting == 0 )
	{
		portENABLE_INTERRUPTS();
	}
}

/*SysTick中断服务函数*/
void xPortSysTickHandler(void)
{
	/*关中断*/
	vPortRaiseBASEPRI();
	
	/*更新系统时基*/
	xTaskIncrementTick();
	
	/*关中断*/
	vPortClearBASEPRIFromISR();
}
