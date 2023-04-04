#include "Sensors.h"
#include "dht11.h"
#include "timer6.h"
#include "User_task.h"
#include "adc.h"
#include "UART4.h"
#include "USART3.h"
extern u8  USART3_RX_TEMP[USART3_MAX_RECV_LEN]; 
//传感器数据
Sensor_Data_t Sensor_Data;
Sensor_Data_t Last_Sensor_Data;
extern unsigned char USART4_RX_BUF[USART_REC_LEN];
/*所有传感器 初始化*/
u8 Sensors_Init(void)
{	
	//	UART4_Init(115200);
	  usart3_init(115200);
		adc_gpio_init( );  //全部ADC初始化
 	  adc_init( );
	
	while(!(Water_Level_Sensor_Init()&&			//水位传感器 初始化
					Soil_Moisture_Sensor_Init()&&		//土壤湿度传感器 初始化
					Air_Quality_Sensor_Init()&&			//空气质量传感器 初始化
					Raindrop_Sensor_Init()))				//雨滴传感器 初始化
	{
		printf("Sensors initialized fail!\r\n");
	}

	printf("Sensors initialized seccess!\r\n");
	
	return 1;
		
}


/*获取传感器数据*/
void Get_Sensors_Data(Sensor_Data_t *Sensor_Data)
{		
	float adcx;

		//2、获取土壤湿度
		//printf("2、获取土壤湿度..\r\n");
		adcx=read_adc_value(0x05);//模拟赋值	
	printf("adcx:%f\r\n",(float)adcx*(3.3/4096)*100);
	Sensor_Data->Soil_Moisture=(float)adcx*(3.3/4096)*300;	
	
		//1、获取水位数据
	//	printf("1、获取水位数据..\r\n");
	//adcx=read_adc_value(0x05);//模拟赋值	
	Sensor_Data->Water_Level=(float)adcx*(3.3/4096)*100;
	
	//3、获取空气质量
		//printf("3、获取空气质量..\r\n");
//		adcx=read_adc_value(0x05);//模拟赋值	
	Sensor_Data->Air_Quality=(float)adcx*(3.3/4096)*100;

	
	//4、获取雨滴
		//printf("4、获取雨滴..\r\n");	
//			adcx=read_adc_value(0x07);//模拟赋值	
	Sensor_Data->Raindrop=(float)adcx*(3.3/4096)*100;
		Sensor_Data->Temperature=1;
		Sensor_Data->Humidity=1;
		Sensor_Data->illuminance=1;
		Sensor_Data->Soil_Moisture=1;
		Sensor_Data->Water_Level=2;
		Sensor_Data->Air_Quality=3;
		
		sscanf((const char *)USART3_RX_TEMP,"%2d",&(Sensor_Data->Temperature));
		sscanf((const char *)USART3_RX_TEMP+3,"%2d",&(Sensor_Data->Humidity));
		sscanf((const char *)USART3_RX_TEMP+5,"%3d",&(Sensor_Data->illuminance));
		
		sscanf((const char *)USART3_RX_TEMP+11,"%2d",&(Sensor_Data->Soil_Moisture));
		sscanf((const char *)USART3_RX_TEMP+13,"%2d",&(Sensor_Data->Water_Level));
		sscanf((const char *)USART3_RX_TEMP+15,"%2d",&(Sensor_Data->Air_Quality));
		
		Sensor_Data->Soil_Moisture=Sensor_Data->Soil_Moisture*29;
		Sensor_Data->Water_Level=Sensor_Data->Water_Level*29;
		Sensor_Data->Air_Quality=Sensor_Data->Air_Quality*29;
		
  printf("Soil_Moisture:%d\n Water_Level:%d\n Air_Quality:%d\n Raindrop:%d\n\r",
	Sensor_Data->Soil_Moisture,Sensor_Data->Water_Level,Sensor_Data->Air_Quality,Sensor_Data->Raindrop);	
	printf("-------------------------\r\n");
	printf("Temperature:%d\n Humidity:%d\n illuminance:%d\r\n",Sensor_Data->Temperature,Sensor_Data->Humidity,Sensor_Data->illuminance);

}


/*水位传感器 初始化*/
u8 Water_Level_Sensor_Init(void)
{
	
	return 1;//seccess
}

/*土壤湿度传感器 初始化*/
u8 Soil_Moisture_Sensor_Init(void)
{
	
	return 1;//seccess
}

/*空气质量传感器 初始化*/
u8 Air_Quality_Sensor_Init(void)
{
	
	return 1;//seccess
}

/*雨滴传感器 初始化*/
u8 Raindrop_Sensor_Init(void)
{
	
	return 1;//seccess
}


//保存上次传感器数据
void SensorData_Save(void)
{
		Last_Sensor_Data.Air_Quality   =Sensor_Data.Air_Quality;
		Last_Sensor_Data.Raindrop	    =Sensor_Data.Raindrop;
		Last_Sensor_Data.Soil_Moisture =Sensor_Data.Soil_Moisture;
		Last_Sensor_Data.Water_Level   =Sensor_Data.Water_Level;
}

//判断前后两次传感器数据是否相等（只有数据变化才上传）
uint16_t SensorData_IsEqual(void)
{
	if(Sensor_Data.Air_Quality    ==Last_Sensor_Data.Air_Quality&&
		 Sensor_Data.Raindrop		    ==Last_Sensor_Data.Raindrop&&
		 Sensor_Data.Soil_Moisture  ==Last_Sensor_Data.Soil_Moisture&&
		 Sensor_Data.Water_Level		==Last_Sensor_Data.Water_Level)
		return 1;
	else
	  return 0; 
}


