#include "game.h"
#include "bsp_ili_lcd.h"
#include "bsp_xpt2046.h"
#include "bsp_usart.h"
#include <string.h>
#include <stdio.h>
/*按钮结构体数组*/
Touch_Button button[BUTTON_NUM];

uint8_t unit=0;              //章节控制

//简单延时函数
void delay(unsigned int n)
{
	while(n--);
}

//绘制重来按钮
static void Draw_Restar_Button(void *btn);
//绘制选择语句
static void Draw_Word_Button(void *btn);
//重来按钮控制
static void Command_Select_Restart(void *btn);
//语句选择按键处理
static void Command_Select_Sentence(void *btn);



void Screan_Init(uint8_t LCD_Mode)
{
	ILI9341_GramScan ( LCD_Mode );
	
  /* 整屏清为黑 */
	LCD_SetBackColor(CL_BLACK);
  ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	

  
  /* 初始化按钮 */
  Touch_Button_Init();
  
  /* 描绘重来按钮 */
  button[0].draw_btn(&button[0]);  
}

void Touch_Button_Init(void)
{
  /*重来按钮*/
  button[0].start_x = BUTTON_START_X;
  button[0].start_y = 0;
  button[0].end_x = BUTTON_START_X+COLOR_BLOCK_WIDTH ;
  button[0].end_y = COLOR_BLOCK_HEIGHT;
  button[0].touch_flag = 0;  
  button[0].draw_btn = Draw_Restar_Button ;
  button[0].btn_command = Command_Select_Restart ;
	/*选择按键*/
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
//语句选择按键处理
static void Command_Select_Sentence(void *btn)
{
	Touch_Button *ptr = (Touch_Button *)btn;

	unit = ptr->towhere;
	
	//使按键消失
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
    ILI9341_DispString_EN_CH( ptr->start_x + (ptr->end_x - ptr->start_x - 16*2 )/2,
																ptr->start_y+ ((ptr->end_y - ptr->start_y-16)/2),	
																"重来");
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
    ILI9341_DispString_EN_CH( ptr->start_x + (ptr->end_x - ptr->start_x - 16*2 )/2,          
//																ptr->start_y+15,	           
																ptr->start_y+ ((ptr->end_y - ptr->start_y-16)/2),	
																"重来");
  } 
  
     /*按钮边框*/
		LCD_SetColors(CL_BLUE4,CL_WHITE);
    ILI9341_DrawRectangle(ptr->start_x,
														ptr->start_y,
														ptr->end_x - ptr->start_x,
														ptr->end_y - ptr->start_y,0);
}
//画带字的按钮
static void Draw_Word_Button(void *btn)
{
  Touch_Button *ptr = (Touch_Button *)btn;
  /*释放按键*/
  if(ptr->touch_flag == 0)
  {	
//			LCD_SetColors(CL_BUTTON_GREY,CL_WHITE);
//    ILI9341_DrawRectangle(	ptr->start_x-1,
//															ptr->start_y-1,
//															ptr->end_x - ptr->start_x,
//															ptr->end_y - ptr->start_y,1);
		LCD_SetColors(CL_WHITE,CL_BLACK);
		/*选择字体，使用中英文显示时，尽量把英文选择成8*16的字体，
		*中文字体大小是16*16的，需要其它字体请自行制作字模*/
		/*这个函数只对英文字体起作用*/
		LCD_SetFont(&Font8x16);
    ILI9341_DispString_EN_CH( ptr->start_x ,
																ptr->start_y+ ((ptr->end_y - ptr->start_y-16)/2),	
																ptr->word);
	
  }
  else  /*按键按下*/
  {
		LCD_SetColors(CL_RED,CL_BLACK);
		/*选择字体，使用中英文显示时，尽量把英文选择成8*16的字体，
		*中文字体大小是16*16的，需要其它字体请自行制作字模*/
		/*这个函数只对英文字体起作用*/
		LCD_SetFont(&Font8x16);
    ILI9341_DispString_EN_CH( ptr->start_x ,                     
																ptr->start_y+ ((ptr->end_y - ptr->start_y-16)/2),	
																ptr->word);
		delay(0xfffff);
  }   
}

