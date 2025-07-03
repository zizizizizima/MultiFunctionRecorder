/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gpio.h"
#include "usart.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//------------------------------GUI���------------------------------//
typedef enum {GUI_TITLE, GUI_INFO, GUI_PIC, GUI_MAIN,GUI_RECORDER,GUI_TIMESET,GUI_PARASET} GUI_STATE;	 
//accept for the title and team information, there're only four : 
//1.main(showing time and date)
//2.recorder(recording)
//3.timeset(...)
//4.paraset(...)
//------------------------------GUI���------------------------------//
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define MAX_RECV_LEN 512	//uart rev queue size(512*8bit)
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
//------------------------------GUI���------------------------------//
GUI_STATE GUI_Sta_Cur = GUI_TITLE;// ��ǰUI����
GUI_STATE GUI_Sta_Next = GUI_INFO; // �������UI����

uint8_t Press_Flag;

float Temp;
//------------------------------GUI���------------------------------//
//------------------------------USART-------------------
uint8_t rx1_buf[MAX_RECV_LEN];
uint8_t rx6_buf[MAX_RECV_LEN];
//------------------------------USART-------------------

int cid = -1;//客户端ID号
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for guiTask */
osThreadId_t guiTaskHandle;
const osThreadAttr_t guiTask_attributes = {
  .name = "guiTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for keyTask */
osThreadId_t keyTaskHandle;
const osThreadAttr_t keyTask_attributes = {
  .name = "keyTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for usartTask */
osThreadId_t usartTaskHandle;
const osThreadAttr_t usartTask_attributes = {
  .name = "usartTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for ledTask */
osThreadId_t ledTaskHandle;
const osThreadAttr_t ledTask_attributes = {
  .name = "ledTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for espTask */
osThreadId_t espTaskHandle;
const osThreadAttr_t espTask_attributes = {
  .name = "espTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for tempTask */
osThreadId_t tempTaskHandle;
const osThreadAttr_t tempTask_attributes = {
  .name = "tempTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for QueueUsart1 */
osMessageQueueId_t QueueUsart1Handle;
const osMessageQueueAttr_t QueueUsart1_attributes = {
  .name = "QueueUsart1"
};
/* Definitions for QueueUsart6 */
osMessageQueueId_t QueueUsart6Handle;
const osMessageQueueAttr_t QueueUsart6_attributes = {
  .name = "QueueUsart6"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void UITitle(void);
void UIInfo(void);
void UIPic(void);
void UIMain(void);
void UIRecorder(void);
void UITimeSet(void);
void UIParaSet(void);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartGUITask(void *argument);
void StartKeyTask(void *argument);
void StartUsartTask(void *argument);
void StartledTask(void *argument);
void StartESPTask(void *argument);
void StartTempTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of QueueUsart1 */
  QueueUsart1Handle = osMessageQueueNew (512, sizeof(uint8_t), &QueueUsart1_attributes);

  /* creation of QueueUsart6 */
  QueueUsart6Handle = osMessageQueueNew (512, sizeof(uint8_t), &QueueUsart6_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of guiTask */
  guiTaskHandle = osThreadNew(StartGUITask, NULL, &guiTask_attributes);

  /* creation of keyTask */
  keyTaskHandle = osThreadNew(StartKeyTask, NULL, &keyTask_attributes);

  /* creation of usartTask */
  usartTaskHandle = osThreadNew(StartUsartTask, NULL, &usartTask_attributes);

  /* creation of ledTask */
  ledTaskHandle = osThreadNew(StartledTask, NULL, &ledTask_attributes);

  /* creation of espTask */
  espTaskHandle = osThreadNew(StartESPTask, NULL, &espTask_attributes);

  /* creation of tempTask */
  tempTaskHandle = osThreadNew(StartTempTask, NULL, &tempTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartGUITask */
/**
* @brief Function implementing the guiTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartGUITask */
void StartGUITask(void *argument)
{
  /* USER CODE BEGIN StartGUITask */
  /* Infinite loop */
  //-----OLED��ʼ����GUIͼ�ο��ʼ��---//
	GUI_Init();
	GUI_SetFont(&GUI_FontHZ_SimSun_16);
  for(;;)
  { 
		switch (GUI_Sta_Cur){
			default:break;
			// ���ݵ�ǰ��ʾ״̬���ö�Ӧ��ͼ����
			case GUI_TITLE: UITitle(); break;
			case GUI_INFO: UIInfo();	break;
			case GUI_MAIN: UIMain(); 	break;
			case GUI_RECORDER: UIRecorder(); break;
			case GUI_TIMESET: UITimeSet(); break;
			case GUI_PARASET: UIParaSet(); break;
		}
    osDelay(10);
  }
  /* USER CODE END StartGUITask */
}

/* USER CODE BEGIN Header_StartKeyTask */
/**
* @brief Function implementing the keyTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartKeyTask */
void StartKeyTask(void *argument)
{
  /* USER CODE BEGIN StartKeyTask */
  /* Infinite loop */
  for(;;)
  {
		 uint8_t key = ScanKey(); // ɨ�谴������
    if (key > 0) { // �а�������
      while (ScanKey() > 0); // �ȴ������ſ�����ֹ��������
      osThreadFlagsSet(defaultTaskHandle, key); // ��Ĭ��������key֪ͨ
    }
    switch (key) {
			default:        break;
			//���˵�ѡ��
			case K1_Pin:  if (GUI_MAIN == GUI_Sta_Cur)
										{
											GUI_Sta_Cur = GUI_TIMESET;
										}
										break;
			
			//������ˮ�Ʋ�����ť
			case K2_Pin:  
										break;
			case K3_Pin:	if(GUI_MAIN == GUI_Sta_Cur)
										{
											GUI_Sta_Cur = GUI_RECORDER;
										}
										break;
			case K4_Pin:  if (GUI_MAIN == GUI_Sta_Cur) 
										{	
											GUI_Sta_Cur = GUI_PARASET;
										}
										break;
			//ȷ����ť
			case K5_Pin:  if(GUI_TITLE == GUI_Sta_Cur || GUI_INFO == GUI_Sta_Cur) GUI_Sta_Cur = GUI_RECORDER;
										else if(GUI_RECORDER == GUI_Sta_Cur && Press_Flag == 0)
										{
											Press_Flag = 1;
											//!!!!!Start recording 
										}else if(GUI_RECORDER == GUI_Sta_Cur && Press_Flag == 1)
										{
											Press_Flag = 0;
											//!!!!!Finish recording after the second press
										}
										break;
			//���ذ�ť
			case K6_Pin:  
										break;
			}
    osDelay(1);
  }
  /* USER CODE END StartKeyTask */
}

/* USER CODE BEGIN Header_StartUsartTask */
/**
* @brief Function implementing the usartTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUsartTask */
void StartUsartTask(void *argument)
{
  /* USER CODE BEGIN StartUsartTask */
	HAL_UARTEx_ReceiveToIdle_IT(&huart1,rx1_buf,sizeof(rx1_buf));
	HAL_UARTEx_ReceiveToIdle_IT(&huart6,rx6_buf,sizeof(rx6_buf));
	uint8_t dat[MAX_RECV_LEN];	//临时数组
  /* Infinite loop */
  for(;;)
  {
		//uart1 receiving data and processing
		if(osMessageQueueGet(QueueUsart1Handle,dat,NULL,10)==osOK)
		{
			printf("UART1:%s",dat);	//串口1打印回显
			HAL_UART_Transmit(&huart6,dat,strlen((char *)dat),0xFFFF);	//转发串口6（？
			//是给开发者提供了在串口助手里的调试接口
		}
		
		//串口6接收数据处理
		if(osMessageQueueGet(QueueUsart6Handle,dat,NULL,10)==osOK)
		{
			printf("UART6:%s",dat);	//串口1打印
			char *str = (char *)dat;
			if(strstr(str,"CONNECT") > str)	cid = atoi(str);	//如果有客户端连接进来，取ID号
			if(strstr(str,"CLOSED") > str)	cid = -1;					//如果有客户端连接断开，关闭ID号	
			
			//时间对比
			char *p = strstr(str,"Disp:");
			if(p > str) //找到了
			{
//				strncpy(g_segBuf,p+1,20);	//跳过前五个，最多20个字符
//				g_segBuf[19] = '\0';
				int idx = 0;
				for(char *dat = p + 5; *dat && idx < 19; ++dat)
				{
					if((*dat >= '0' && *dat <= '9')	||
						 (*dat >= 'a' && *dat <= 'f') ||
						 (*dat >= 'A' && *dat <= 'F'))
					{
						//
					}
				}
				//
			}
			
			//时间显示
			
			char *p_time = strstr(str,"GMT");
			if(p_time >= str)
			{
				int idx = 0;
				for(char *dat = p_time - 6; *dat && idx < 19; ++dat)
				{
					if(idx <= 3)
					{
						if((*dat >= '0' && *dat <= '9'))
						{
							//g_segBuf2[idx++] = *dat;
						}
					}else
					{
						//g_segBuf2[idx++] = '\0';
					}
				}
			}
		}
    osDelay(1);
  }
  /* USER CODE END StartUsartTask */
}

/* USER CODE BEGIN Header_StartledTask */
/**
* @brief Function implementing the ledTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartledTask */
void StartledTask(void *argument)
{
  /* USER CODE BEGIN StartledTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartledTask */
}

/* USER CODE BEGIN Header_StartESPTask */
/**
* @brief Function implementing the espTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartESPTask */
void StartESPTask(void *argument)
{
  /* USER CODE BEGIN StartESPTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartESPTask */
}

/* USER CODE BEGIN Header_StartTempTask */
/**
* @brief Function implementing the tempTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTempTask */
void StartTempTask(void *argument)
{
  /* USER CODE BEGIN StartTempTask */
	uint32_t tick = 0;
	ds18b20_init();		//init DS18B20 device
  /* Infinite loop */
  for(;;)
  {
	  uint32_t cur_tick = osKernelGetTickCount();
	  if(cur_tick >= tick){
		  tick = cur_tick + 1000;
		  Temp = ds18b20_read();
	  }
		osDelay(1);
  }
  /* USER CODE END StartTempTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
//----------------------------GUI��ͬ���������ʾ����------------------------------//
/** @brief Function for ��ʾѡ������
* @param argument: Not used
* @retval None
*/
void UITitle(void)
{
	static uint32_t tick_title = 0; // ���徲̬�������洢�������ʱ��ʱ���
	if (0 == tick_title) tick_title = osKernelGetTickCount(); // ��ʼ�������ʱ����¼ʱ���
	
	GUI_Clear();	// ��Ļ�������
	GUI_DispStringHCenterAt("MFR", 64, 0);	// ��Ļ���Ϸ�������ʾ����
	GUI_Update();	// ˢ����Ļ��ʾ
 
	// �����ǰʱ���Ѿ���������ʱ��2��
	// ����״̬��ת����Ա��Ϣ����
	if (osKernelGetTickCount() >= tick_title + 2000) {
		GUI_Sta_Cur = GUI_INFO;
		tick_title = 0;	// ʱ������㣬�Ա��ٴν���
	 }
};

/**
* @brief Function for ��ʾ����ѧ��
* @param argument: Not used
* @retval None
*/
void UIInfo(void)
{
	static uint32_t tick_info = 0; // ���徲̬�������洢�������ʱ��ʱ���
	if (0 == tick_info) tick_info = osKernelGetTickCount(); // ��ʼ�������ʱ����¼ʱ���
	
	GUI_Clear();	// ��Ļ�������
	GUI_DispStringHCenterAt("SYZ", 64, 0);	// ��Ļ������ʾ����
	GUI_DispStringHCenterAt("22041304", 64, 16);// ��Ļ������ʾ����
	GUI_DispStringHCenterAt("WXY", 64, 32);	
	GUI_DispStringHCenterAt("22040804", 64, 48);
	GUI_Update();	// ˢ����Ļ��ʾ
 
	// �����ǰʱ���Ѿ���������ʱ��3��
	// ����״̬��ת�����˵�����
	if (osKernelGetTickCount() >= tick_info + 3000) {
		GUI_Sta_Cur = GUI_PIC;
		tick_info = 0;	// ʱ������㣬�Ա��ٴν���
	 }
};

/**
* @brief Function for ���˵�
* @param argument: Not used
* @retval None
*/
void UIPic(void)
{
	static uint32_t tick_info = 0; // ���徲̬�������洢�������ʱ��ʱ���
	if (0 == tick_info) tick_info = osKernelGetTickCount(); // ��ʼ�������ʱ����¼ʱ���
	
	GUI_Clear();	// ��Ļ�������
	GUI_DrawBitmap(&bmSYZ,0,0);
	GUI_DrawBitmap(&bmWXY,128 - bmWXY.XSize,0);	
	GUI_Update();	// ˢ����Ļ��ʾ
	
	if (osKernelGetTickCount() >= tick_info + 3000) {
		GUI_Sta_Cur = GUI_MAIN;
		tick_info = 0;	// ʱ������㣬�Ա��ٴν���
	}

};

/**
* @brief Function for ���˵�
* @param argument: Not used
* @retval None
*/
void UIMain(void)
{
	GUI_Clear();	// ��Ļ�������
	GUI_DispStringHCenterAt("Main", 64, 0);	// ��Ļ���Ϸ�������ʾ����
	GUI_Update();	// ˢ����Ļ��ʾ

};

/**
* @brief Function for Recording
* @param argument: Not used
* @retval None
*/
void UIRecorder(void)
{
	GUI_Clear();	// ��Ļ�������
	GUI_DispStringHCenterAt("Recording", 64, 0);	// ��Ļ���Ϸ�������ʾ����
	GUI_Update();	// ˢ����Ļ��ʾ

};

/**
* @brief Function for Time setting
* @param argument: Not used
* @retval None
*/
void UITimeSet(void)
{
	GUI_Clear();	// ��Ļ�������
	GUI_DispStringHCenterAt("Timeset", 64, 0);	// ��Ļ���Ϸ�������ʾ����
	GUI_Update();	// ˢ����Ļ��ʾ

};

/**
* @brief Function for Time setting
* @param argument: Not used
* @retval None
*/
void UIParaSet(void)
{
	GUI_Clear();	// ��Ļ�������
	GUI_DispStringHCenterAt("Paraset", 64, 0);	// ��Ļ���Ϸ�������ʾ����
	GUI_Update();	// ˢ����Ļ��ʾ

};

//------------------------------GUI��ͬ���������ʾ����------------------------------//

//------------------------------USART-----------------------------
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t Size)
{
	if(huart == &huart1)
	{
		rx1_buf[Size] = 0;	//字符串结束符
		osMessageQueuePut(QueueUsart1Handle,rx1_buf,NULL,0);
		__HAL_UNLOCK(huart);
		HAL_UARTEx_ReceiveToIdle_IT(&huart1,rx1_buf,sizeof(rx1_buf));
	}
	if(huart == &huart6)
	{
		rx6_buf[Size] = 0;	//字符串结束符
		osMessageQueuePut(QueueUsart6Handle,rx6_buf,NULL,0);
		__HAL_UNLOCK(huart);
		HAL_UARTEx_ReceiveToIdle_IT(&huart6,rx6_buf,sizeof(rx6_buf));
	}
}
//------------------------------USART-----------------------------
/* USER CODE END Application */

