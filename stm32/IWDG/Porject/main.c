#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_key.h"

void IWDG_Config(uint8_t prv,uint16_t rlv);
void IWDG_Feed(void);

void delay(unsigned int n)
{
	while(n--);
}


int main(void)
{
	LED_Configer();
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{
		LED_R(ON);
		RCC_ClearFlag();
	}
	else
		LED_B(ON);
	
	delay(0xfffff);
	Key_Configer();
	//ι��ʱ���Լ1S
	IWDG_Config(IWDG_Prescaler_64,625);
	while(1)
	{
		//����KEY1ι��
		if(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN))
		{
			LED_G(ON);
			LED_R(OFF);
			IWDG_Feed();
		}
		if(Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN))
			LED_R_TOGGLE;

	}

}

//IWDG���ú���Tout = prv/40 * rlv (s)
void IWDG_Config(uint8_t prv,uint16_t rlv)
{
	//ʹ��PR��RLR��д
	IWDG_WriteAccessCmd(ENABLE);
	//����Ԥ��Ƶ��ֵ
	IWDG_SetPrescaler(prv);
	//������װ��ֵ
	IWDG_SetReload(rlv);
	//����װֵ�����������
	IWDG_ReloadCounter();
	//ʹ�ܶ������Ź�
	IWDG_Enable();
}

//ι������
void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}

