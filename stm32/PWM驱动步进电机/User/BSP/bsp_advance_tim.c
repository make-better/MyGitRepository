#include "bsp_advance_tim.h"

/***定时器1主模式***/
void TIM1_config(uint16_t Cycle)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CH4_GPIO_CLK|ADVANCE_TIM_CLK , ENABLE); 

    GPIO_InitStructure.GPIO_Pin = ADVANCE_TIM_CH4_PIN;                   //TIM1_CH4 PA11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ADVANCE_TIM_CH4_PORT, &GPIO_InitStructure);

    TIM_TimeBaseStructure.TIM_Period = Cycle-1;                                                   
    TIM_TimeBaseStructure.TIM_Prescaler =71;                    //设置用来作为TIMx时钟频率除数的预分频值                                                     
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //设置时钟分割：TDTS= Tck_tim            
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;            //重复计数，一定要=0！！！
    TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);                                       

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;          //选择定时器模式：TIM脉冲宽度调制模式1       
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_Pulse = Cycle/2-1;                    //设置待装入捕获寄存器的脉冲值                                   
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      //输出极性       

    TIM_OC4Init(ADVANCE_TIM, &TIM_OCInitStructure);                                                         

    TIM_SelectMasterSlaveMode(ADVANCE_TIM, TIM_MasterSlaveMode_Enable);
    TIM_SelectOutputTrigger(ADVANCE_TIM, TIM_TRGOSource_Update);
    
		TIM_CtrlPWMOutputs(ADVANCE_TIM, ENABLE);  //主输出使能
    TIM_OC4PreloadConfig(ADVANCE_TIM, TIM_OCPreload_Enable);                              
    TIM_ARRPreloadConfig(ADVANCE_TIM, ENABLE);                                                          
}
/***定时器2从模式***/
void TIM2_config(uint16_t PulseNum)
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
    TIM2->SMCR|=0x07;                                  //设置从模式寄存器 
    //TIM_ITRxExternalClockConfig(TIM2, TIM_TS_ITR0);

    //TIM_ARRPreloadConfig(TIM3, ENABLE);         
    TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;        
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;     
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure);
}
void Pulse_output(uint16_t Cycle,uint16_t PulseNum)
{
    TIM2_config(PulseNum);
    TIM_Cmd(TIM2, ENABLE);
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    TIM1_config(Cycle);
    
    TIM_Cmd(ADVANCE_TIM, ENABLE);
    TIM_CtrlPWMOutputs(ADVANCE_TIM, ENABLE);   //高级定时器一定要加上，主输出使能
}



