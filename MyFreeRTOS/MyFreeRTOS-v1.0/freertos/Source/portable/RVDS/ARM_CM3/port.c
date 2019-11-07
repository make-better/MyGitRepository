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

/*SysTick ���ƼĴ���*/
#define portNVIC_SYSTICK_CTRL_REG (*((volatile uint32_t *) 0xe000e010))
/*SysTick ��װ�ؼĴ���*/
#define portNVIC_SYSTICK_LOAD_REG (*((volatile uint32_t *) 0xe000e014))

/*SysTick ʱ��Դѡ��*/
#ifndef configSYSTICK_CLOCK_HZ
    #define configSYSTICK_CLOCK_HZ configCPU_CLOCK_HZ
    /*ȷ��SysTick��ʱ�����ں�ʱ��һ��*/
    #define portNVIC_SYSTICK_CLK_BIT ( 1UL << 2UL )
#else
    #define portNVIC_SYSTICK_CLK_BIT ( 0 )
#endif

#define portNVIC_SYSTICK_INT_BIT     ( 1UL << 1UL )
#define portNVIC_SYSTICK_ENABLE_BIT  ( 1UL << 0UL )

void vPortSetupTimerInterrupt(void)
{
	/*������װ�ؼĴ�����ֵ*/
	portNVIC_SYSTICK_LOAD_REG = (configSYSTICK_CLOCK_HZ / configTICK_RATE_HZ) -1UL;
	
	/*����ϵͳ��ʱ����ʱ�ӵ����ں�ʱ��
	  ʹ��SysTick ��ʱ���ж�
	  ʹ��SysTick ��ʱ��*/
	portNVIC_SYSTICK_CTRL_REG = (portNVIC_SYSTICK_CLK_BIT |
	                             portNVIC_SYSTICK_INT_BIT |
	                             portNVIC_SYSTICK_ENABLE_BIT );
}
/*��������*/
void prvStartFirstTask( void );
void vPortSVCHandler( void );
void xPortPendSVHandler( void );


static void prvTaskExitError(void)
{
	/*����ֹͣ������*/
	for(;;);
}

StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack,
                                    TaskFunction_t pxCode,
                                    void *pvParameters )
{
    /*�쳣����ʱ���Զ����ص�CPU�Ĵ���������*/
    pxTopOfStack--;
    *pxTopOfStack = portINITIAL_XPSR;	             /*xPSRd��bit24������1*/
	  pxTopOfStack--;
	  /*�������ĵ�ַ*/
	  *pxTopOfStack = (( StackType_t ) pxCode )&portSTART_ADDRESS_MASK;  /*PC,��������ں���*/
	  pxTopOfStack--;
	  *pxTopOfStack = ( StackType_t )prvTaskExitError;                   /*LR,�������ص�ַ*/
	  pxTopOfStack -= 5; /* R12, R3, R2 and R1 Ĭ�ϳ�ʼ��Ϊ0*/
	  *pxTopOfStack = ( StackType_t )pvParameters;                       /*R0,�����β�*/
	
	  /*�쳣����ʱ���ֶ����ص�CPU�Ĵ���������*/
	  pxTopOfStack -= 8;/*R11, R10, R9, R8, R7, R6, R5 and R4Ĭ�ϳ�ʼ��Ϊ0*/
	  
	  /*����ջ��ָ�룬��ʱpxTopOfStackָ�����ջ*/
	  return pxTopOfStack;
}

BaseType_t xPortStartScheduler(void)
{
	/*���� PendSV �� SysTick ���ж����ȼ�Ϊ���*/
	portNVIC_SYSPRI2_REG |= portNVIC_PENDSV_PRI;
	portNVIC_SYSPRI2_REG |= portNVIC_SYSTICK_PRI;
	
	/*��ʼ��SysTick*/
	vPortSetupTimerInterrupt();
	/*������һ�����񣬲��ڷ���*/
	prvStartFirstTask();
	
	/*��Ӧ�����е�����*/
	return 0;
}


__asm void prvStartFirstTask(void)
{
	PRESERVE8
	
	/*��Cortex-M�У�0xE000ED08��SCB_VTOR����Ĵ����ĵ�ַ��
	 �����ŵ������������ʼ��ַ����MSP�ĵ�ַ*/
	ldr r0, =0xE000ED08
	ldr r0, [r0]   //0x00000000
	ldr r0, [r0]
	
	/*��������ջָ��msp��ָ��*/
	msr msp, r0
	
	/*ʹ��ȫ���ж�*/
	cpsie i
	cpsie f
	dsb
	isb
	
	/*����SVCȥ������һ������*/
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

/**********�����ٽ�� �����жϱ��� ����Ƕ��*********/
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

/*SysTick�жϷ�����*/
void xPortSysTickHandler(void)
{
	/*���ж�*/
	vPortRaiseBASEPRI();
	
	/*����ϵͳʱ��*/
	xTaskIncrementTick();
	
	/*���ж�*/
	vPortClearBASEPRIFromISR();
}
