#include "Adjustment_task.h"
#include "queue.h"
#include "main.h"
#include "widget.h"
#include "sensors.h"
#include "lcd.h"
#include "uart5.h"
//传感器数据
extern Sensor_Data_t Sensor_Data;			//本次
extern u8 USART5_RX_data[USART5_MAX_RECV_LEN]; 
//调节控件开关	
extern AJ_widget_t AJ_widget_SW;				//本次
extern AJ_widget_t Last_AJ_widget_SW;		//上次

//自动调节标志位
char IsAuto_flash=Manual;
extern char voive_code[];
void AJ_widget_action(AJ_widget_t *widget_Data);
void Voice_request(AJ_widget_t *widget_Data);
//传感器任务函数
void Adjustment_task(void *pvParameters)
{
	printf("Adjustment_task\r\n");
	
	//传感器初始化
	while(!Sensors_Init());
	
	//调节控件初始化
	AJ_widgets_Init();
	
	UART5_Configuration();//语音播报初始化
	
	while(1)
	{
		Voice_request(&AJ_widget_SW);  //响应来自语音系统的命令
		 //自动调节
		 if(IsAuto_flash==Auto)
		 {
			 LCD_Fill(0,520 ,78,544,DARKBLUE);//刷新数字区域的背景
			 			 /*温度过高 -开风扇1*/
			 if((Sensor_Data.Temperature > Temperature_Max)&&
					(Last_AJ_widget_SW.Ventilator1 == AJ_OFF))
			 {
				 AJ_widget_SW.Ventilator1 = AJ_ON;
				 Ventilator1_ON();
				 voive_code[3]=Sensor_Data.Temperature;  //语音警告
				    Voice_control(0x01);
			 } 
			 /*温度过低 -开加热灯*/
			 if(Sensor_Data.Temperature < Temperature_Min&&
					 Last_AJ_widget_SW.HeatLamp == AJ_OFF)
			 {
				 AJ_widget_SW.HeatLamp = AJ_ON;		
         HeatLamp_ON();
           voive_code[3]=Sensor_Data.Temperature;
				    Voice_control(0x02);				 
			 }
			 //温度适宜
			 	 if((Sensor_Data.Temperature < Temperature_Max)&&(Sensor_Data.Temperature >Temperature_Min)&&  //温度适宜
					(Last_AJ_widget_SW.HeatLamp == AJ_ON||Last_AJ_widget_SW.Ventilator1 == AJ_ON))  //有一个开着
			 {
				 AJ_widget_SW.Ventilator1 = AJ_OFF;
				Ventilator1_OFF	();
				HeatLamp_OFF	();		
				Voice_control(0x0E);
							 
			 }
			 
			 /*湿度过高 -开风扇2*/
			 if((Sensor_Data.Humidity > Humidity_Max)&&
				 Last_AJ_widget_SW.Ventilator2 == AJ_OFF)
			 {
				 AJ_widget_SW.Ventilator2 = AJ_ON;		
				 Ventilator2_ON();
				    voive_code[3]=Sensor_Data.Humidity;
				    Voice_control(0x03);	
			 }
			 /*湿度正常 -关风扇2*/
			 if(Sensor_Data.Humidity < Humidity_Max&&
				 Last_AJ_widget_SW.Ventilator2 == AJ_ON)
			 {
				 AJ_widget_SW.Ventilator2 = AJ_OFF;
Ventilator2_OFF	();			
				 				Voice_control(0x10);
           				 
			 }
			 
			 			 /*光照度过低 -开照明灯*/
			 if((Sensor_Data.illuminance < illuminance_Max)&&
				 Last_AJ_widget_SW.Floodlight == AJ_OFF)
			 {
				 AJ_widget_SW.Floodlight = AJ_ON;		
				 Floodlight_ON();
				 voive_code[3]=Sensor_Data.illuminance;
				 Voice_control(0x04);	
			 }
			 /*光照度正常 -关照明灯*/
			 if(Sensor_Data.illuminance > illuminance_Max&&
				 Last_AJ_widget_SW.Floodlight == AJ_ON)
			 {
				 AJ_widget_SW.Floodlight = AJ_OFF;	
				Floodlight_OFF();		
				Voice_control(0x14);				 
			 }
			 
			  /*土壤湿度过低 -开灌溉马达*/
			 if((Sensor_Data.Soil_Moisture < Soil_Moisture_Max)&&
				 Last_AJ_widget_SW.Irrigation_Motor == AJ_OFF)
			 {
				 AJ_widget_SW.Irrigation_Motor = AJ_ON;		 
				 Irrigation_Motor_ON();
				    Voice_control(0x05);	
			 }
			 /*土壤湿度正常 -关灌溉马达*/
			 if(Sensor_Data.Soil_Moisture > Soil_Moisture_Max&&
				 Last_AJ_widget_SW.Irrigation_Motor == AJ_ON)
			 {
				 AJ_widget_SW.Irrigation_Motor = AJ_OFF;	
				Irrigation_Motor_OFF();
				Voice_control(0x14);				 
			 }
			 
			  /*水位过高 -关灌溉马达*/
			 if((Sensor_Data.Water_Level > Water_Level_Max)&&
				 Last_AJ_widget_SW.Irrigation_Motor == AJ_ON)
			 {
				 AJ_widget_SW.Irrigation_Motor = AJ_OFF;	
				Irrigation_Motor_OFF();		
				    Voice_control(0x06);					 
			 }
			
			 	/*空气浊度过高 -开蜂鸣器*/
			 if((Sensor_Data.Air_Quality > Air_Quality_Max)&&
					(Last_AJ_widget_SW.Buzzer == AJ_OFF))
			 {
				 AJ_widget_SW.Buzzer = AJ_ON;
				 Buzzer_ON();
				    Voice_control(0x07);	
			 } 
			 /*空气浊度正常 -关蜂鸣器*/
			 if(Sensor_Data.Air_Quality < Air_Quality_Max&&
					 Last_AJ_widget_SW.Buzzer == AJ_ON)
			 {
				 AJ_widget_SW.Buzzer = AJ_OFF;
				 Buzzer_OFF();
				 Voice_control(0x1A);
			 }
			 
			 	/*雨滴指数正常 -开舵机*/
			 if((Sensor_Data.Raindrop > Raindrop_Max)&&
					(Last_AJ_widget_SW.Skylight == AJ_OFF))
			 {
				 AJ_widget_SW.Skylight = AJ_ON;
				 Open_Skylight();
				    Voice_control(0x08);	
			 } 
			 /*雨滴指数过高 -关舵机*/
			 if(Sensor_Data.Raindrop < Raindrop_Max&&
					 Last_AJ_widget_SW.Skylight == AJ_ON)
			 {
				 AJ_widget_SW.Skylight = AJ_OFF;
				 Close_Skylight();
				 Voice_control(0x1C);
			 }
			 
			 	if(AJ_widget_SW.Ventilator1||AJ_widget_SW.Ventilator2||AJ_widget_SW.Skylight||AJ_widget_SW.Irrigation_Motor||
					AJ_widget_SW.Floodlight||AJ_widget_SW.HeatLamp||AJ_widget_SW.WarningLamp||AJ_widget_SW.Buzzer)
				{
					AJ_widget_SW.WarningLamp = AJ_ON;
					WarningLamp_ON();
					//Voice_control(0x01);	//会覆盖之前未说完的话
				}
				else
				{
					AJ_widget_SW.WarningLamp = AJ_OFF;
					WarningLamp_OFF();
					Voice_control(0x18);
				}
		 }
		 
		 		 //手动调节///////////////////////////////按键或者是云平台
		 if(IsAuto_flash==Manual)
		 {
			 LCD_Fill(0,520,78,544,BRRED);//刷新数字区域的背景
			 //开排气扇1
				if((AJ_widget_SW.Ventilator1 == AJ_ON)&&
					 (Last_AJ_widget_SW.Ventilator1 == AJ_OFF))
				{
					Ventilator1_ON();
				  Voice_control(0x0d);
				}
			 //关排气扇1
				if((AJ_widget_SW.Ventilator1 == AJ_OFF)&&
					 (Last_AJ_widget_SW.Ventilator1 == AJ_ON))
				{
					Ventilator1_OFF();
				    Voice_control(0x0E);
				}

				
			 //开排气扇2
				if((AJ_widget_SW.Ventilator2 == AJ_ON)&&
					 (Last_AJ_widget_SW.Ventilator2 == AJ_OFF))
				{
					Ventilator2_ON();
				    Voice_control(0x0F);
				}
			 //关排气扇2
				if((AJ_widget_SW.Ventilator2 == AJ_OFF)&&
					 (Last_AJ_widget_SW.Ventilator2 == AJ_ON))
				{
					Ventilator2_OFF();
				    Voice_control(0x10);
				}	

				
			 //开天窗
				if((AJ_widget_SW.Skylight == AJ_ON)&&
					 (Last_AJ_widget_SW.Skylight == AJ_OFF))
				{
					Open_Skylight();
				    Voice_control(0x1B);
				}
			 //关天窗
				if((AJ_widget_SW.Skylight == AJ_OFF)&&
					 (Last_AJ_widget_SW.Skylight == AJ_ON))
				{
					Close_Skylight();
				    Voice_control(0x1C);
				}	
		
				
			 //开灌溉马达
				if((AJ_widget_SW.Irrigation_Motor == AJ_ON)&&
					 (Last_AJ_widget_SW.Irrigation_Motor == AJ_OFF))
				{
					Irrigation_Motor_ON();
				    Voice_control(0x11);
				}
			 //关灌溉马达
				if((AJ_widget_SW.Irrigation_Motor == AJ_OFF)&&
					 (Last_AJ_widget_SW.Irrigation_Motor == AJ_ON))
				{
					Irrigation_Motor_OFF();
				    Voice_control(0x12);
				}	

       //开照明灯
				if((AJ_widget_SW.Floodlight == AJ_ON)&&
					 (Last_AJ_widget_SW.Floodlight == AJ_OFF))
				{
					Floodlight_ON();
				    Voice_control(0x13);
				}
			 //关照明灯
				if((AJ_widget_SW.Floodlight == AJ_OFF)&&
					 (Last_AJ_widget_SW.Floodlight == AJ_ON))
				{
					Floodlight_OFF();
					Voice_control(0x14);
				}	

       //开加热灯
				if((AJ_widget_SW.HeatLamp == AJ_ON)&&
					 (Last_AJ_widget_SW.HeatLamp == AJ_OFF))
				{
					HeatLamp_ON();
					Voice_control(0x15);
				}
			 //关加热灯
				if((AJ_widget_SW.HeatLamp == AJ_OFF)&&
					 (Last_AJ_widget_SW.HeatLamp == AJ_ON))
				{
					HeatLamp_OFF();
					Voice_control(0x16);
				}	

       //开警报灯
				if((AJ_widget_SW.WarningLamp == AJ_ON)&&
					 (Last_AJ_widget_SW.WarningLamp == AJ_OFF))
				{
					WarningLamp_ON();
					Voice_control(0x17);
				}
			 //关警报灯
				if((AJ_widget_SW.WarningLamp == AJ_OFF)&&
					 (Last_AJ_widget_SW.WarningLamp == AJ_ON))
				{
					WarningLamp_OFF();
					Voice_control(0x18);
				}	

				//开蜂鸣器
				if((AJ_widget_SW.Buzzer == AJ_ON)&&
					 (Last_AJ_widget_SW.Buzzer == AJ_OFF))
				{
					Buzzer_ON();
					Voice_control(0x19);
				}
			 //关蜂鸣器
				if((AJ_widget_SW.Buzzer == AJ_OFF)&&
					 (Last_AJ_widget_SW.Buzzer == AJ_ON))
				{
					Buzzer_OFF();
					Voice_control(0x1A);
				}					
				
				//printf("Manual....\r\n");				
		 }
		 
		 	 
		 //保存上次控件开关的状态
		 if(!AJ_widget_SW_IsEqual())
		 {  
			 printf("AJ_widget_SW_Save\r\n");
			 AJ_widget_SW_Save();		 
		 }
			vTaskDelay(500);
	}

}

