#include "app.h"
#include "ff.h"
#include "./flash/bsp_spi_flash.h"
#include "bsp_usart.h"
#include "bsp_led.h"


extern FATFS fs;													/* FatFs�ļ�ϵͳ���� */
extern FIL fnew;													/* �ļ����� */
extern FRESULT res_flash;                /* �ļ�������� */
extern UINT fnum;            					  /* �ļ��ɹ���д���� */
extern BYTE ReadBuffer[100];        /* �������� */
BYTE WriteBuffer[] =              /* д������*/
"��ӭʹ�ñ���STM32������ �����Ǹ������ӣ��½��ļ�ϵͳ�����ļ�\r\n"; 


//�˺����Դ��ļ�ϵͳ������s
void Fatfs_exist_test(void)
{
	//���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��
	//��ʼ������������������
	//f_mount()->find_volume()->disk_initialize->SPI_FLASH_Init()
	res_flash = f_mount(&fs,"1:",1);
	
/*----------------------- ��ʽ������ -----------------*/  
	/* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
	if(res_flash == FR_NO_FILESYSTEM)
	{
		printf("��FLASH��û���ļ�ϵͳ���������и�ʽ��...\r\n");
    /* ��ʽ�� */
		res_flash=f_mkfs("1:",0,0);							
		
		if(res_flash == FR_OK)
		{
			printf("��FLASH�ѳɹ���ʽ���ļ�ϵͳ��\r\n");
      /* ��ʽ������ȡ������ */
			res_flash = f_mount(NULL,"1:",1);			
      /* ���¹���	*/			
			res_flash = f_mount(&fs,"1:",1);
		}
		else
		{
			LED_RED;
			printf("������ʽ��ʧ�ܡ�����\r\n");
			while(1);
		}
	}
  else if(res_flash!=FR_OK)
  {
    printf("�����ⲿFlash�����ļ�ϵͳʧ�ܡ�(%d)\r\n",res_flash);
    printf("��������ԭ��SPI Flash��ʼ�����ɹ���\r\n");
		while(1);
  }
  else
  {
   // printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
  }
}

void Flash_Fatfs_test(void)
{
	 printf("****** ����һ��SPI FLASH �ļ�ϵͳʵ�� ******\r\n");
	//�ļ�ϵͳ��ʽ������
  Fatfs_exist_test();
/*----------------------- �ļ�ϵͳ���ԣ�д���� -------------------*/
	/* ���ļ���ÿ�ζ����½�����ʽ�򿪣�����Ϊ��д */
	printf("\r\n****** ���������ļ�д�����... ******\r\n");	
	res_flash = f_open(&fnew, "1:FatFs��д�����ļ�.txt",FA_CREATE_ALWAYS | FA_WRITE );
	if ( res_flash == FR_OK )
	{
		printf("����/����FatFs��д�����ļ�.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
    /* ��ָ���洢������д�뵽�ļ��� */
		res_flash=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
    if(res_flash==FR_OK)
    {
      printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n",fnum);
      printf("�����ļ�д�������Ϊ��\r\n%s\r\n",WriteBuffer);
    }
    else
    {
      printf("�����ļ�д��ʧ�ܣ�(%d)\n",res_flash);
    }    
		/* ���ٶ�д���ر��ļ� */
    f_close(&fnew);
	}
	else
	{	
		LED_RED;
		printf("������/�����ļ�ʧ�ܡ�\r\n");
	}
	
/*------------------- �ļ�ϵͳ���ԣ������� --------------------------*/
	printf("****** ���������ļ���ȡ����... ******\r\n");
	res_flash = f_open(&fnew, "1:FatFs��д�����ļ�.txt",FA_OPEN_EXISTING | FA_READ); 	 
	if(res_flash == FR_OK)
	{
		LED_GREEN;
		printf("�����ļ��ɹ���\r\n");
		res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
    if(res_flash==FR_OK)
    {
      printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n",fnum);
      printf("����ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", ReadBuffer);	
    }
    else
    {
      printf("�����ļ���ȡʧ�ܣ�(%d)\n",res_flash);
    }		
	}
	else
	{
		LED_RED;
		printf("�������ļ�ʧ�ܡ�\r\n");
	}
	/* ���ٶ�д���ر��ļ� */
	f_close(&fnew);	
  
	/* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
	f_mount(NULL,"1:",1);
  
  /* ������ɣ�ͣ�� */
}

//���������value*1000000��ΪҪд���ļ���lighting.txt����ֵ
void Lighting_write(float value)
{
	//�ļ�ϵͳ��ʽ������
  Fatfs_exist_test();
	res_flash = f_open(&fnew, "1:lighting.txt",FA_CREATE_ALWAYS | FA_WRITE );
	if ( res_flash == FR_OK )
	{
		//printf("����/����FatFs��д�����ļ�.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
    /* ��ָ���洢������д�뵽�ļ��� */
		//res_flash=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
		f_printf(&fnew, "%d", (int)(value*1000000)); 
		f_close(&fnew);
	}
	//-----------------�ļ���ȡ����--------------------------
		res_flash = f_open(&fnew, "1:lighting.txt",FA_OPEN_EXISTING | FA_READ );
		if(res_flash == FR_OK)
	{
		LED_GREEN;
		//printf("�����ļ��ɹ���\r\n");
		res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
    if(res_flash==FR_OK)
    {
    //  printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n",fnum);
      printf("����ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", ReadBuffer);	
    }
    else
    {
      printf("�����ļ���ȡʧ�ܣ�(%d)\n",res_flash);
    }		
	}
	else
	{
		LED_RED;
		printf("�������ļ�ʧ�ܡ�\r\n");
	}
	/* ���ٶ�д���ر��ļ� */
	f_close(&fnew);	
}

//��flash�е�gamedata.txt�ж�����Ҫ�����ݣ���������ָ��λ��ReadBuffer
void Game_read_from_flash(void)
{
	res_flash = f_open(&fnew, "1:gamedata.txt",FA_OPEN_EXISTING | FA_READ );
		if(res_flash == FR_OK)
	{
		printf("�����ļ��ɹ���\r\n");
		res_flash = f_lseek(&fnew,39);
		if(res_flash != FR_OK)
		{
			printf("�α����ʧ��\n\n");
			return;
		}
		res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
    if(res_flash==FR_OK)
    {
      printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n",fnum);
      printf("����ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", ReadBuffer);	
    }
    else
    {
      printf("�����ļ���ȡʧ�ܣ�(%d)\n",res_flash);
    }		
	}
	else
	{
		printf("�������ļ�ʧ�ܡ�\r\n");
	}
	/* ���ٶ�д���ر��ļ� */
	f_close(&fnew);	
}


