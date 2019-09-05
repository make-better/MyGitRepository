/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "stm32f10x_tim.h"
#include "bsp_general_Tim.h"

extern TIM_ICUserValueType TIM_ICUserValue[8];

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

void TIM6_IRQHandler(void)
{
	
}

void TIM3_IRQHandler(void)
{
	 if ( TIM_GetITStatus ( TIM3, TIM_IT_Update) != RESET )
	{
		
		TIM_ClearITPendingBit ( TIM3, TIM_FLAG_Update );
	}
#if USE_TIM3_CH1
	// TIM3_CH1上升沿捕获中断
	if ( TIM_GetITStatus (TIM3, TIM_IT_CC1 ) != RESET) 
		{
			// 第一次捕获
			if ( TIM_ICUserValue[0].Capture_StartFlag == 0 )
				{
					//记录首次捕获时计数器的值
					TIM_ICUserValue[0].Capture_CNTValue = TIM3->CNT;							
					// 存捕获比较寄存器的值的变量的值清 0
					TIM_ICUserValue[0].Capture_CcrValue = 0;
					// 开始捕获标准置 1
					TIM_ICUserValue[0].Capture_StartFlag = 1;
				}
			// 下降沿捕获中断
			else { // 第二次捕获
					// 获取捕获比较寄存器的值减去上一次计数器的值，这个值就是捕获到的高电平的时间的值
					TIM_ICUserValue[0].Capture_CcrValue =	TIM_GetCapture1(TIM3)-TIM_ICUserValue[0].Capture_CNTValue;
					// 开始捕获标志清 0
					TIM_ICUserValue[0].Capture_StartFlag = 0;
					// 捕获完成标志置 1
					TIM_ICUserValue[0].Capture_FinishFlag = 1;
					}
			 TIM_ClearITPendingBit (TIM3,TIM_IT_CC1);
		}
#endif
		
#if USE_TIM3_CH2
	// TIM3_CH2上升沿捕获中断
	if ( TIM_GetITStatus (TIM3, TIM_IT_CC2 ) != RESET) 
		{
			if ( TIM_ICUserValue[1].Capture_StartFlag == 0 ) 
				{
					TIM_ICUserValue[1].Capture_CNTValue = TIM3->CNT;
					TIM_ICUserValue[1].Capture_CcrValue = 0;
					TIM_ICUserValue[1].Capture_StartFlag = 1;
				}
			else {
					TIM_ICUserValue[1].Capture_CcrValue =	TIM_GetCapture2(TIM3)-TIM_ICUserValue[1].Capture_CNTValue;		
					TIM_ICUserValue[1].Capture_StartFlag = 0;
					TIM_ICUserValue[1].Capture_FinishFlag = 1;
					}
			 TIM_ClearITPendingBit (TIM3,TIM_IT_CC2);
		}
#endif

#if USE_TIM3_CH3
	// TIM3_CH3上升沿捕获中断
	  if ( TIM_GetITStatus (TIM3, TIM_IT_CC3 ) != RESET)
		{
			if ( TIM_ICUserValue[2].Capture_StartFlag == 0 ) 
				{
					TIM_ICUserValue[2].Capture_CNTValue = TIM3->CNT;
					TIM_ICUserValue[2].Capture_CcrValue = 0;
					TIM_ICUserValue[2].Capture_StartFlag = 1;
				}
			else {
					TIM_ICUserValue[2].Capture_CcrValue =	TIM3->CCR3-TIM_ICUserValue[2].Capture_CNTValue;		
					TIM_ICUserValue[2].Capture_StartFlag = 0;
					TIM_ICUserValue[2].Capture_FinishFlag = 1;
					}
			 TIM_ClearITPendingBit (TIM3,TIM_IT_CC3);
		}
#endif
		
#if USE_TIM3_CH4
	// TIM3_CH4上升沿捕获中断
	if ( TIM_GetITStatus (TIM3, TIM_IT_CC4 ) != RESET) 
		{
			if ( TIM_ICUserValue[3].Capture_StartFlag == 0 ) 
				{
					TIM_ICUserValue[3].Capture_CNTValue = TIM3->CNT;
					TIM_ICUserValue[3].Capture_CcrValue = 0;
					TIM_ICUserValue[3].Capture_StartFlag = 1;
				}
			else {
					TIM_ICUserValue[3].Capture_CcrValue =	TIM_GetCapture4(TIM3)-TIM_ICUserValue[3].Capture_CNTValue;		
					TIM_ICUserValue[3].Capture_StartFlag = 0;
					TIM_ICUserValue[3].Capture_FinishFlag = 1;
					}
			 TIM_ClearITPendingBit (TIM3,TIM_IT_CC4);
		}
#endif
}
/**
  * @}
  */ 
