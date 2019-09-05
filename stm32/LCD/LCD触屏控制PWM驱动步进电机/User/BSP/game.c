#include "game.h"
#include "bsp_ili_lcd.h"
#include "bsp_xpt2046.h"
#include "bsp_usart.h"
#include <string.h>
#include <stdio.h>
/*��ť�ṹ������*/
Touch_Button button[BUTTON_NUM];

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
  /*���ذ�ť*/
  button[0].start_x = 80;
  button[0].start_y = 40;
  button[0].end_x = 160 ;
  button[0].end_y = 80;
	button[0].value = 1;
  button[0].touch_flag = 0;  
	sprintf(button[0].name,"����");
  button[0].draw_btn = Draw_Button ;
  button[0].btn_command = Command_Button ;
	/*���򰴼�*/
	button[1].start_x = 80;
  button[1].start_y = 120;
  button[1].end_x = 160 ;
  button[1].end_y = 160;
	button[1].value = 2;
  button[1].touch_flag = 0;  
	sprintf(button[1].name,"����");
  button[1].draw_btn = Draw_Button ;
  button[1].btn_command = Command_Button ;
	/*����ת������*/
	button[2].start_x = 80;
  button[2].start_y = 200;
  button[2].end_x = 160 ;
  button[2].end_y = 240;
	button[2].value = 3;
  button[2].touch_flag = 0;
	sprintf(button[2].name,"�ѻ�");
  button[2].draw_btn = Draw_Button ;
  button[2].btn_command = Command_Button ;
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
    ILI9341_DispString_EN_CH( ptr->start_x+20,
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
    ILI9341_DispString_EN_CH( ptr->start_x+20,          
//																ptr->start_y+15,	           
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
		case 1:onoff=~onoff;
			break;
		case 2:dir=~dir;
			break;
		case 3:en=~en;
			break;
		default:break;
	}
} 


