#include "Sensors.h"
#include "dht11.h"
#include "timer6.h"
#include "User_task.h"
#include "adc.h"
#include "UART4.h"
#include "USART3.h"
extern u8  USART3_RX_TEMP[USART3_MAX_RECV_LEN]; 
//����������
Sensor_Data_t Sensor_Data;
Sensor_Data_t Last_Sensor_Data;
extern unsigned char USART4_RX_BUF[USART_REC_LEN];
/*���д����� ��ʼ��*/
u8 Sensors_Init(void)
{	
	//	UART4_Init(115200);
	  usart3_init(115200);
		adc_gpio_init( );  //ȫ��ADC��ʼ��
 	  adc_init( );
	
	while(!(Water_Level_Sensor_Init()&&			//ˮλ������ ��ʼ��
					Soil_Moisture_Sensor_Init()&&		//����ʪ�ȴ����� ��ʼ��
					Air_Quality_Sensor_Init()&&			//�������������� ��ʼ��
					Raindrop_Sensor_Init()))				//��δ����� ��ʼ��
	{
		printf("Sensors initialized fail!\r\n");
	}

	printf("Sensors initialized seccess!\r\n");
	
	return 1;
		
}


/*��ȡ����������*/
void Get_Sensors_Data(Sensor_Data_t *Sensor_Data)
{		
	float adcx;

		//2����ȡ����ʪ��
		//printf("2����ȡ����ʪ��..\r\n");
		adcx=read_adc_value(0x05);//ģ�⸳ֵ	
	printf("adcx:%f\r\n",(float)adcx*(3.3/4096)*100);
	Sensor_Data->Soil_Moisture=(float)adcx*(3.3/4096)*300;	
	
		//1����ȡˮλ����
	//	printf("1����ȡˮλ����..\r\n");
	//adcx=read_adc_value(0x05);//ģ�⸳ֵ	
	Sensor_Data->Water_Level=(float)adcx*(3.3/4096)*100;
	
	//3����ȡ��������
		//printf("3����ȡ��������..\r\n");
//		adcx=read_adc_value(0x05);//ģ�⸳ֵ	
	Sensor_Data->Air_Quality=(float)adcx*(3.3/4096)*100;

	
	//4����ȡ���
		//printf("4����ȡ���..\r\n");	
//			adcx=read_adc_value(0x07);//ģ�⸳ֵ	
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


/*ˮλ������ ��ʼ��*/
u8 Water_Level_Sensor_Init(void)
{
	
	return 1;//seccess
}

/*����ʪ�ȴ����� ��ʼ��*/
u8 Soil_Moisture_Sensor_Init(void)
{
	
	return 1;//seccess
}

/*�������������� ��ʼ��*/
u8 Air_Quality_Sensor_Init(void)
{
	
	return 1;//seccess
}

/*��δ����� ��ʼ��*/
u8 Raindrop_Sensor_Init(void)
{
	
	return 1;//seccess
}


//�����ϴδ���������
void SensorData_Save(void)
{
		Last_Sensor_Data.Air_Quality   =Sensor_Data.Air_Quality;
		Last_Sensor_Data.Raindrop	    =Sensor_Data.Raindrop;
		Last_Sensor_Data.Soil_Moisture =Sensor_Data.Soil_Moisture;
		Last_Sensor_Data.Water_Level   =Sensor_Data.Water_Level;
}

//�ж�ǰ�����δ����������Ƿ���ȣ�ֻ�����ݱ仯���ϴ���
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


