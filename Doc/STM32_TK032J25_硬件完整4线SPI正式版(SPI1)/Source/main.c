/****************************************************************************
*
* �ļ���: main.c
* ���ݼ���:	������ʵ�ֶ�дSD���ļ��Ĺ��ܡ���ֲ��FatFS�ļ�ϵͳ��
*	����˵����
	(1) ����ϴ����ߣ���windows�Ĵ��ڹ��ߣ����糬���նˡ�
	(2) �����ַ�1����ӡSD��Ŀ¼�µ��ļ����ļ����б�
	(3) �����ַ�2����SD����Ŀ¼�´���һ���ļ����ļ���Ϊ blkeji.txt
			��������ļ�д��һ���ַ�����
	(4) �����ַ�3����SD����Ŀ¼�µ�blkeji.txt�ļ������������ݣ���ӡ�����ڡ�
*
*
*/

/*	   

	��������еĺ����õ��˽϶�ľֲ����������ȱʡ�Ķ�ջ�����á�
	��Ҫ������һЩ��
	
	�޸� startup_stm32f10x_hd.s �ļ�
	��ǰΪ ��Stack_Size      EQU     0x00000400
	����Ϊ ��Stack_Size      EQU     0x00001000


*/


#include <stdio.h>
#include "stm32f10x.h"
#include "systick.h"
#include "usart_printf.h"
#include "sdcard.h"
#include "ff.h"
#include "LCD.h"
#define EXAMPLE_NAME	"SDIO FatFS Demo"
#define EXAMPLE_DATE	"2011-05-15"

static void InitBoard(void);
static void DispLogo(void);

void ViewRootDir(void);
void CreatNewFile(void);
void ReadFileData(void);
void DispMenu(void);

// #include "ttr.h"
// #include "scenery_A.h"
// #include "scenery_B.h"
// #include "mm.h"

static void Button_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    /* config tft back_light gpio base on the PT4101 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;		
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
*	��������main
*	��  �ܣ��û��������
*	��  ��: ��
*	��  ��: ��
*/
int main(void)
{
	char cmd;
	unsigned int i=30001,flag=1;
	InitBoard();	/* Ϊ����main���������������Щ�����ǽ���ʼ���Ĵ����װ��������� */
	LCD_GPIO_Config();
// 	DispLogo();		/* ��ʾ����Logo */
	
// 	Button_GPIO_Config();
// 	SD_Init();
// 	test_color();
// 	LCD_Fill_Pic(0,0,240,320, gImage_ttr);
	SPI1_Init();
	Lcd_Initialize();
	WriteComm(0x36);
	WriteData(0x10);//����Ϊ��������ʾ����
	LCD_PutString(30,30,"֧�ֺ�����",YELLOW,RED,0);
// TP_Init();
	WriteComm(0x36);//����Ĵ�����ר������������ʾ�����
	WriteData(0x60);//����Ϊ��������ʾ����
	LCD_PutString(80,20,"����������������������",YELLOW,RED,1);
	Lcd_ColorBox(100,100,20,20,Yellow);
	Lcd_ColorBox(140,70,20,20,Yellow);
		while(1);
	{
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0){i++;flag=1;}
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0){i--;flag=1;}
		if(flag==1)
		{
			switch(i%15)
			{
			case 0:
				display_picture("T35_00.BMP");	
				break;	
			case 1:
// 				LCD_Fill_Pic(0,0,176,220, gImage_scenery_A);
				display_picture("T35_01.BMP");
				break;
			case 2:
// 				LCD_Fill_Pic(0,0,176,220, gImage_scenery_B);
				display_picture("T35_02.BMP");
				break;
			case 3:
// 				LCD_Fill_Pic(0,0,176,220, gImage_mm);
				display_picture("T35_03.BMP");
				break;
			case 4:
				display_picture("T35_04.BMP");	
				break;
			case 5:
				display_picture("T35_05.BMP");	
				break;
			case 6:
				display_picture("T30_01.BMP");	
				break;
			case 7:
				display_picture("T35_06.BMP");	
				break;
			case 8:
				display_picture("T35_07.BMP");	
				break;
			case 9:
				display_picture("T35_08.BMP");	
				break;
			case 10:
				display_picture("T35_09.BMP");	
				break;
			case 11:
				display_picture("T35_10.BMP");	
				break;
			case 12:
				display_picture("MM.BMP");	
				break;
			case 13:
				display_picture("T30_02.bmp");	
				break;
			case 14:
				display_picture("�羰.bmp");	
				break;
			}		
			flag=0;		
		}
	}
	DispMenu();/* ��ӡ�����б��û�����ͨ�����ڲ���ָ�� */
	while(1)
	{
		cmd = getchar();
		switch (cmd)
		{
			case '1':
				printf("��1 - ViewRootDir��\r\n");
				ViewRootDir();		/* ��ʾSD����Ŀ¼�µ��ļ��� */
				break;

			case '2':
				printf("��2 - CreatNewFile��\r\n");
				CreatNewFile();		/* ����һ�����ļ�,д��һ���ַ��� */
				break;

			case '3':
				printf("��3 - ReadFileData��\r\n");
				ReadFileData();		/* ��ȡ��Ŀ¼��blkeji.txt������ */
				break;

			default:
				DispMenu();
				break;
		}
	}
	
}

