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
//------------------------------GUI相关>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//
typedef enum {GUI_TITLE, GUI_INFO, GUI_MAIN} GUI_STATE;	 // 用枚举类型表示UI界面状态
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<GUI相关------------------------------//
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
//------------------------------GUI相关------------------------------//
GUI_STATE GUI_Sta_Cur = GUI_TITLE;// 当前UI界面
GUI_STATE GUI_Sta_Next = GUI_INFO; // 待进入的UI界面
//------------------------------GUI相关------------------------------//
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
  //-----OLED初始化、GUI图形库初始化---//
	GUI_Init();
	GUI_SetFont(&GUI_FontHZ_SimSun_16);
  for(;;)
  { 
		switch (GUI_Sta_Cur){
			default:break;
			// 根据当前显示状态调用对应绘图函数
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
		 uint8_t key = ScanKey(); // 扫描按键键码
    if (key > 0) { // 有按键按下
      while (ScanKey() > 0); // 等待按键放开，防止按键连按
      osThreadFlagsSet(defaultTaskHandle, key); // 向默认任务发送key通知
    }
//    switch (key) {
//			default:        break;
//			//主菜单选择
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
//			//调整流水灯参数按钮
//			case K2_Pin:  if(GUI_LEDSPEED == g_sta)
//										{ 
//											if(speed < 1)	speed = 1;
//											speed--;
//										}else if(GUI_LEDDIR == g_sta)
//										{
//											dir = 0;//左←
//										}
//										break;
//			case K3_Pin:	if(GUI_LEDSPEED == g_sta)
//										{
//											speed++;
//											if(speed > 9)	speed = 1;
//										}else if(GUI_LEDDIR == g_sta)
//										{
//											dir = 1;//右
//										}
//										break;
//			//确定按钮
//			case K5_Pin:  g_sta = g_sta_select;break;
//			//返回按钮
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
//----------------------------GUI不同界面相关显示函数>>>>>>>>>>>>>>>>>>>>>>>>>>>>//
* @brief Function for 显示选题名称（？
* @param argument: Not used
* @retval None
*/
void UITitle(void)
{
	static uint32_t tick_title = 0; // 定义静态变量，存储进入界面时的时间戳
	if (0 == tick_title) tick_title = osKernelGetTickCount(); // 开始进入界面时，记录时间戳
	
	GUI_Clear();	// 屏幕内容清空
	GUI_DispStringHCenterAt("选题名称", 64, 0);	// 屏幕正上方居中显示标题
	GUI_Update();	// 刷新屏幕显示
 
	// 如果当前时间已经超过进入时间2秒
	// 界面状态跳转到成员信息界面
	if (osKernelGetTickCount() >= tick_title + 2000) {
		GUI_Sta_Cur = GUI_INFO;
		tick_title = 0;	// 时间戳清零，以备再次进入
	 }
};

/**
* @brief Function for 显示姓名学号
* @param argument: Not used
* @retval None
*/
void UIInfo(void)
{
	static uint32_t tick_info = 0; // 定义静态变量，存储进入界面时的时间戳
	if (0 == tick_info) tick_info = osKernelGetTickCount(); // 开始进入界面时，记录时间戳
	
	GUI_Clear();	// 屏幕内容清空
	GUI_DispStringHCenterAt("施韵芝", 64, 0);	// 屏幕居中显示姓名
	GUI_DispStringHCenterAt("22041304", 64, 16);// 屏幕居中显示姓名
	GUI_DispStringHCenterAt("王欣怡", 64, 32);	
	GUI_DispStringHCenterAt("2204xxxx", 64, 48);
	GUI_Update();	// 刷新屏幕显示
 
	// 如果当前时间已经超过进入时间3秒
	// 界面状态跳转到主菜单界面
	if (osKernelGetTickCount() >= tick_info + 3000) {
		GUI_Sta_Cur = GUI_INFO;
		tick_info = 0;	// 时间戳清零，以备再次进入
	 }
};

/**
* @brief Function for 主菜单
* @param argument: Not used
* @retval None
*/
void UIMain(void)
{
	GUI_Clear();	// 屏幕内容清空
	GUI_DispStringHCenterAt("Main", 64, 0);	// 屏幕正上方居中显示标题
	GUI_Update();	// 刷新屏幕显示

};
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<GUI不同界面相关显示函数------------------------------//
/* USER CODE END Application */

