#include "bsp_exti.h"


//配置NVIC
static void NVIC_Configer(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//优先级组在USART初始化时设置过了
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	//配置抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	//使能中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	//配置中断源1
	NVIC_InitStructure.NVIC_IRQChannel = FRE1_EXTI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	//配置中断源2
	NVIC_InitStructure.NVIC_IRQChannel = FRE2_4_EXTI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	//配置中断源3
	NVIC_InitStructure.NVIC_IRQChannel = FRE5_8_EXTI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStructure);
}

static void Fre_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//开启按键端口时钟
	RCC_APB2PeriphClockCmd(FRE_GPIO_CLK,ENABLE);
	//开AFIO时钟
	RCC_APB2PeriphClockCmd(EXTI_AFIO_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//下拉输入
	
	//配置Fre1~4
	GPIO_InitStructure.GPIO_Pin = FRE1_EXTI_PIN | FRE2_EXTI_PIN | FRE3_EXTI_PIN | FRE4_EXTI_PIN;
	GPIO_Init(FRE1_4_EXTI_PORT,&GPIO_InitStructure);
	//配置Fre5~8
	GPIO_InitStructure.GPIO_Pin = FRE5_EXTI_PIN | FRE6_EXTI_PIN | FRE7_EXTI_PIN | FRE8_EXTI_PIN;
	GPIO_Init(FRE5_8_EXTI_PORT,&GPIO_InitStructure);
	
	//配置EXTI信号源
	GPIO_EXTILineConfig(Fre1_EXTI_PortSource,Fre1_EXTI_PinSource);
	GPIO_EXTILineConfig(Fre2_EXTI_PortSource,Fre2_EXTI_PinSource);
	GPIO_EXTILineConfig(Fre3_EXTI_PortSource,Fre3_EXTI_PinSource);
	GPIO_EXTILineConfig(Fre4_EXTI_PortSource,Fre4_EXTI_PinSource);
	GPIO_EXTILineConfig(Fre5_EXTI_PortSource,Fre5_EXTI_PinSource);
	GPIO_EXTILineConfig(Fre6_EXTI_PortSource,Fre6_EXTI_PinSource);
	GPIO_EXTILineConfig(Fre7_EXTI_PortSource,Fre7_EXTI_PinSource);
	GPIO_EXTILineConfig(Fre8_EXTI_PortSource,Fre8_EXTI_PinSource);
}

//初始化EXTI
void Fre_EXTI_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	//配置NVIC
	NVIC_Configer();
	//配置GPIO
	Fre_GPIO_Init();

	//中断模式
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	//上升沿中断
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	//使能中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	//选择EXTI信号源
	EXTI_InitStructure.EXTI_Line = Fre1_INT_EXTI_Line;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line = Fre2_INT_EXTI_Line;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line = Fre3_INT_EXTI_Line;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line = Fre4_INT_EXTI_Line;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line = Fre5_INT_EXTI_Line;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line = Fre6_INT_EXTI_Line;
	EXTI_Init(&EXTI_InitStructure);
  EXTI_InitStructure.EXTI_Line = Fre7_INT_EXTI_Line;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line = Fre8_INT_EXTI_Line;
	EXTI_Init(&EXTI_InitStructure);
	
}









