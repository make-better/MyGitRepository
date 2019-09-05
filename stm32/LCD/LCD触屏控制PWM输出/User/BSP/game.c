#include "game.h"
#include "bsp_ili_lcd.h"
#include "bsp_xpt2046.h"
#include "bsp_usart.h"
#include "bsp_advance_tim.h"
#include <string.h>
#include <stdio.h>
/*��ť�ṹ������*/
Touch_Button button[BUTTON_NUM];

//����ʱ����
void delay(unsigned int n)
{
	while(n--);
}

//����Ƶ�ʰ�ť
static void Draw_Frequency_Button(void *btn);
//����ռ�ձȰ�ť
static void Draw_Duty_Cycle_Button(void *btn);
//Ƶ�ʰ�ť����
static void Command_Frequency_Button(void *btn);
//ռ�ձȰ�ť����
static void Command_Duty_Cycle_Button(void *btn);




void Screan_Init(uint8_t LCD_Mode)
{
	uint8_t i=0;
	ILI9341_GramScan ( LCD_Mode );
  /* ������Ϊ�� */
	LCD_SetBackColor(CL_BLACK);
  ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	
  /* ��ʼ����ť */
  Touch_Button_Init();
  /* ���������ť */
	for(i=0;i<BUTTON_NUM;i++)
	{
		button[i].draw_btn(&button[i]);
	}
    
}

void Touch_Button_Init(void)
{
  /*������ť*/
  button[0].start_x = 30;
  button[0].start_y = 30;
  button[0].end_x = 30+7*8+4 ;
  button[0].end_y = 30+COLOR_BLOCK_HEIGHT;
	button[0].value = 8000-1;
  button[0].touch_flag = 0;  
  button[0].draw_btn = Draw_Frequency_Button ;
  button[0].btn_command = Command_Frequency_Button ;
	/*ѡ�񰴼�*/
	button[1].start_x = 200;
  button[1].start_y = 30;
  button[1].end_x = 200+7*8+4 ;
  button[1].end_y = 30+COLOR_BLOCK_HEIGHT;
	button[1].value = 800-1;
  button[1].touch_flag = 0;  
  button[1].draw_btn = Draw_Frequency_Button ;
  button[1].btn_command = Command_Frequency_Button ;
	
	button[2].start_x = 30;
  button[2].start_y = 135;
  button[2].end_x = 30+11*8+4;
  button[2].end_y = 135+COLOR_BLOCK_HEIGHT;
	button[2].value = 5;
  button[2].touch_flag = 0;  
  button[2].draw_btn = Draw_Duty_Cycle_Button ;
  button[2].btn_command = Command_Duty_Cycle_Button ;
	
	button[3].start_x = 200;
  button[3].start_y = 135;
  button[3].end_x = 200+11*8+4 ;
  button[3].end_y = 135+COLOR_BLOCK_HEIGHT;
	button[3].value = 2;
  button[3].touch_flag = 0;  
  button[3].draw_btn = Draw_Duty_Cycle_Button ;
  button[3].btn_command = Command_Duty_Cycle_Button ;
  
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

        button[i].draw_btn(&button[i]); /*�ػ水ť*/        
      
        button[i].btn_command(&button[i]);  /*ִ�а����Ĺ�������*/
        
        break;
      }
    }  
}

static void Draw_Frequency_Button(void *btn)
{
	char string[10];
  Touch_Button *ptr = (Touch_Button *)btn;
    
	sprintf(string,"%dHz",8000000/(ptr->value+1));
	
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
																string);
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
//																ptr->start_y+15,	           
																ptr->start_y+ ((ptr->end_y - ptr->start_y-16)/2),	
																string);
  } 
  
     /*��ť�߿�*/
		LCD_SetColors(CL_BLUE4,CL_WHITE);
    ILI9341_DrawRectangle(ptr->start_x,
														ptr->start_y,
														ptr->end_x - ptr->start_x,
														ptr->end_y - ptr->start_y,0);
}


//Ƶ�ʰ�������
static void Command_Frequency_Button(void *btn)
{
	Touch_Button *ptr = (Touch_Button *)btn;
	frequency = 8000000/(ptr->value+1);
	TIM_TimeBaseStructure.TIM_Period = ptr->value;
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);
	TIM_OCInitStructure.TIM_Pulse = (TIM_TimeBaseStructure.TIM_Period+1)*(duty_cycle)/10;
	TIM_OC1Init(ADVANCE_TIM, &TIM_OCInitStructure);
	TIM_Cmd(ADVANCE_TIM, ENABLE);
} 

//����ռ�ձȰ�ť
static void Draw_Duty_Cycle_Button(void *btn)
{
	char string[10];
  Touch_Button *ptr = (Touch_Button *)btn;
    
	sprintf(string,"ռ�ձ�:%%%d ",ptr->value*10);
	
  /*�ͷŰ���*/
  if(ptr->touch_flag == 0)
  {
		LCD_SetColors(CL_BUTTON_GREY,CL_WHITE);
    ILI9341_DrawRectangle(	ptr->start_x,
															ptr->start_y,
															ptr->end_x - ptr->start_x,
															ptr->end_y - ptr->start_y,1);
 
		LCD_SetColors(CL_BLUE1,CL_BUTTON_GREY);
		/*ѡ�����壬ʹ����Ӣ����ʾʱ��������Ӣ��ѡ���8*16�����壬
		*���������С��16*16�ģ���Ҫ��������������������ģ*/
		/*�������ֻ��Ӣ������������*/
		LCD_SetFont(&Font8x16);
    ILI9341_DispString_EN_CH( ptr->start_x+4,
																ptr->start_y+ ((ptr->end_y - ptr->start_y-16)/2),	
																string);
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
//																ptr->start_y+15,	           
																ptr->start_y+ ((ptr->end_y - ptr->start_y-16)/2),	
																string);
  } 

     /*��ť�߿�*/
		LCD_SetColors(CL_BLUE4,CL_WHITE);
    ILI9341_DrawRectangle(ptr->start_x,
														ptr->start_y,
														ptr->end_x - ptr->start_x,
														ptr->end_y - ptr->start_y,0);
}

//ռ�ձȰ�ť����
static void Command_Duty_Cycle_Button(void *btn)
{
	Touch_Button *ptr = (Touch_Button *)btn;
	duty_cycle = (ptr->value);
	TIM_OCInitStructure.TIM_Pulse = (TIM_TimeBaseStructure.TIM_Period+1)*(ptr->value)/10;
	TIM_OC1Init(ADVANCE_TIM, &TIM_OCInitStructure);
}