void TIM4_IRQHandler(void)
{
	// 采集波形的一个周期内发生了寄存器溢出，暂时不处理
	 if ( TIM_GetITStatus ( TIM4, TIM_IT_Update) != RESET )
	{
		
		TIM_ClearITPendingBit ( TIM4, TIM_FLAG_Update );
	}
#if USE_TIM4_CH1
	// TIM4_CH1上升沿捕获中断
	if ( TIM_GetITStatus (TIM4, TIM_IT_CC1 ) != RESET) 
		{
			// 第一次捕获
			if ( TIM_ICUserValue[4].Capture_StartFlag == 0 )
				{
					//记录首次捕获时计数器的值
					TIM_ICUserValue[4].Capture_CNTValue = TIM4->CNT;							
					// 存捕获比较寄存器的值的变量的值清 0
					TIM_ICUserValue[4].Capture_CcrValue = 0;
					// 开始捕获标准置 1
					TIM_ICUserValue[4].Capture_StartFlag = 1;
				}
			// 下降沿捕获中断
			else { // 第二次捕获
					// 获取捕获比较寄存器的值减去上一次计数器的值，这个值就是捕获到的高电平的时间的值
					TIM_ICUserValue[4].Capture_CcrValue =	TIM_GetCapture1(TIM4)-TIM_ICUserValue[4].Capture_CNTValue;
					// 开始捕获标志清 0
					TIM_ICUserValue[4].Capture_StartFlag = 0;
					// 捕获完成标志置 1
					TIM_ICUserValue[4].Capture_FinishFlag = 1;
					}
			 TIM_ClearITPendingBit (TIM4,TIM_IT_CC1);
		}
#endif
		
#if USE_TIM4_CH2
	// TIM4_CH2上升沿捕获中断
	if ( TIM_GetITStatus (TIM4, TIM_IT_CC2 ) != RESET) 
		{
			if ( TIM_ICUserValue[5].Capture_StartFlag == 0 ) 
				{
					TIM_ICUserValue[5].Capture_CNTValue = TIM4->CNT;
					TIM_ICUserValue[5].Capture_CcrValue = 0;
					TIM_ICUserValue[5].Capture_StartFlag = 1;
				}
			else {
					TIM_ICUserValue[5].Capture_CcrValue =	TIM_GetCapture2(TIM4)-TIM_ICUserValue[5].Capture_CNTValue;		
					TIM_ICUserValue[5].Capture_StartFlag = 0;
					TIM_ICUserValue[5].Capture_FinishFlag = 1;
					}
			 TIM_ClearITPendingBit (TIM4,TIM_IT_CC2);
		}
#endif

#if USE_TIM4_CH3
	// TIM4_CH3上升沿捕获中断
	  if ( TIM_GetITStatus (TIM4, TIM_IT_CC3 ) != RESET)
		{
			if ( TIM_ICUserValue[6].Capture_StartFlag == 0 ) 
				{
					TIM_ICUserValue[6].Capture_CNTValue = TIM4->CNT;
					TIM_ICUserValue[6].Capture_CcrValue = 0;
					TIM_ICUserValue[6].Capture_StartFlag = 1;
				}
			else {
					TIM_ICUserValue[6].Capture_CcrValue =	TIM4->CCR3-TIM_ICUserValue[6].Capture_CNTValue;		
					TIM_ICUserValue[6].Capture_StartFlag = 0;
					TIM_ICUserValue[6].Capture_FinishFlag = 1;
					}
			 TIM_ClearITPendingBit (TIM4,TIM_IT_CC3);
		}
#endif
		
#if USE_TIM4_CH4
	// TIM4_CH4上升沿捕获中断
	if ( TIM_GetITStatus (TIM4, TIM_IT_CC4 ) != RESET) 
		{
			if ( TIM_ICUserValue[7].Capture_StartFlag == 0 ) 
				{
					TIM_ICUserValue[7].Capture_CNTValue = TIM4->CNT;
					TIM_ICUserValue[7].Capture_CcrValue = 0;
					TIM_ICUserValue[7].Capture_StartFlag = 1;
				}
			else {
					TIM_ICUserValue[7].Capture_CcrValue =	TIM_GetCapture4(TIM4)-TIM_ICUserValue[7].Capture_CNTValue;		
					TIM_ICUserValue[7].Capture_StartFlag = 0;
					TIM_ICUserValue[7].Capture_FinishFlag = 1;
					}
			 TIM_ClearITPendingBit (TIM4,TIM_IT_CC4);
		}
#endif
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
