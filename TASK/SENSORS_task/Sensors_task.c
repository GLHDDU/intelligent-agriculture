#include "Sensors_task.h"
#include "queue.h"
#include "main.h"
#include "Sensors.h"
#include "dht11.h"
#include "timer2.h"


 
//������������
void SENSORS_task(void *pvParameters)
{	
		printf("SENSORS_task\r\n");
		//Sensors_Init();//��������ʼ��
		
		while(1)
		{
				SensorData_Save();//�����ϴ�����	 
			
				//Get_Sensors_Data(&Sensor_Data); //��ȡ������������(�Ŷ�ʱ��6�жϷ��������� )

				vTaskDelay(100);		
		}

}



