#ifndef _SENSORS_h_
#define _SENSORS_h_

#include "sys.h"

//�����ָ����ֵ
#define Temperature_Max   25
#define Temperature_Min   20
#define Humidity_Max      65
#define illuminance_Max   80
#define Soil_Moisture_Max 500
#define Water_Level_Max   500
#define Air_Quality_Max   500
#define Raindrop_Max 			200

//����������
typedef struct
{	int Temperature;		//�����¶�
	int Humidity;			//����ʪ��	
	int illuminance;			//����ʪ��	
	int Soil_Moisture; //����ʪ��
	int Water_Level;		//ˮλ	
	int Air_Quality;		//��������
	int Raindrop;			//���

}Sensor_Data_t;


u8 Sensors_Init(void);
u8 Water_Level_Sensor_Init(void);
u8 Soil_Moisture_Sensor_Init(void);
u8 Air_Quality_Sensor_Init(void);
u8 Raindrop_Sensor_Init(void);

void Get_Sensors_Data(Sensor_Data_t *Sensor_Data);
void SensorData_Save(void);
uint16_t SensorData_IsEqual(void);

#endif
