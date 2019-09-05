#include "bsp_general_Tim.h"
#include "bsp_usart.h"

TIM_ICUserValueType TIM_ICUserValue[8];

 // 中断优先级配置
static void GENERAL_TIM_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
#if USE_TIM3
	// 设置中断组为 1
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn ;
	// 设置主优先级为 1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	// 设置抢占优先级为 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//中断通道使能
	NVIC_Init(&NVIC_InitStructure);
#endif

#if USE_TIM4
		// 设置中断组为 1
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn ;
	// 设置主优先级为 1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	// 设置抢占优先级为 4
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//中断通道使能
	NVIC_Init(&NVIC_InitStructure);
#endif
}
//////////////////TIM3///////////////////////////////
#if USE_TIM3
static void TIM3_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(TIM3_GPIO_CLK,ENABLE);//开启对应GPIO的时钟
#if USE_TIM3_CH1
	//配置CH1 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStructure.GPIO_Pin = TIM3_CH1_PIN;
	GPIO_Init(TIM3_CH1_PORT,&GPIO_InitStructure);
#endif
	
#if USE_TIM3_CH2
	//配置CH2 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = TIM3_CH2_PIN;
	GPIO_Init(TIM3_CH2_PORT,&GPIO_InitStructure);
#endif
	
#if USE_TIM3_CH3
	//配置CH3 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = TIM3_CH3_PIN;
	GPIO_Init(TIM3_CH3_PORT,&GPIO_InitStructure);
#endif

#if USE_TIM3_CH4
	//配置CH4 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = TIM3_CH4_PIN;
	GPIO_Init(TIM3_CH4_PORT,&GPIO_InitStructure);
#endif
}

static void GENERAL_TIM3_Config(void)
{
	//配置引脚
	TIM3_GPIO_Config();
	//配置时基
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	//初始化TIM3时钟72MHz
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;	//分频 72M/72 = 1MHz
	TIM_TimeBaseInitStructure.TIM_Period = 0xffff-1;  //计时周期65536  计时器的一个周期是(1s/1M)*65536 =  0.0656s即65ms
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//配置为1分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//配置为向上计数模式
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; //重复计数器，TIM3没有
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);  //配置TIM3
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);               //清除中断标志位
}

static void TIM3_IC_Config(void)
{
	TIM_ICInitTypeDef TIM_ICInitStructure;
#if USE_TIM3_CH1
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;				//配置捕获通道,!!!!"这个参数不支持多参数相或"!!!!!
	TIM_ICInitStructure.TIM_ICFilter = 0x0f;								//设置滤波,参考手册p215
	TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM_STRAT_ICPolarity;//选择IC触发边沿,上升沿还是下降沿
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			//预分频数1
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//直接输入
	TIM_ICInit(TIM3, &TIM_ICInitStructure);									//配置TIM3的输入捕获功能
#endif
#if USE_TIM3_CH2
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;	
	TIM_ICInitStructure.TIM_ICFilter = 0x0f;
	TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
#endif
#if USE_TIM3_CH3
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
	TIM_ICInitStructure.TIM_ICFilter = 0x0f;
	TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
#endif
#if USE_TIM3_CH4
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
	TIM_ICInitStructure.TIM_ICFilter = 0x0f;
	TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
#endif
// 开启更新和捕获中断
	TIM_ITConfig (TIM3, TIM_IT_Update | TIM3_IT_CCx, ENABLE );
	TIM_ClearFlag(TIM3, TIM_FLAG_Update|TIM3_IT_CCx);//清中断标志
// 使能计数器
	TIM_Cmd(TIM3, ENABLE);
}
#endif
/////////////////////TIM4////////////////////////////////
#if USE_TIM4
static void TIM4_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(TIM4_GPIO_CLK,ENABLE);
#if USE_TIM4_CH1
	//配置CH1 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = TIM4_CH1_PIN;
	GPIO_Init(TIM4_CH1_PORT,&GPIO_InitStructure);
#endif
	
#if USE_TIM4_CH2
	//配置CH2 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = TIM4_CH2_PIN;
	GPIO_Init(TIM4_CH2_PORT,&GPIO_InitStructure);
#endif
	
#if USE_TIM4_CH3
	//配置CH3 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = TIM4_CH3_PIN;
	GPIO_Init(TIM4_CH3_PORT,&GPIO_InitStructure);
#endif

#if USE_TIM4_CH4
	//配置CH4 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = TIM4_CH4_PIN;
	GPIO_Init(TIM4_CH4_PORT,&GPIO_InitStructure);
#endif
}

static void GENERAL_TIM4_Config(void)
{
	//配置引脚
	TIM4_GPIO_Config();
	//配置时基
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;
	TIM_TimeBaseInitStructure.TIM_Period = 0xffff;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
}

static void TIM4_IC_Config(void)
{
	TIM_ICInitTypeDef TIM_ICInitStructure;
#if USE_TIM4_CH1
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0f;
	TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
#endif
#if USE_TIM4_CH2
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0x0f;
	TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
#endif
#if USE_TIM4_CH3
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
	TIM_ICInitStructure.TIM_ICFilter = 0x0f;
	TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
#endif
#if USE_TIM4_CH4
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
	TIM_ICInitStructure.TIM_ICFilter = 0x0f;
	TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
#endif
// 开启更新和捕获中断
	TIM_ITConfig (TIM4, TIM_IT_Update | TIM4_IT_CCx, ENABLE );
	TIM_ClearFlag(TIM4, TIM_FLAG_Update|TIM4_IT_CCx);
// 使能计数器
	TIM_Cmd(TIM4, ENABLE);
}
#endif

void TIM_Init(void)
{
	GENERAL_TIM_NVIC_Config();
	
#if USE_TIM3
	GENERAL_TIM3_Config();
	TIM3_IC_Config();
#endif
	
#if USE_TIM4
	GENERAL_TIM4_Config();
	TIM4_IC_Config();
#endif	
}
//对捕获的结果进行处理
void PWM_IC_Print(void)
{
	double fre;
	// TIM 计数器的驱动时钟
	uint32_t TIM_PscCLK = 72000000 / (71+1);
	int i = 0;
	for(i = 0;i<8;i++)
	{
		if(TIM_ICUserValue[i].Capture_FinishFlag == 1) 
		{
			// 计算一个周期的计数器的值
			fre = (TIM_ICUserValue[i].Capture_CcrValue+1);
			if(fre>10000||fre<500)
				continue;
			// 打印频率
			printf ( "\r\n %d号输入测得频率: %f hz\r\n",i,1/(fre/TIM_PscCLK));
			
			TIM_ICUserValue[i].Capture_FinishFlag = 0;
		}
	}
	
}
