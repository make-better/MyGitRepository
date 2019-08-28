#include "game.h"
#include "bsp_ili_lcd.h"
#include "bsp_xpt2046.h"
#include "bsp_usart.h"
#include <string.h>
#include <stdio.h>
/*按钮结构体数组*/
Touch_Button button[BUTTON_NUM];

//简单延时函数
void delay(unsigned int n)
{
	while(n--);
}

//绘制按钮
static void Draw_Button(void *btn);

//按钮控制
static void Command_Button(void *btn);


void Screan_Init(uint8_t LCD_Mode)
{
	uint8_t i=0;
	ILI9341_GramScan ( LCD_Mode );
  /* 整屏清为黑 */
	LCD_SetBackColor(CL_BLACK);
  ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	
  /* 初始化按钮 */
  Touch_Button_Init();
  /* 描绘重来按钮 */
	for(i=0;i<BUTTON_NUM;i++)
	{
		button[i].draw_btn(&button[i]);
	}
    
}

void Touch_Button_Init(void)
{
  /*开关按钮*/
  button[0].start_x = 80;
  button[0].start_y = 40;
  button[0].end_x = 160 ;
  button[0].end_y = 80;
	button[0].value = 1;
  button[0].touch_flag = 0;  
	sprintf(button[0].name,"开关");
  button[0].draw_btn = Draw_Button ;
  button[0].btn_command = Command_Button ;
	/*方向按键*/
	button[1].start_x = 80;
  button[1].start_y = 120;
  button[1].end_x = 160 ;
  button[1].end_y = 160;
	button[1].value = 2;
  button[1].touch_flag = 0;  
	sprintf(button[1].name,"方向");
  button[1].draw_btn = Draw_Button ;
  button[1].btn_command = Command_Button ;
	/*自由转动按键*/
	button[2].start_x = 80;
  button[2].start_y = 200;
  button[2].end_x = 160 ;
  button[2].end_y = 240;
	button[2].value = 3;
  button[2].touch_flag = 0;
	sprintf(button[2].name,"脱机");
  button[2].draw_btn = Draw_Button ;
  button[2].btn_command = Command_Button ;
}

/**
* @brief  Touch_Button_Down 按键被按下时调用的函数，由触摸屏调用
* @param  x 触摸位置的x坐标
* @param  y 触摸位置的y坐标
* @retval 无
*/
void Touch_Button_Down(uint16_t x,uint16_t y)
{
  uint8_t i;
  for(i=0;i<BUTTON_NUM;i++)
  {
    /* 触摸到了按钮 */
    if(x<=button[i].end_x && y<=button[i].end_y && y>=button[i].start_y && x>=button[i].start_x )
    {
      if(button[i].touch_flag == 0)     /*原本的状态为没有按下，则更新状态*/
      {
      button[i].touch_flag = 1;         /* 记录按下标志 */
      
      button[i].draw_btn(&button[i]);  /*重绘按钮*/
      }        
      
    }
    else if(button[i].touch_flag == 1) /* 触摸移出了按键的范围且之前有按下按钮 */
    {
      button[i].touch_flag = 0;         /* 清除按下标志，判断为误操作*/
      
      button[i].draw_btn(&button[i]);   /*重绘按钮*/
    }
  }

}

/**
* @brief  Touch_Button_Up 按键被释放时调用的函数，由触摸屏调用
* @param  x 触摸最后释放时的x坐标
* @param  y 触摸最后释放时的y坐标
* @retval 无
*/
void Touch_Button_Up(uint16_t x,uint16_t y)
{
   uint8_t i; 
   for(i=0;i<BUTTON_NUM;i++)
   {
     /* 触笔在按钮区域释放 */
      if((x<button[i].end_x && x>button[i].start_x && y<button[i].end_y && y>button[i].start_y))
      {        
				
        button[i].touch_flag = 0;       /*释放触摸标志*/

        button[i].draw_btn(&button[i]); /*重绘按钮*/        
      
        button[i].btn_command(&button[i]);  /*执行按键的功能命令*/
        
        break;
      }
    }  
}

static void Draw_Button(void *btn)
{
  Touch_Button *ptr = (Touch_Button *)btn;
	
  /*释放按键*/
  if(ptr->touch_flag == 0)
  {
		LCD_SetColors(CL_BUTTON_GREY,CL_WHITE);
    ILI9341_DrawRectangle(	ptr->start_x,
															ptr->start_y,
															ptr->end_x - ptr->start_x,
															ptr->end_y - ptr->start_y,1);
    

		
		LCD_SetColors(CL_RED,CL_BUTTON_GREY);
		/*选择字体，使用中英文显示时，尽量把英文选择成8*16的字体，
		*中文字体大小是16*16的，需要其它字体请自行制作字模*/
		/*这个函数只对英文字体起作用*/
		LCD_SetFont(&Font8x16);
    ILI9341_DispString_EN_CH( ptr->start_x+20,
																ptr->start_y+ ((ptr->end_y - ptr->start_y-16)/2),	
																ptr->name);
  }
  else  /*按键按下*/
  {
		LCD_SetColors(CL_BUTTON_GREY,CL_BUTTON_GREY);
    ILI9341_DrawRectangle(ptr->start_x,
														ptr->start_y,
														ptr->end_x - ptr->start_x,
														ptr->end_y - ptr->start_y,1);
		LCD_SetColors(CL_BLACK,CL_BUTTON_GREY);
		/*选择字体，使用中英文显示时，尽量把英文选择成8*16的字体，
		*中文字体大小是16*16的，需要其它字体请自行制作字模*/
		/*这个函数只对英文字体起作用*/
		LCD_SetFont(&Font8x16);
    ILI9341_DispString_EN_CH( ptr->start_x+20,          
//																ptr->start_y+15,	           
																ptr->start_y+ ((ptr->end_y - ptr->start_y-16)/2),	
																ptr->name);
  } 
  
     /*按钮边框*/
		LCD_SetColors(CL_BLUE4,CL_WHITE);
    ILI9341_DrawRectangle(ptr->start_x,
														ptr->start_y,
														ptr->end_x - ptr->start_x,
														ptr->end_y - ptr->start_y,0);
}


//按键处理
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


