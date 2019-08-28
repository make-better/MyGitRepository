#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_exti.h"
extern uint16_t temp;

static void SYSCLKConfig_STOP(void);
static void Enter_StandBy_Mode(void);
void delay(uint32_t n)
{
	while(n--);
}
int main(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	USART_Config();
	EXTI_Configer();
	LED_Configer();
	LED_G(ON);
	printf("开机\n");
	delay(0xffff);
	//printf("进入睡眠模式\n");
	//__WFI();
	//printf("进入停止模式\n");
	//进入停止模式,中断唤醒
	//PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);
	//SYSCLKConfig_STOP();
	//Enter_StandBy_Mode();
	printf("又正常运行了\n");
	LED_G(OFF);
	while(1)
	{
		//__WFI();
	}
}
//停机唤醒后配置系统时钟 使能HSE PLL，选择PLL作为系统时钟
static void SYSCLKConfig_STOP(void)
{
	RCC_HSEConfig(RCC_HSE_ON);
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource() != 0X08);
}
//进入待机模式
static void Enter_StandBy_Mode(void)
{
	PWR_ClearFlag(PWR_FLAG_WU);
	PWR_WakeUpPinCmd(ENABLE);
	PWR_EnterSTANDBYMode();
}	
