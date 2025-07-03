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
		osDelay(waitms); //延时等待ESP01模块应答时间
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
	printf("ESP01 reset\n");
	SendATCmd("AT+RST\r\n",500);
	printf("close info returning\n");
	SendATCmd("ATE0\r\n",500);
	printf("module version info...\n");
	SendATCmd("AT+GMR\r\n",500);
	printf("STA mode\n");
	SendATCmd("AT+CWMODE=1\r\n",500); //if AT+CWMODE=2:AP，=3:AP+STA
	printf("Connect to WiFi:thzjw...");
	SendATCmd("AT+CWJAP=\"thzjw\",\"ldfsyzlsy\"\r\n",10000);//连接WIFI
	printf("Connect to Beijing Timing...");
	SendATCmd("AT+CIPSTART=\"TCP\",\"www.beijing-time.org\",80\r\n",10000); //连接北京时间服务器
	SendATCmd("AT+CIPSEND=5\r\n",500);			
	SendATCmd("GET\r\n",500);
	osDelay(5000);

}