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
	printf("����\n");
	delay(0xffff);
	//printf("����˯��ģʽ\n");
	//__WFI();
	//printf("����ֹͣģʽ\n");
	//����ֹͣģʽ,�жϻ���
	//PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);
	//SYSCLKConfig_STOP();
	//Enter_StandBy_Mode();
	printf("������������\n");
	LED_G(OFF);
	while(1)
	{
		//__WFI();
	}
}
//ͣ�����Ѻ�����ϵͳʱ�� ʹ��HSE PLL��ѡ��PLL��Ϊϵͳʱ��
static void SYSCLKConfig_STOP(void)
{
	RCC_HSEConfig(RCC_HSE_ON);
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource() != 0X08);
}
//�������ģʽ
static void Enter_StandBy_Mode(void)
{
	PWR_ClearFlag(PWR_FLAG_WU);
	PWR_WakeUpPinCmd(ENABLE);
	PWR_EnterSTANDBYMode();
}	
