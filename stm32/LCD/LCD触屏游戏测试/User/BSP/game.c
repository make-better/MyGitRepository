#include "game.h"
#include "bsp_ili_lcd.h"
#include "bsp_xpt2046.h"
#include "bsp_usart.h"
#include <string.h>
#include <stdio.h>
/*��ť�ṹ������*/
Touch_Button button[BUTTON_NUM];

uint8_t unit=0;              //�½ڿ���

//����ʱ����
void delay(unsigned int n)
{
	while(n--);
}

//����������ť
static void Draw_Restar_Button(void *btn);
//����ѡ�����
static void Draw_Word_Button(void *btn);
//������ť����
static void Command_Select_Restart(void *btn);
//���ѡ�񰴼�����
static void Command_Select_Sentence(void *btn);



void Screan_Init(uint8_t LCD_Mode)
{
	ILI9341_GramScan ( LCD_Mode );
	
  /* ������Ϊ�� */
	LCD_SetBackColor(CL_BLACK);
  ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	

  
  /* ��ʼ����ť */
  Touch_Button_Init();
  
  /* ���������ť */
  button[0].draw_btn(&button[0]);  
}

void Touch_Button_Init(void)
{
  /*������ť*/
  button[0].start_x = BUTTON_START_X;
  button[0].start_y = 0;
  button[0].end_x = BUTTON_START_X+COLOR_BLOCK_WIDTH ;
  button[0].end_y = COLOR_BLOCK_HEIGHT;
  button[0].touch_flag = 0;  
  button[0].draw_btn = Draw_Restar_Button ;
  button[0].btn_command = Command_Select_Restart ;
	/*ѡ�񰴼�*/
	button[1].start_x = 0;
  button[1].start_y = 0;
  button[1].end_x = 0 ;
  button[1].end_y = 0;
  button[1].touch_flag = 0;  
  button[1].draw_btn = Draw_Word_Button ;
  button[1].btn_command = Command_Select_Sentence ;
	
	button[2].start_x = 0;
  button[2].start_y = 0;
  button[2].end_x = 0 ;
  button[2].end_y = 0;
  button[2].touch_flag = 0;  
  button[2].draw_btn = Draw_Word_Button ;
  button[2].btn_command = Command_Select_Sentence ;
  
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
//���ѡ�񰴼�����
static void Command_Select_Sentence(void *btn)
{
	Touch_Button *ptr = (Touch_Button *)btn;

	unit = ptr->towhere;
	
	//ʹ������ʧ
	button[1].start_x = 0;
	button[1].start_y = 0;
	button[1].end_x = 0;
	button[1].end_y = 0;
	button[1].touch_flag = 0;
	
	button[2].start_x = 0;
	button[2].start_y = 0;
	button[2].end_x = 0;
	button[2].end_y = 0;
	button[2].touch_flag = 0;
}
static void Draw_Restar_Button(void *btn)
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
    ILI9341_DispString_EN_CH( ptr->start_x + (ptr->end_x - ptr->start_x - 16*2 )/2,
																ptr->start_y+ ((ptr->end_y - ptr->start_y-16)/2),	
																"����");
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
    ILI9341_DispString_EN_CH( ptr->start_x + (ptr->end_x - ptr->start_x - 16*2 )/2,          
//																ptr->start_y+15,	           
																ptr->start_y+ ((ptr->end_y - ptr->start_y-16)/2),	
																"����");
  } 
  
     /*��ť�߿�*/
		LCD_SetColors(CL_BLUE4,CL_WHITE);
    ILI9341_DrawRectangle(ptr->start_x,
														ptr->start_y,
														ptr->end_x - ptr->start_x,
														ptr->end_y - ptr->start_y,0);
}
//�����ֵİ�ť
static void Draw_Word_Button(void *btn)
{
  Touch_Button *ptr = (Touch_Button *)btn;
  /*�ͷŰ���*/
  if(ptr->touch_flag == 0)
  {	
//			LCD_SetColors(CL_BUTTON_GREY,CL_WHITE);
//    ILI9341_DrawRectangle(	ptr->start_x-1,
//															ptr->start_y-1,
//															ptr->end_x - ptr->start_x,
//															ptr->end_y - ptr->start_y,1);
		LCD_SetColors(CL_WHITE,CL_BLACK);
		/*ѡ�����壬ʹ����Ӣ����ʾʱ��������Ӣ��ѡ���8*16�����壬
		*���������С��16*16�ģ���Ҫ��������������������ģ*/
		/*�������ֻ��Ӣ������������*/
		LCD_SetFont(&Font8x16);
    ILI9341_DispString_EN_CH( ptr->start_x ,
																ptr->start_y+ ((ptr->end_y - ptr->start_y-16)/2),	
																ptr->word);
	
  }
  else  /*��������*/
  {
		LCD_SetColors(CL_RED,CL_BLACK);
		/*ѡ�����壬ʹ����Ӣ����ʾʱ��������Ӣ��ѡ���8*16�����壬
		*���������С��16*16�ģ���Ҫ��������������������ģ*/
		/*�������ֻ��Ӣ������������*/
		LCD_SetFont(&Font8x16);
    ILI9341_DispString_EN_CH( ptr->start_x ,                     
																ptr->start_y+ ((ptr->end_y - ptr->start_y-16)/2),	
																ptr->word);
		delay(0xfffff);
  }   
}

