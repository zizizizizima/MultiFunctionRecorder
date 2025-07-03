#include "cmsis_os.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>

/**
  * @brief  Send AT commands
  * @param  cmd and timeout
  * @retval None
  */
void SendATCmd(char *cmd, int waitms)
{
	if(NULL!=cmd)
	{
		HAL_UART_Transmit(&huart6,(uint8_t *)cmd,strlen(cmd),0xFFFF);
		osDelay(waitms); //��ʱ�ȴ�ESP01ģ��Ӧ��ʱ��
	}
}


/**
* @brief Function for Time fetching
* @param argument: Not used
* @retval None
*/
void FetchServer(void)
{
	osDelay(500);
	printf("ESP01����\n");
	SendATCmd("AT+RST\r\n",500);
	printf("�ر�ģ�����\n");
	SendATCmd("ATE0\r\n",500);
	printf("�鿴ģ��汾��Ϣ��\n");
	SendATCmd("AT+GMR\r\n",500);
	printf("����APģʽ\n");
	SendATCmd("AT+CWMODE=1\r\n",500); //if AT+CWMODE=2:AP��=3:AP+STA
	printf("���ӵ�WiFi:thzjw��");
	SendATCmd("AT+CWJAP=\"thzjw\",\"ldfsyzlsy\"\r\n",10000);//����WIFI
	printf("���ӵ�����ʱ��");
	SendATCmd("AT+CIPSTART=\"TCP\",\"www.beijing-time.org\",80\r\n",10000); //���ӱ���ʱ�������
	SendATCmd("AT+CIPSEND=5\r\n",500);			
	SendATCmd("GET\r\n",500);
	osDelay(5000);

}