//重来按键处理
static void Command_Select_Restart(void *btn)
{
	LCD_SetColors(CL_WHITE,CL_BLACK);
	ILI9341_Clear(0,0,ILI9341_MORE_PIXEL,ILI9341_LESS_PIXEL);
	unit = 0;
	Game_Space();
} 




//选择语句配置函数
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
			/*清屏*/
			LCD_SetColors(CL_WHITE,CL_BLACK);
			ILI9341_Clear(40,0,ILI9341_MORE_PIXEL,ILI9341_LESS_PIXEL);
			LCD_SetFont(&Font8x16);
			ILI9341_DispString_EN_CH(12*8,                  
																	LINE(1),	
																	"*某人的奇妙冒险*");
			Sentence_Config("开始",1,7,1);
			button[1].draw_btn(&button[1]);  //画出本按键
		}
			break;
		case 1:
		{
			/*清屏*/
			LCD_SetColors(CL_WHITE,CL_BLACK);
			ILI9341_Clear(40,0,ILI9341_MORE_PIXEL,ILI9341_LESS_PIXEL);
			LCD_SetFont(&Font8x16);
			ILI9341_DispString_EN_CH(11*8,                  
																	LINE(1),	
																	"少年啊，去创造奇迹吧!");
		}
			break;
		case 2:
		{
			
			Sentence_Config("1.去",1,5,6);		
			button[1].draw_btn(&button[1]);  //画出按键1
				
			Sentence_Config("2.不去",2,7,3);
			button[2].draw_btn(&button[2]);  //画出按键2
			
		}
			break;
		case 3:
		{
			/*清屏*/
			LCD_SetColors(CL_WHITE,CL_BLACK);
			ILI9341_Clear(40,0,ILI9341_MORE_PIXEL,ILI9341_LESS_PIXEL);
			LCD_SetFont(&Font8x16);
			ILI9341_DispString_EN_CH(12*8,                  
															LINE(1),	
															"你做出了选择并走出了房门。");
		}
			break;
		case 4:
			{
				/*清屏*/
					LCD_SetColors(CL_WHITE,CL_BLACK);
					ILI9341_Clear(40,0,ILI9341_MORE_PIXEL,ILI9341_LESS_PIXEL);
					LCD_SetFont(&Font8x16);
					ILI9341_DispString_EN_CH(12*8,                  
																	LINE(1),	
																	"刚出去你就被陨石砸死了！");
			}
			break;
			case 5:
			{
			/*清屏*/
					LCD_SetColors(CL_WHITE,CL_BLACK);
					ILI9341_Clear(40,0,ILI9341_MORE_PIXEL,ILI9341_LESS_PIXEL);
					LCD_SetFont(&Font8x16);
					ILI9341_DispString_EN_CH(12*8,                  
																	LINE(1),	
																	"你与大地融为了一体……");		
			}
			break;
			case 6:
		{
			/*清屏*/
			LCD_SetColors(CL_WHITE,CL_BLACK);
			ILI9341_Clear(40,0,ILI9341_MORE_PIXEL,ILI9341_LESS_PIXEL);
			LCD_SetFont(&Font8x16);
			ILI9341_DispString_EN_CH(12*8,                  
															LINE(1),	
															"你做出了选择并走出了房门。");
		}
			break;
			case 7:
			{	
			/*清屏*/
					LCD_SetColors(CL_WHITE,CL_BLACK);
					ILI9341_Clear(40,0,ILI9341_MORE_PIXEL,ILI9341_LESS_PIXEL);
					LCD_SetFont(&Font8x16);
					ILI9341_DispString_EN_CH(10*8,                  
																	LINE(1),	
																	"你踏入了幻想乡再也没有回来……");
			}
			break;
		default:unit=0;
			break;
	}
	/* 描绘重来按钮 */
  button[0].draw_btn(&button[0]);  
}