/*******************************************************************************
*	��������ViewRootDir
*	��  �ܣ���ʾSD����Ŀ¼�µ��ļ���
*	��  ��: ��
*	��  ��: ��
*/
void DispMenu(void)
{
	printf("\r\n*******************************************\r\n");
	printf("Please Select Command\r\n");
	printf("1 - Display Root Directory Files\r\n");
	printf("2 - Create A New File\r\n");
	printf("3 - Read File Data\r\n");
}

/*******************************************************************************
*	��������ViewRootDir
*	��  �ܣ���ʾSD����Ŀ¼�µ��ļ���
*	��  ��: ��
*	��  ��: ��
*/
void ViewRootDir(void)
{
	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FRESULT result;
	FATFS fs;
	DIR DirInf;  
	FILINFO FileInf;
	
	uint8_t tmpStr[20];
	uint32_t cnt = 0;
	
 	/* �����ļ�ϵͳ */
	result = f_mount(0, &fs);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("FileSystem Mounted Failed (%d)\r\n", result);
	}

	/* �򿪸��ļ��� */
	result = f_opendir(&DirInf, "/"); /* ���������������ӵ�ǰĿ¼��ʼ */
	if (result != FR_OK) 
	{
		printf("Root Directory is Open Error (%d)\r\n", result);
		return;
	}

	/* ��ȡ��ǰ�ļ����µ��ļ���Ŀ¼ */
	printf("Name\t\t\tTyepe\t\t\tSize\r\n");
	for (cnt = 0; ;cnt++) 
	{
		result = f_readdir(&DirInf,&FileInf); 		/* ��ȡĿ¼��������Զ����� */
		if (result != FR_OK || FileInf.fname[0] == 0)
		{
			break;
		}
		
		if (FileInf.fname[0] == '.')
		{
			continue;
		}
		printf("%s\t\t", FileInf.fname);
		if (FileInf.fattrib == AM_DIR)
		{
			printf("Directory\t");
		} 
		else 
		{
			printf("File\t\t");
		}
		printf("%d\r\n", FileInf.fsize);
		sprintf((char *)tmpStr, "%d", FileInf.fsize);
	}

	/* ж���ļ�ϵͳ */
	f_mount(0, NULL);	
}

/*******************************************************************************
*	��������CreatNewFile
*	��  �ܣ���SD������һ�����ļ����ļ�������д��www.blkeji.com��
*	��  ��: ��
*	��  ��: ��
*/
void CreatNewFile(void)
{
	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FRESULT result;
	FATFS fs;
	FIL file;
	DIR DirInf;  
	uint32_t bw;
	
 	/* �����ļ�ϵͳ */
	result = f_mount(0, &fs);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("FileSystem Mounted Failed (%d)\r\n", result);
	}

	/* �򿪸��ļ��� */
	result = f_opendir(&DirInf, "/"); /* ���������������ӵ�ǰĿ¼��ʼ */
	if (result != FR_OK) 
	{
		printf("Root Directory is Open Error (%d)\r\n", result);
		return;
	}

	/* ���ļ� */
	result = f_open(&file, "blkeji.txt", FA_CREATE_ALWAYS | FA_WRITE);
	
	/* дһ������ */
	result = f_write(&file, "FatFS Write Demo \r\n www.blkeji.com \r\n", 34, &bw);	
	if (result == FR_OK)
	{
		printf("blkeji.txt File Write Success\r\n");
	}
	else
	{
		printf("blkeji.txt File Write Failed");
	}
	
	/* �ر��ļ�*/
	f_close(&file);
 	
	/* ж���ļ�ϵͳ */
	f_mount(0, NULL);	
}

