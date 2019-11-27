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
#include "bsp_basic_tim.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_exti.h"

extern uint16_t frequence[8];
uint16_t freCount[8] = {0};
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

//������ʱ���жϷ�����
void TIM6_IRQHandler(void)
{
	if(TIM_GetITStatus(TIMx,TIM_IT_Update))
	{
		//����Ƶ��
		frequence[0] = freCount[0]*2;
		frequence[1] = freCount[1]*2;
		frequence[2] = freCount[2]*2;
		frequence[3] = freCount[3]*2;
		frequence[4] = freCount[4]*2;
		frequence[5] = freCount[5]*2;
		frequence[6] = freCount[6]*2;
		frequence[7] = freCount[7]*2;
		
		//�����ؼ�������
		freCount[0] = 0;
		freCount[1] = 0;
		freCount[2] = 0;
		freCount[3] = 0;
		freCount[4] = 0;
		freCount[5] = 0;
		freCount[6] = 0;
		freCount[7] = 0;
		
		TIM_ClearITPendingBit(TIMx,TIM_IT_Update);
	}
}

//�ⲿ�жϷ�����
void EXTI4_IRQHandler(void)
{
	if (EXTI_GetITStatus(Fre1_INT_EXTI_Line) != RESET) {
		LED1_TOGGLE;
		//�����ؼ�һ
		freCount[0]++;
		//����жϱ�־λ
		EXTI_ClearITPendingBit(Fre1_INT_EXTI_Line);
	}
}
//�ⲿ�жϷ�����
void EXTI9_5_IRQHandler(void)
{	
	if (EXTI_GetITStatus(Fre2_INT_EXTI_Line) != RESET) {
		LED1_TOGGLE;
		//�����ؼ�һ
		freCount[1]++;
		//����жϱ�־λ
		EXTI_ClearITPendingBit(Fre2_INT_EXTI_Line);
	}
	
	if (EXTI_GetITStatus(Fre3_INT_EXTI_Line) != RESET) {
		LED1_TOGGLE;
		//�����ؼ�һ
		freCount[2]++;
		//����жϱ�־λ
		EXTI_ClearITPendingBit(Fre3_INT_EXTI_Line);
	}
	
	if (EXTI_GetITStatus(Fre4_INT_EXTI_Line) != RESET) {
		LED1_TOGGLE;
		//�����ؼ�һ
		freCount[3]++;
		//����жϱ�־λ
		EXTI_ClearITPendingBit(Fre4_INT_EXTI_Line);
	}
}

//�ⲿ�жϷ�����
void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(Fre5_INT_EXTI_Line) != RESET) {
		LED1_TOGGLE;
		//�����ؼ�һ
		freCount[4]++;
		//����жϱ�־λ
		EXTI_ClearITPendingBit(Fre5_INT_EXTI_Line);
	}
	
	if (EXTI_GetITStatus(Fre6_INT_EXTI_Line) != RESET) {
		LED1_TOGGLE;
		//�����ؼ�һ
		freCount[5]++;
		//����жϱ�־λ
		EXTI_ClearITPendingBit(Fre6_INT_EXTI_Line);
	}
	
	if (EXTI_GetITStatus(Fre7_INT_EXTI_Line) != RESET) {
		LED1_TOGGLE;
		//�����ؼ�һ
		freCount[6]++;
		//����жϱ�־λ
		EXTI_ClearITPendingBit(Fre7_INT_EXTI_Line);
	}
	
	if (EXTI_GetITStatus(Fre8_INT_EXTI_Line) != RESET) {
		LED1_TOGGLE;
		//�����ؼ�һ
		freCount[7]++;
		//����жϱ�־λ
		EXTI_ClearITPendingBit(Fre8_INT_EXTI_Line);
	}
}



/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
