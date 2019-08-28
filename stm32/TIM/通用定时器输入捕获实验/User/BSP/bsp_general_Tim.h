#ifndef _BSP_GENERAL_TIM_H_
#define _BSP_GENERAL_TIM_H_

#include "stm32f10x.h"


typedef struct {
	uint8_t Capture_FinishFlag; // ���������־λ
	uint8_t Capture_StartFlag; // ����ʼ��־λ
	uint16_t Capture_CcrValue; // ����Ĵ�����ֵ
	uint16_t Capture_Period; // �Զ���װ�ؼĴ������±�־
}TIM_ICUserValueType;


#define GENERAL_TIMx									TIM5
#define GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define GENERAL_TIM_CLK               RCC_APB1Periph_TIM5

#define GENERAL_TIM_CH1_GPIO_CLK 			RCC_APB2Periph_GPIOA 


#define GENERAL_TIM_CH1_PIN						GPIO_Pin_0
#define GENERAL_TIM_CH1_PORT					GPIOA
#define GENERAL_TIM_CHANNEL_x 				TIM_Channel_1

 // �ж���غ궨��
#define GENERAL_TIM_IT_CCx						TIM_IT_CC1
#define GENERAL_TIM_IRQ 							TIM5_IRQn
#define GENERAL_TIM_INT_FUN 					TIM5_IRQHandler
// ��ȡ����Ĵ���ֵ�����궨��
#define GENERAL_TIM_GetCapturex_FUN 	TIM_GetCapture1
// �����źż��Ժ����궨��
#define GENERAL_TIM_OCxPolarityConfig_FUN TIM_OC1PolarityConfig
// ��������ʼ����
#define GENERAL_TIM_STRAT_ICPolarity 	TIM_ICPolarity_Rising
// �����Ľ�������
#define GENERAL_TIM_END_ICPolarity 		TIM_ICPolarity_Falling

void TIM_Init(void);





#endif // _BSP_GENERAL_TIM_H_