//������������
static void Command_Select_Restart(void *btn)
{
	LCD_SetColors(CL_WHITE,CL_BLACK);
	ILI9341_Clear(0,0,ILI9341_MORE_PIXEL,ILI9341_LESS_PIXEL);
	unit = 0;
	Game_Space();
} 




//ѡ��������ú���
void Sentence_Config(char *str,uint8_t num,uint8_t line,uint8_t to)
{
	uint8_t i=0;
	sprintf(button[num].word,(const char*)str);
	//i = sizeof(*str);
	for(i=0;button[num].word[i]!='\0';i++);
	button[num].start_x = (320-i*8)/2;
	button[num].start_y = LINE(line);
	button[num].end_x = button[num].start_x+i*8;
	button[num].end_y = LINE(line+1);
	button[num].touch_flag = 0;
	button[num].towhere = to;
	button[num].draw_btn = Draw_Word_Button;
	button[num].btn_command = Command_Select_Sentence;
}


void Game_Space(void)
{
	switch(unit)
	{
		case 0:
		{
			/*����*/
			LCD_SetColors(CL_WHITE,CL_BLACK);
			ILI9341_Clear(40,0,ILI9341_MORE_PIXEL,ILI9341_LESS_PIXEL);
			LCD_SetFont(&Font8x16);
			ILI9341_DispString_EN_CH(12*8,                  
																	LINE(1),	
																	"*ĳ�˵�����ð��*");
			Sentence_Config("��ʼ",1,7,1);
			button[1].draw_btn(&button[1]);  //����������
		}
			break;
		case 1:
		{
			/*����*/
			LCD_SetColors(CL_WHITE,CL_BLACK);
			ILI9341_Clear(40,0,ILI9341_MORE_PIXEL,ILI9341_LESS_PIXEL);
			LCD_SetFont(&Font8x16);
			ILI9341_DispString_EN_CH(11*8,                  
																	LINE(1),	
																	"���갡��ȥ�����漣��!");
		}
			break;
		case 2:
		{
			
			Sentence_Config("1.ȥ",1,5,6);		
			button[1].draw_btn(&button[1]);  //��������1
				
			Sentence_Config("2.��ȥ",2,7,3);
			button[2].draw_btn(&button[2]);  //��������2
			
		}
			break;
		case 3:
		{
			/*����*/
			LCD_SetColors(CL_WHITE,CL_BLACK);
			ILI9341_Clear(40,0,ILI9341_MORE_PIXEL,ILI9341_LESS_PIXEL);
			LCD_SetFont(&Font8x16);
			ILI9341_DispString_EN_CH(12*8,                  
															LINE(1),	
															"��������ѡ���߳��˷��š�");
		}
			break;
		case 4:
			{
				/*����*/
					LCD_SetColors(CL_WHITE,CL_BLACK);
					ILI9341_Clear(40,0,ILI9341_MORE_PIXEL,ILI9341_LESS_PIXEL);
					LCD_SetFont(&Font8x16);
					ILI9341_DispString_EN_CH(12*8,                  
																	LINE(1),	
																	"�ճ�ȥ��ͱ���ʯ�����ˣ�");
			}
			break;
			case 5:
			{
			/*����*/
					LCD_SetColors(CL_WHITE,CL_BLACK);
					ILI9341_Clear(40,0,ILI9341_MORE_PIXEL,ILI9341_LESS_PIXEL);
					LCD_SetFont(&Font8x16);
					ILI9341_DispString_EN_CH(12*8,                  
																	LINE(1),	
																	"��������Ϊ��һ�塭��");		
			}
			break;
			case 6:
		{
			/*����*/
			LCD_SetColors(CL_WHITE,CL_BLACK);
			ILI9341_Clear(40,0,ILI9341_MORE_PIXEL,ILI9341_LESS_PIXEL);
			LCD_SetFont(&Font8x16);
			ILI9341_DispString_EN_CH(12*8,                  
															LINE(1),	
															"��������ѡ���߳��˷��š�");
		}
			break;
			case 7:
			{	
			/*����*/
					LCD_SetColors(CL_WHITE,CL_BLACK);
					ILI9341_Clear(40,0,ILI9341_MORE_PIXEL,ILI9341_LESS_PIXEL);
					LCD_SetFont(&Font8x16);
					ILI9341_DispString_EN_CH(10*8,                  
																	LINE(1),	
																	"��̤���˻�������Ҳû�л�������");
			}
			break;
		default:unit=0;
			break;
	}
	/* ���������ť */
  button[0].draw_btn(&button[0]);  
}
