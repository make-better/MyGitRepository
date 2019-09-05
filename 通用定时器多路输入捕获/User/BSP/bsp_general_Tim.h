#ifndef _BSP_GENERAL_TIM_H_
#define _BSP_GENERAL_TIM_H_

#include "stm32f10x.h"


typedef struct {
	uint8_t Capture_FinishFlag; // ���������־λ
	uint8_t Capture_StartFlag; // ����ʼ��־λ
	uint16_t Capture_CNTValue; // ��һ�β������ʱ��������ֵ
	uint16_t Capture_CcrValue; // ����Ĵ�����ֵ
	uint16_t Capture_Period; // �Զ���װ�ؼĴ������±�־
}TIM_ICUserValueType;

#define USE_TIM3							1
#define USE_TIM3_CH1          1
#define USE_TIM3_CH2          1
#define USE_TIM3_CH3          1
#define USE_TIM3_CH4          1
//��TIM3�Ķ���
#define TIM3_IT_CCx						/**/TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4/**/


#define USE_TIM4              0
#define USE_TIM4_CH1          1
#define USE_TIM4_CH2          1
#define USE_TIM4_CH3          1
#define USE_TIM4_CH4          1
//��TIM4�Ķ���
#define TIM4_IT_CCx						TIM_IT_CC1//|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4


//��ʱ�ӵĺ궨��
//TIM3
#if (USE_TIM3_CH1|USE_TIM3_CH2)&(USE_TIM3_CH3|USE_TIM3_CH4)		
	#define TIM3_GPIO_CLK 	RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC 
#elif (USE_TIM3_CH1|USE_TIM3_CH2 == 1)&&(USE_TIM3_CH3|USE_TIM3_CH4 == 0)
	#define TIM3_GPIO_CLK 	RCC_APB2Periph_GPIOA
#else
	#define TIM3_GPIO_CLK 	RCC_APB2Periph_GPIOB
#endif														
//TIM4
#define TIM4_GPIO_CLK 		RCC_APB2Periph_GPIOB
	
													
	
//TIM3��GPIO����
#define TIM3_CH1_PIN						GPIO_Pin_6
#define TIM3_CH1_PORT					GPIOA

#define TIM3_CH2_PIN						GPIO_Pin_7
#define TIM3_CH2_PORT					GPIOA

#define TIM3_CH3_PIN						GPIO_Pin_0
#define TIM3_CH3_PORT					GPIOB

#define TIM3_CH4_PIN						GPIO_Pin_1
#define TIM3_CH4_PORT					GPIOB

//TIM4��GPIO����
#define TIM4_CH1_PIN						GPIO_Pin_6
#define TIM4_CH1_PORT					GPIOB

#define TIM4_CH2_PIN						GPIO_Pin_7
#define TIM4_CH2_PORT					GPIOB

#define TIM4_CH3_PIN						GPIO_Pin_8
#define TIM4_CH3_PORT					GPIOB

#define TIM4_CH4_PIN						GPIO_Pin_9
#define TIM4_CH4_PORT					GPIOB




// ��ȡ����Ĵ���ֵ�����궨��
#define GENERAL_TIM_GetCapturex_FUN 	TIM_GetCapture1
// �����źż��Ժ����궨��
#define GENERAL_TIM_OCxPolarityConfig_FUN TIM_OC1PolarityConfig
// ��������ʼ����
#define GENERAL_TIM_STRAT_ICPolarity 	TIM_ICPolarity_Rising
// �����Ľ�������
//#define GENERAL_TIM_END_ICPolarity 		TIM_ICPolarity_Falling

void TIM_Init(void);
void PWM_IC_Print(void);




#endif // _BSP_GENERAL_TIM_H_