//保存上次控件开关的状态
void AJ_widget_SW_Save(void)
{
		Last_AJ_widget_SW.Ventilator1   	 =AJ_widget_SW.Ventilator1;
		Last_AJ_widget_SW.Ventilator2	 		 =AJ_widget_SW.Ventilator2;
		Last_AJ_widget_SW.Skylight 				 =AJ_widget_SW.Skylight;
		Last_AJ_widget_SW.Irrigation_Motor =AJ_widget_SW.Irrigation_Motor;
	  Last_AJ_widget_SW.Floodlight       =AJ_widget_SW.Floodlight;
	  Last_AJ_widget_SW.HeatLamp         =AJ_widget_SW.HeatLamp;
	  Last_AJ_widget_SW.WarningLamp      =AJ_widget_SW.WarningLamp;
	  Last_AJ_widget_SW.Buzzer           =AJ_widget_SW.Buzzer;
}

//判断前后两次控件开关的状态是否相等
uint16_t AJ_widget_SW_IsEqual(void)
{
	if(Last_AJ_widget_SW.Ventilator1   	   ==AJ_widget_SW.Ventilator1&&
		 Last_AJ_widget_SW.Ventilator2	 		 ==AJ_widget_SW.Ventilator2&&
		 Last_AJ_widget_SW.Skylight 				 ==AJ_widget_SW.Skylight&&
		 Last_AJ_widget_SW.Irrigation_Motor  ==AJ_widget_SW.Irrigation_Motor&&
		 Last_AJ_widget_SW.Floodlight        ==AJ_widget_SW.Floodlight&&
	   Last_AJ_widget_SW.HeatLamp          ==AJ_widget_SW.HeatLamp&&
	   Last_AJ_widget_SW.WarningLamp       ==AJ_widget_SW.WarningLamp&&
	   Last_AJ_widget_SW.Buzzer            ==AJ_widget_SW.Buzzer)
		return 1;
	else
	  return 0; 
}




