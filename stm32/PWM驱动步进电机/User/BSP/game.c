#include "game.h"
#include "bsp_ili_lcd.h"
#include "bsp_gpio.h"
#include "bsp_xpt2046.h"
#include "bsp_usart.h"
#include <string.h>
#include <stdio.h>
/*��ť�ṹ������*/
Touch_Button button[BUTTON_NUM];
uint8_t unit=1;               //��ʾ����
uint8_t Display=0;						//�жϺ���ʾ��־λ
//����ʱ����
void delay(unsigned int n)
{
	while(n--);
}

//���ư�ť
static void Draw_Button(void *btn);
//��ť����
static void Command_Button(void *btn);


void Screan_Init(uint8_t LCD_Mode)
{
	uint8_t i=0;
	ILI9341_GramScan ( LCD_Mode );
  /* ������Ϊ�� */
	LCD_SetBackColor(CL_BLACK);
  ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	    
}


/**
* @brief  Touch_Button_Down ����������ʱ���õĺ������ɴ���������
* @param  x ����λ�õ�x����
* @param  y ����λ�õ�y����
* @retval ��
*/
void Touch_Button_Down(uint16_t x,uint16_t y)
{
  uint8_t i;
  for(i=0;i<BUTTON_NUM;i++)
  {
    /* �������˰�ť */
    if(x<=button[i].end_x && y<=button[i].end_y && y>=button[i].start_y && x>=button[i].start_x )
    {
      if(button[i].touch_flag == 0)     /*ԭ����״̬Ϊû�а��£������״̬*/
      {
      button[i].touch_flag = 1;         /* ��¼���±�־ */
      
      button[i].draw_btn(&button[i]);  /*�ػ水ť*/
      }        
      
    }
    else if(button[i].touch_flag == 1) /* �����Ƴ��˰����ķ�Χ��֮ǰ�а��°�ť */
    {
      button[i].touch_flag = 0;         /* ������±�־���ж�Ϊ�����*/
      
      button[i].draw_btn(&button[i]);   /*�ػ水ť*/
    }
  }

}

/**
* @brief  Touch_Button_Up �������ͷ�ʱ���õĺ������ɴ���������
* @param  x ��������ͷ�ʱ��x����
* @param  y ��������ͷ�ʱ��y����
* @retval ��
*/
void Touch_Button_Up(uint16_t x,uint16_t y)
{
   uint8_t i; 
   for(i=0;i<BUTTON_NUM;i++)
   {
     /* �����ڰ�ť�����ͷ� */
      if((x<button[i].end_x && x>button[i].start_x && y<button[i].end_y && y>button[i].start_y))
      {        
				
        button[i].touch_flag = 0;       /*�ͷŴ�����־*/
      
        button[i].btn_command(&button[i]);  /*ִ�а����Ĺ�������*/
        
        break;
      }
    }  
}

static void Draw_Button(void *btn)
{
  Touch_Button *ptr = (Touch_Button *)btn;
	
  /*�ͷŰ���*/
  if(ptr->touch_flag == 0)
  {
		LCD_SetColors(CL_BUTTON_GREY,CL_WHITE);
    ILI9341_DrawRectangle(	ptr->start_x,
															ptr->start_y,
															ptr->end_x - ptr->start_x,
															ptr->end_y - ptr->start_y,1);
    

		
		LCD_SetColors(CL_RED,CL_BUTTON_GREY);
		/*ѡ�����壬ʹ����Ӣ����ʾʱ��������Ӣ��ѡ���8*16�����壬
		*���������С��16*16�ģ���Ҫ��������������������ģ*/
		/*�������ֻ��Ӣ������������*/
		LCD_SetFont(&Font8x16);
    ILI9341_DispString_EN_CH( ptr->start_x+4,
																ptr->start_y+ ((ptr->end_y - ptr->start_y-16)/2),	
																ptr->name);
  }
  else  /*��������*/
  {
		LCD_SetColors(CL_BUTTON_GREY,CL_BUTTON_GREY);
    ILI9341_DrawRectangle(ptr->start_x,
														ptr->start_y,
														ptr->end_x - ptr->start_x,
														ptr->end_y - ptr->start_y,1);
		LCD_SetColors(CL_BLACK,CL_BUTTON_GREY);
		/*ѡ�����壬ʹ����Ӣ����ʾʱ��������Ӣ��ѡ���8*16�����壬
		*���������С��16*16�ģ���Ҫ��������������������ģ*/
		/*�������ֻ��Ӣ������������*/
		LCD_SetFont(&Font8x16);
    ILI9341_DispString_EN_CH( ptr->start_x+4,          	           
																ptr->start_y+ ((ptr->end_y - ptr->start_y-16)/2),	
																ptr->name);
  } 
  
     /*��ť�߿�*/
		LCD_SetColors(CL_BLUE4,CL_WHITE);
    ILI9341_DrawRectangle(ptr->start_x,
														ptr->start_y,
														ptr->end_x - ptr->start_x,
														ptr->end_y - ptr->start_y,0);
}


