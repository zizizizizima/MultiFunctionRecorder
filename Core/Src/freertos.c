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
#include "gpio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//------------------------------GUI���>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//
typedef enum {GUI_TITLE, GUI_INFO, GUI_MAIN} GUI_STATE;	 // ��ö�����ͱ�ʾUI����״̬
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<GUI���------------------------------//
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
//------------------------------GUI���------------------------------//
GUI_STATE GUI_Sta_Cur = GUI_TITLE;// ��ǰUI����
GUI_STATE GUI_Sta_Next = GUI_INFO; // �������UI����
//------------------------------GUI���------------------------------//
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

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void UITitle(void);
void UIInfo(void);
void UIPic(void);
void UIMain(void);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartGUITask(void *argument);
void StartKeyTask(void *argument);

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
//    switch (key) {
//			default:        break;
//			//���˵�ѡ��
//			case K1_Pin:  if (GUI_MAIN == g_sta)
//										{
//											if(g_sta_select < GUI_LED)	g_sta_select = GUI_LED;
//											else {	g_sta_select--; if(g_sta_select < GUI_LED) g_sta_select = GUI_LEDPARAM_SET;}
//										}else if(GUI_LEDPARAM_SET == g_sta)
//										{
//											if(g_sta_select < GUI_LEDSPEED)	g_sta_select = GUI_LEDSPEED;
//											else {	g_sta_select--; if(g_sta_select < GUI_LEDSPEED) g_sta_select = GUI_LEDDIR;}
//										}
//										break;
//			case K4_Pin:  if (GUI_MAIN == g_sta) 
//										{	
//											if(g_sta_select < GUI_LED)	g_sta_select = GUI_LED;
//											else {	g_sta_select++; if(g_sta_select > GUI_LEDPARAM_SET) g_sta_select = GUI_LED;}
//										}else if(GUI_LEDPARAM_SET == g_sta)
//										{
//											if(g_sta_select < GUI_LEDSPEED)	g_sta_select = GUI_LEDSPEED;
//											else {	g_sta_select++; if(g_sta_select > GUI_LEDDIR) g_sta_select = GUI_LEDSPEED;}
//										}
//										break;
//			//������ˮ�Ʋ�����ť
//			case K2_Pin:  if(GUI_LEDSPEED == g_sta)
//										{ 
//											if(speed < 1)	speed = 1;
//											speed--;
//										}else if(GUI_LEDDIR == g_sta)
//										{
//											dir = 0;//���
//										}
//										break;
//			case K3_Pin:	if(GUI_LEDSPEED == g_sta)
//										{
//											speed++;
//											if(speed > 9)	speed = 1;
//										}else if(GUI_LEDDIR == g_sta)
//										{
//											dir = 1;//��
//										}
//										break;
//			//ȷ����ť
//			case K5_Pin:  g_sta = g_sta_select;break;
//			//���ذ�ť
//			case K6_Pin:  if (GUI_MAIN == g_sta) g_sta = GUI_LOGO;
//										else if(GUI_LEDDIR == g_sta)		 g_sta = GUI_LEDPARAM_SET;
//										else if(GUI_LEDSPEED == g_sta)	 g_sta = GUI_LEDPARAM_SET;
//										else g_sta = GUI_MAIN;
//										break;
    osDelay(1);
  }
  /* USER CODE END StartKeyTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
//----------------------------GUI��ͬ���������ʾ����>>>>>>>>>>>>>>>>>>>>>>>>>>>>//
/** @brief Function for ��ʾѡ������
* @param argument: Not used
* @retval None
*/
void UITitle(void)
{
	static uint32_t tick_title = 0; // ���徲̬�������洢�������ʱ��ʱ���
	if (0 == tick_title) tick_title = osKernelGetTickCount(); // ��ʼ�������ʱ����¼ʱ���
	
	GUI_Clear();	// ��Ļ�������
	GUI_DispStringHCenterAt("ѡ��", 64, 0);	// ��Ļ���Ϸ�������ʾ����
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
	GUI_DispStringHCenterAt("ʩ��֥", 64, 0);	// ��Ļ������ʾ����
	GUI_DispStringHCenterAt("22041304", 64, 16);// ��Ļ������ʾ����
	GUI_DispStringHCenterAt("������", 64, 32);	
	GUI_DispStringHCenterAt("22040804", 64, 48);
	GUI_Update();	// ˢ����Ļ��ʾ
 
	// �����ǰʱ���Ѿ���������ʱ��3��
	// ����״̬��ת�����˵�����
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
void UIPic(void)
{
	GUI_Clear();	// ��Ļ�������
	GUI_DispStringHCenterAt("Main", 64, 0);	// ��Ļ���Ϸ�������ʾ����
	GUI_Update();	// ˢ����Ļ��ʾ

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
//hello im syz//
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<GUI��ͬ���������ʾ����------------------------------//
/* USER CODE END Application */

