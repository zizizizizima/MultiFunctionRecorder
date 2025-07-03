#ifndef __DS18B20_H
#define	__DS18B20_H

#include "main.h"

#define HIGH  1
#define LOW   0

#define DS18B20_PIN		DATA_Pin                  
#define DS18B20_PORT	DATA_GPIO_Port 

//���κ꣬��������������һ��ʹ��,����ߵ�ƽ��͵�ƽ
#define DS18B20_DATA_OUT(a)	if (a)	\
					HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_SET);\
					else		\
					HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);
 //��ȡ���ŵĵ�ƽ
#define  DS18B20_DATA_IN()	   HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN)

uint8_t ds18b20_init(void);
float ds18b20_read(void);
#endif /* __DS18B20_H */