/*******************************************************************************
*	��������ReadFileData
*	��  �ܣ���ȡָ���ļ�ǰ128���ַ�������ӡ�����ڡ�
*	��  ��: ��
*	��  ��: ��
*/
void ReadFileData(void)
{
	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FRESULT result;
	FATFS fs;
	FIL file;
	DIR DirInf;  
	uint32_t bw;
	char buf[128];
	
 	/* �����ļ�ϵͳ */
	result = f_mount(0, &fs);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("FileSystem Mounted Failed (%d)\r\n", result);
	}

	/* �򿪸��ļ��� */
	result = f_opendir(&DirInf, "/"); /* ���������������ӵ�ǰĿ¼��ʼ */
	if (result != FR_OK) 
	{
		printf("Root Directory is Open Error (%d)\r\n", result);
		return;
	}

	/* ���ļ� */
	result = f_open(&file, "blkeji.txt", FA_OPEN_EXISTING | FA_READ);
	if (result !=  FR_OK)
	{
		printf("Don't Find File : blkeji.txt\r\n");
		return;		
	}

	/* ��ȡ�ļ� */
	result = f_read(&file, &buf, sizeof(buf) - 1, &bw);
	if (bw > 0)
	{
		buf[bw] = 0;
		printf("\r\nblkeji.txt content : \r\n%s\r\n", buf);
	}
	else
	{
		printf("\r\nblkeji.txt content : \r\n");		
	}
	
	/* �ر��ļ�*/
	f_close(&file);
 	
	/* ж���ļ�ϵͳ */
	f_mount(0, NULL);
}

/*******************************************************************************
	��������NVIC_Configuration
	��  ��:
	��  ��:
	����˵��������SDIO�ж�
*/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
	��������GPIO_Configuration
	��  ��:
	��  ��:
	����˵��������4��LEDΪ�������

	LED���߷��䣺
	LED0 : PE0  (���0����)
	LED1 : PE1  (���0����)
	LED2 : PE2  (���0����)
	LED3 : PE3  (���0����)

*/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��1������GPIOA GPIOC GPIOD GPIOE GPIOF GPIOG��ʱ��
	   ע�⣺����ط�����һ����ȫ��
	*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC
			| RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG,
				ENABLE);

	/* ��3�����������е�LEDָʾ��GPIOΪ�������ģʽ */
	/* ���ڽ�GPIO����Ϊ���ʱ��GPIO����Ĵ�����ֵȱʡ��0����˻�����LED����
		�����Ҳ�ϣ���ģ�����ڸı�GPIOΪ���ǰ�����޸�����Ĵ�����ֵΪ1 */
	GPIO_SetBits(GPIOE,  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/*******************************************************************************
	��������InitBoard
	��  ��:
	��  ��:
	����˵������ʼ��Ӳ���豸
*/
static void InitBoard(void)
{
	/*
		���������ST���еĺ���������ʵ����
		Libraries\CMSIS\Core\CM3\system_stm32f10x.c

		�����ڲ�Flash�ӿڣ���ʼ��PLL������ϵͳƵ��
		ϵͳʱ��ȱʡ����Ϊ72MHz���������Ҫ���ģ�����Ҫȥ�޸���ص�ͷ�ļ��еĺ궨��
	 */
	SystemInit();

	/* ����LED GPIO */
// 	GPIO_Configuration();

	/* ���ô��� */
	USART_Configuration();

	/* ����systic��Ϊ1ms�ж�,���������
	\Libraries\CMSIS\Core\CM3\core_cm3.h */
// 	SysTick_Config(SystemFrequency / 1000);
	
	/* �����ж�ϵͳ */
	NVIC_Configuration();
}

/*******************************************************************************
	������: DispLogo
	��  ��:
	��  ��:
	����˵������ʾ����Logo (ͨ�����ڴ�ӡ��PC���ĳ����ն���ʾ)
*/
static void DispLogo(void)
{
	/* ͨ����������������͸������� */
	PrintfLogo(EXAMPLE_NAME, EXAMPLE_DATE);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
