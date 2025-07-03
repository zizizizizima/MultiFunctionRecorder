#ifndef __ESP8266_H
#define __ESP8266_H

void SendATCmd(char *cmd, int waitms);
void FetchServer(void);
#endif