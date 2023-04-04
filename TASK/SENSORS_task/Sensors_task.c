#include "Sensors_task.h"
#include "queue.h"
#include "main.h"
#include "Sensors.h"
#include "dht11.h"
#include "timer2.h"


 
//传感器任务函数
void SENSORS_task(void *pvParameters)
{	
		printf("SENSORS_task\r\n");
		//Sensors_Init();//传感器初始化
		
		while(1)
		{
				SensorData_Save();//保存上次数据	 
			
				//Get_Sensors_Data(&Sensor_Data); //获取传感器新数据(放定时器6中断服务函数里了 )

				vTaskDelay(100);		
		}

}



