#ifndef _WIFI_TASK_h_
#define _WIFI_TASK_h_
#include "sys.h"
#include "task.h"
#include "sensors.h"

void WIFI_task(void *pvParameters);
void WIFI_init(void);
void Handle_TxDataBuff(void);
void Handle_RxDataBuff(void);
void Handle_CMDBuf(void);
void ReconnectSever(void);
char *USER_GetSteeringJsonValue(char *cJson, char *Tag);
void Upload_Sensors_Data(Sensor_Data_t *Sensor_Data);
//test
//void TempHumi_State(void);


#endif
