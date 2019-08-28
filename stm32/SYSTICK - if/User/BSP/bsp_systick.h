#ifndef _BSP_SYSTICK_H_
#define _BSP_SYSTICK_H_
//#include "core_cm3.h"
#include "stm32f10x.h"
void SysTick_Delay_us(uint32_t us);
void SysTick_Delay_ms(uint32_t ms);
void SysTick_Init(void);
void wtf(void);
void delay_ms(uint32_t n);

#endif //_BSP_SYSTICK_H_
