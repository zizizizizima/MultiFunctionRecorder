/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */
#include "cmsis_os.h"
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, L1_Pin|L2_Pin|L3_Pin|L4_Pin
                          |L5_Pin|L6_Pin|L7_Pin|L8_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, I2C_SCL_Pin|I2C_SDA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PEPin PEPin PEPin PEPin */
  GPIO_InitStruct.Pin = K2_Pin|K3_Pin|K4_Pin|K1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PEPin PEPin */
  GPIO_InitStruct.Pin = K5_Pin|K6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PEPin PEPin PEPin PEPin
                           PEPin PEPin PEPin PEPin */
  GPIO_InitStruct.Pin = L1_Pin|L2_Pin|L3_Pin|L4_Pin
                          |L5_Pin|L6_Pin|L7_Pin|L8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Pin = I2C_SCL_Pin|I2C_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */
/**
* @brief Function for keys scanning
* @param argument: Not used
* @retval None
*/
uint8_t ScanKey(void) 
{
	uint8_t key = 0;// 按键按下时，K1～K4低电平有效，K5、K6高电平有效
	if (HAL_GPIO_ReadPin(K1_GPIO_Port, K1_Pin) == GPIO_PIN_RESET)	key |= K1_Pin;
	if (HAL_GPIO_ReadPin(K2_GPIO_Port, K2_Pin) == GPIO_PIN_RESET)	key |= K2_Pin;
	if (HAL_GPIO_ReadPin(K3_GPIO_Port, K3_Pin) == GPIO_PIN_RESET)	key |= K3_Pin;
	if (HAL_GPIO_ReadPin(K4_GPIO_Port, K4_Pin) == GPIO_PIN_RESET) key |= K4_Pin;
	if (HAL_GPIO_ReadPin(K5_GPIO_Port, K5_Pin) == GPIO_PIN_SET)		key |= K5_Pin;
	if (HAL_GPIO_ReadPin(K6_GPIO_Port, K6_Pin) == GPIO_PIN_SET)		key |= K6_Pin;
	if (key > 0) { 
		osDelay(10);	// 按键延时10毫秒消抖
		uint8_t key2 = 0;
		if (HAL_GPIO_ReadPin(K1_GPIO_Port, K1_Pin) == GPIO_PIN_RESET) key2 |= K1_Pin;
		if (HAL_GPIO_ReadPin(K2_GPIO_Port, K2_Pin) == GPIO_PIN_RESET) key2 |= K2_Pin;
		if (HAL_GPIO_ReadPin(K3_GPIO_Port, K3_Pin) == GPIO_PIN_RESET) key2 |= K3_Pin;
		if (HAL_GPIO_ReadPin(K4_GPIO_Port, K4_Pin) == GPIO_PIN_RESET) key2 |= K4_Pin;
		if (HAL_GPIO_ReadPin(K5_GPIO_Port, K5_Pin) == GPIO_PIN_SET)		key2 |= K5_Pin;
		if (HAL_GPIO_ReadPin(K6_GPIO_Port, K6_Pin) == GPIO_PIN_SET)		key2 |= K6_Pin;
		if (key == key2)
		{
			return key;
		// 返回有效按键
		}
		return 0;
	}
}
/**
* @brief Function for LED
* @param argument: 8-bit code,1 represent light. xxxx_xxxx --> L1L2L3.....
* @retval None
*/
void SetLeds(uint8_t SetCode)//L1在高位
{
	if((SetCode & 0x80) == 0x80) HAL_GPIO_WritePin(L1_GPIO_Port,L1_Pin,0); else HAL_GPIO_WritePin(L1_GPIO_Port,L1_Pin,1);
	if((SetCode & 0x40) == 0x40) HAL_GPIO_WritePin(L2_GPIO_Port,L2_Pin,0); else HAL_GPIO_WritePin(L2_GPIO_Port,L2_Pin,1);
	if((SetCode & 0x20) == 0x20) HAL_GPIO_WritePin(L3_GPIO_Port,L3_Pin,0); else HAL_GPIO_WritePin(L3_GPIO_Port,L3_Pin,1);
	if((SetCode & 0x10) == 0x10) HAL_GPIO_WritePin(L4_GPIO_Port,L4_Pin,0); else HAL_GPIO_WritePin(L4_GPIO_Port,L4_Pin,1);
	if((SetCode & 0x08) == 0x08) HAL_GPIO_WritePin(L5_GPIO_Port,L5_Pin,0); else HAL_GPIO_WritePin(L5_GPIO_Port,L5_Pin,1);
	if((SetCode & 0x04) == 0x04) HAL_GPIO_WritePin(L6_GPIO_Port,L6_Pin,0); else HAL_GPIO_WritePin(L6_GPIO_Port,L6_Pin,1);
	if((SetCode & 0x02) == 0x02) HAL_GPIO_WritePin(L7_GPIO_Port,L7_Pin,0); else HAL_GPIO_WritePin(L7_GPIO_Port,L7_Pin,1);
	if((SetCode & 0x01) == 0x01) HAL_GPIO_WritePin(L8_GPIO_Port,L8_Pin,0); else HAL_GPIO_WritePin(L8_GPIO_Port,L8_Pin,1);
};
/* USER CODE END 2 */