//��������
static void Command_Button(void *btn)
{
	Touch_Button *ptr = (Touch_Button *)btn;
	switch(ptr->value)
	{	
		case 1://���� 
		{
					unit = 1;
		}break;
		case 2://ֹͣ���
		{
			
			TIM_CtrlPWMOutputs(ADVANCE_TIM, DISABLE);  //�����ʧ��
      TIM_Cmd(ADVANCE_TIM, DISABLE); // �رն�ʱ�� 
      TIM_Cmd(TIM2, DISABLE); // �رն�ʱ�� 
		}break;
		case 3://���е��1 17�ȣ�2�Ŀ�ʼ��
		{
			unit = 3;														//��ת����һ������
			Moto_Contral(1,17);								//�������
		}break;
		case 4://���е��0 12�ȣ�4�ķ��أ� 
		{
			unit = 5;														//��ת����һ������
			Moto_Contral(0,12);								//�������
		}break;
		case 5://���е��1 11�ȣ�4�ļ����� 
		{
			unit = 6;														//��ת����һ������
			Moto_Contral(1,11);								//�������
		}break;
		default:
		{
			unit++;													//������һ�����棬�����������水ť���õ��������
			LCD_SetBackColor(CL_BLACK);
			ILI9341_Clear(0,0,240,320);	//����
		}break;
	}
	LCD_SetBackColor(CL_BLACK);
	ILI9341_Clear(0,0,240,320);	//����
	//�ť��ʧ
	button[1].start_x = 0;
	button[1].start_y = 0;
	button[1].end_x = 0;
	button[1].end_y = 0;
	button[1].touch_flag = 0;
	button[1].value = 2;
	
	button[2].start_x = 0;
	button[2].start_y = 0;
	button[2].end_x = 0;
	button[2].end_y = 0;
	button[2].touch_flag = 0;
	button[2].value = 2;
} 
//��Ҫ��ʾ����
void Display_Space(void)
{
	switch(unit)
	{
		case 1:
		{
			button[0].start_x = 80;
			button[0].start_y = 140;
			button[0].end_x = 160 ;
			button[0].end_y = 180;
			button[0].value = 0;
			button[0].touch_flag = 0;  
			sprintf(button[0].name,"  ������");
			button[0].draw_btn = Draw_Button ;
			button[0].btn_command = Command_Button ;
			button[0].draw_btn(&button[0]);  //�����������桤α
		}break;
		case 2:
		{
			LCD_SetColors(CL_GREEN,CL_BLACK);
			ILI9341_DispString_EN_CH(6*16,LINE(7),"������0��");
			
			/***���ð�ť0***/
			button[0].start_x = 240-40;
			button[0].start_y = 320-16;
			button[0].end_x = 240 ;
			button[0].end_y = 320;
			button[0].value = 3;
			button[0].touch_flag = 0;  
			sprintf(button[0].name,"��ʼ");
			button[0].draw_btn(&button[0]);  //������ť"��ʼ"
			/***���ð�ť1***/
			button[1].start_x = 0;
			button[1].start_y = 320-16;
			button[1].end_x = 40 ;
			button[1].end_y = 320;
			button[1].value = 1;
			button[1].touch_flag = 0;  
			sprintf(button[1].name,"����");
			button[1].draw_btn = Draw_Button ;
			button[1].btn_command = Command_Button ;
			button[1].draw_btn(&button[1]);  //������ť"����"
		}break;
		case 3:
		{
			LCD_SetColors(CL_RED,CL_BLACK);
			ILI9341_DispString_EN_CH(6*16,LINE(7),"������17��");
			/***���ð�ť1***/
			button[1].start_x = 0;
			button[1].start_y = 320-16;
			button[1].end_x = 40 ;
			button[1].end_y = 320;
			button[1].value = 2;
			button[1].touch_flag = 0;  
			sprintf(button[1].name,"ֹͣ");
			button[1].draw_btn(&button[1]);  //������ť"ֹͣ"
		}break;
		case 4:
		{
			LCD_SetColors(CL_GREEN,CL_BLACK);
			ILI9341_DispString_EN_CH(6*16,LINE(7),"������17��");
			/***���ð�ť0***/
			button[0].start_x = 240-40;
			button[0].start_y = 320-16;
			button[0].end_x = 240 ;
			button[0].end_y = 320;
			button[0].value = 4;
			button[0].touch_flag = 0;  
			sprintf(button[0].name,"����");
			button[0].draw_btn(&button[0]);  //������ť"��ʼ"
			/***���ð�ť1***/
			button[1].start_x = 0;
			button[1].start_y = 320-16;
			button[1].end_x = 40 ;
			button[1].end_y = 320;
			button[1].value = 5;
			button[1].touch_flag = 0;  
			sprintf(button[1].name,"����");
			button[1].draw_btn(&button[1]);  //������ť"����"
		}break;
		case 5:
		{
			LCD_SetColors(CL_RED,CL_BLACK);
			ILI9341_DispString_EN_CH(6*16,LINE(7),"������5��");
		}break;
		case 6:
		{
			LCD_SetColors(CL_RED,CL_BLACK);
			ILI9341_DispString_EN_CH(6*16,LINE(7),"������28��");
			/***���ð�ť1***/
			button[1].start_x = 0;
			button[1].start_y = 320-16;
			button[1].end_x = 40 ;
			button[1].end_y = 320;
			button[1].value = 2;
			button[1].touch_flag = 0;  
			sprintf(button[1].name,"ֹͣ");
			button[1].draw_btn(&button[1]);  //������ť"ֹͣ"
		}break;
		default:break;
	}
}
//���ת�����ƺ���
//������ת������(1:+ 0:-)��ת���Ƕ�
void Moto_Contral(uint8_t direction,uint16_t angle)
{
	uint16_t PulseNum=0;
	PulseNum = angle/(ISA/MS);
	if(direction>0)
		GPIO_SetBits(SDC_GPIO_PORT,SDC_DIR_GPIO_Pin);
	else
		GPIO_ResetBits(SDC_GPIO_PORT,SDC_DIR_GPIO_Pin);;
	Pulse_output(5000,PulseNum);//200hz
}