////响应来自语音系统的命令
void Voice_request(AJ_widget_t *widget_Data)
{
	if(USART5_RX_data[3]==0x01)  //开排气扇1
	{
		widget_Data->Ventilator1=AJ_ON;
		USART5_RX_data[3]=0x00;
	}
	if(USART5_RX_data[3]==0x02)  //关排气扇1
	{
		widget_Data->Ventilator1=AJ_OFF;
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x03)  //开排气扇2
	{
		widget_Data->Ventilator2=AJ_ON;
		USART5_RX_data[3]=0x00;
	}
	if(USART5_RX_data[3]==0x04)  //关排气扇2
	{
		widget_Data->Ventilator2=AJ_OFF;
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x05)  //开灌溉马达
	{
		widget_Data->Irrigation_Motor=AJ_ON;
		USART5_RX_data[3]=0x00;
	}
	if(USART5_RX_data[3]==0x06)  //关灌溉马达
	{
		widget_Data->Irrigation_Motor=AJ_OFF;
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x07)   //开红色加热灯
	{
		widget_Data->HeatLamp=AJ_ON;
		USART5_RX_data[3]=0x00;
	}
	if(USART5_RX_data[3]==0x08)  //关红色加热灯
	{
		widget_Data->HeatLamp=AJ_OFF;
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x09)  //开绿色警报灯
	{
		widget_Data->WarningLamp=AJ_ON;
		USART5_RX_data[3]=0x00;
	}
	if(USART5_RX_data[3]==0x10)  //关绿色警报灯
	{
		widget_Data->WarningLamp=AJ_OFF;
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x011)  //开蓝色照明灯
	{
		widget_Data->Floodlight=AJ_ON;
		USART5_RX_data[3]=0x00;
	}
	if(USART5_RX_data[3]==0x12)  //关蓝色照明灯
	{
		widget_Data->Floodlight=AJ_OFF;
		USART5_RX_data[3]=0x00;
	}
	
		if(USART5_RX_data[3]==0x013)  //开蜂鸣器
	{
		widget_Data->Buzzer=AJ_ON;
		USART5_RX_data[3]=0x00;
	}
	if(USART5_RX_data[3]==0x14)  //关蜂鸣器
	{
		widget_Data->Buzzer=AJ_OFF;
		USART5_RX_data[3]=0x00;
	}
	
			if(USART5_RX_data[3]==0x15)  //开天窗
	{
		widget_Data->Skylight=AJ_ON;
		USART5_RX_data[3]=0x00;
	}
	if(USART5_RX_data[3]==0x16)  //关天窗
	{
		widget_Data->Skylight=AJ_OFF;
		USART5_RX_data[3]=0x00;
	}
}
