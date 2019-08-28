#include "bsp_advance_tim.h"

/***��ʱ��1��ģʽ***/
void TIM1_config(uint32_t Cycle)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_TIM1 , ENABLE); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                   //TIM1_CH4 PA11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_TimeBaseStructure.TIM_Period = Cycle-1;                                                   
    TIM_TimeBaseStructure.TIM_Prescaler =71;                    //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ                                                     
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //����ʱ�ӷָTDTS= Tck_tim            
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;            //�ظ�������һ��Ҫ=0������
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);                                       

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;          //ѡ��ʱ��ģʽ��TIM�����ȵ���ģʽ1       
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_Pulse = Cycle/2-1;                    //���ô�װ�벶��Ĵ���������ֵ                                   
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      //�������       

    TIM_OC4Init(TIM1, &TIM_OCInitStructure);                                                         

    TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);
    TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);
    
		TIM_CtrlPWMOutputs(TIM1, ENABLE);  //�����ʹ��
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);                              
    TIM_ARRPreloadConfig(TIM1, ENABLE);                                                          
}
/***��ʱ��2��ģʽ***/
void TIM2_config(uint32_t PulseNum)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure; 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = PulseNum-1;   
    TIM_TimeBaseStructure.TIM_Prescaler =0;    
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);  

    TIM_SelectInputTrigger(TIM2, TIM_TS_ITR0);
    //TIM_InternalClockConfig(TIM3);
    TIM2->SMCR|=0x07;                                  //���ô�ģʽ�Ĵ��� 
    //TIM_ITRxExternalClockConfig(TIM2, TIM_TS_ITR0);

    //TIM_ARRPreloadConfig(TIM3, ENABLE);         
    TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);

   // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;        
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;     
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure);
}
void Pulse_output(uint32_t Cycle,uint32_t PulseNum)
{
    TIM2_config(PulseNum);
    TIM_Cmd(TIM2, ENABLE);
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    TIM1_config(Cycle);
    
    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);   //�߼���ʱ��һ��Ҫ���ϣ������ʹ��
}




