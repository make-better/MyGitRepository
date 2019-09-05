#include "game.h"
#include "bsp_ili_lcd.h"
#include "bsp_xpt2046.h"
#include "bsp_usart.h"
#include "bsp_advance_tim.h"
#include <string.h>
#include <stdio.h>
/*按钮结构体数组*/
Touch_Button button[BUTTON_NUM];

//简单延时函数
void delay(unsigned int n)
{
	while(n--);
}

//绘制频率按钮
static void Draw_Frequency_Button(void *btn);
//绘制占空比按钮
static void Draw_Duty_Cycle_Button(void *btn);
//频率按钮控制
static void Command_Frequency_Button(void *btn);
//占空比按钮控制
static void Command_Duty_Cycle_Button(void *btn);




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
  /*重来按钮*/
  button[0].start_x = 30;
  button[0].start_y = 30;
  button[0].end_x = 30+7*8+4 ;
  button[0].end_y = 30+COLOR_BLOCK_HEIGHT;
	button[0].value = 8000-1;
  button[0].touch_flag = 0;  
  button[0].draw_btn = Draw_Frequency_Button ;
  button[0].btn_command = Command_Frequency_Button ;
	/*选择按键*/
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

static void Draw_Frequency_Button(void *btn)
{
	char string[10];
  Touch_Button *ptr = (Touch_Button *)btn;
    
	sprintf(string,"%dHz",8000000/(ptr->value+1));
	
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
    ILI9341_DispString_EN_CH( ptr->start_x+4,
																ptr->start_y+ ((ptr->end_y - ptr->start_y-16)/2),	
																string);
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
    ILI9341_DispString_EN_CH( ptr->start_x+4,          
//																ptr->start_y+15,	           
																ptr->start_y+ ((ptr->end_y - ptr->start_y-16)/2),	
																string);
  } 
  
     /*按钮边框*/
		LCD_SetColors(CL_BLUE4,CL_WHITE);
    ILI9341_DrawRectangle(ptr->start_x,
														ptr->start_y,
														ptr->end_x - ptr->start_x,
														ptr->end_y - ptr->start_y,0);
}


//频率按键处理
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

//绘制占空比按钮
static void Draw_Duty_Cycle_Button(void *btn)
{
	char string[10];
  Touch_Button *ptr = (Touch_Button *)btn;
    
	sprintf(string,"占空比:%%%d ",ptr->value*10);
	
  /*释放按键*/
  if(ptr->touch_flag == 0)
  {
		LCD_SetColors(CL_BUTTON_GREY,CL_WHITE);
    ILI9341_DrawRectangle(	ptr->start_x,
															ptr->start_y,
															ptr->end_x - ptr->start_x,
															ptr->end_y - ptr->start_y,1);
 
		LCD_SetColors(CL_BLUE1,CL_BUTTON_GREY);
		/*选择字体，使用中英文显示时，尽量把英文选择成8*16的字体，
		*中文字体大小是16*16的，需要其它字体请自行制作字模*/
		/*这个函数只对英文字体起作用*/
		LCD_SetFont(&Font8x16);
    ILI9341_DispString_EN_CH( ptr->start_x+4,
																ptr->start_y+ ((ptr->end_y - ptr->start_y-16)/2),	
																string);
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
    ILI9341_DispString_EN_CH( ptr->start_x+4,          
//																ptr->start_y+15,	           
																ptr->start_y+ ((ptr->end_y - ptr->start_y-16)/2),	
																string);
  } 

     /*按钮边框*/
		LCD_SetColors(CL_BLUE4,CL_WHITE);
    ILI9341_DrawRectangle(ptr->start_x,
														ptr->start_y,
														ptr->end_x - ptr->start_x,
														ptr->end_y - ptr->start_y,0);
}

//占空比按钮控制
static void Command_Duty_Cycle_Button(void *btn)
{
	Touch_Button *ptr = (Touch_Button *)btn;
	duty_cycle = (ptr->value);
	TIM_OCInitStructure.TIM_Pulse = (TIM_TimeBaseStructure.TIM_Period+1)*(ptr->value)/10;
	TIM_OC1Init(ADVANCE_TIM, &TIM_OCInitStructure);
}
