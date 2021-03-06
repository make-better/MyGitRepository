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
#include "bsp_breath.h"
extern __IO uint16_t period_class;
extern uint16_t indexWave[];
extern uint16_t POINT_NUM;
extern uint32_t color_value;
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
/**
  * @}
  */ 

void BRE_TIMx_IRQHandler(void)
{
	static uint16_t tim_cut=0;
	static uint16_t tim_cir=0;
	static uint16_t V_class=0;
	if(TIM_GetITStatus(BRE_TIMx,TIM_IT_Update) == SET)
	{
		V_class++;
		if(V_class>=MAX-1)
		{
			tim_cut++;
			if(tim_cut>=period_class)
			{
				tim_cir++;
				
				tim_cut = 0;
			}
			if(tim_cir>=POINT_NUM)
			{
				tim_cir = 0;
				tim_cut = 0;
			}
			V_class=0;
		}
		else
		{
			//红色LED幅值控制
			if((color_value&0xff0000)>>16 >= V_class)
			{
				BRE_TIMx->BRE_RED_CCRx = indexWave[tim_cir];
			}
			else
			{
				BRE_TIMx->BRE_RED_CCRx = 0;
			}
			//绿色LED幅值控制
			if((color_value&0xff00)>>8 >= V_class)
			{
				BRE_TIMx->BRE_GREEN_CCRx = indexWave[tim_cir];
			}
			else
			{
				BRE_TIMx->BRE_GREEN_CCRx = 0;
			}
			//蓝色LED幅值控制
			if((color_value&0xff)>= V_class)
			{
				BRE_TIMx->BRE_BLUE_CCRx = indexWave[tim_cir];
			}
			else
			{
				BRE_TIMx->BRE_BLUE_CCRx = 0;
			}
		}
		
	}
	TIM_ClearITPendingBit(BRE_TIMx,TIM_IT_Update);
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
