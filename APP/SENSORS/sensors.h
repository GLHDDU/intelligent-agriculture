#ifndef _SENSORS_h_
#define _SENSORS_h_

#include "sys.h"

//各项环境指标阈值
#define Temperature_Max   25
#define Temperature_Min   20
#define Humidity_Max      65
#define illuminance_Max   80
#define Soil_Moisture_Max 500
#define Water_Level_Max   500
#define Air_Quality_Max   500
#define Raindrop_Max 			200

//传感器数据
typedef struct
{	int Temperature;		//空气温度
	int Humidity;			//空气湿度	
	int illuminance;			//空气湿度	
	int Soil_Moisture; //土壤湿度
	int Water_Level;		//水位	
	int Air_Quality;		//空气质量
	int Raindrop;			//雨滴

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
