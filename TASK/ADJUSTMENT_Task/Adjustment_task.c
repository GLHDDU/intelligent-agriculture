#include "Adjustment_task.h"
#include "queue.h"
#include "main.h"
#include "widget.h"
#include "sensors.h"
#include "lcd.h"
#include "uart5.h"
//����������
extern Sensor_Data_t Sensor_Data;			//����
extern u8 USART5_RX_data[USART5_MAX_RECV_LEN]; 
//���ڿؼ�����	
extern AJ_widget_t AJ_widget_SW;				//����
extern AJ_widget_t Last_AJ_widget_SW;		//�ϴ�

//�Զ����ڱ�־λ
char IsAuto_flash=Manual;
extern char voive_code[];
void AJ_widget_action(AJ_widget_t *widget_Data);
void Voice_request(AJ_widget_t *widget_Data);
//������������
void Adjustment_task(void *pvParameters)
{
	printf("Adjustment_task\r\n");
	
	//��������ʼ��
	while(!Sensors_Init());
	
	//���ڿؼ���ʼ��
	AJ_widgets_Init();
	
	UART5_Configuration();//����������ʼ��
	
	while(1)
	{
		Voice_request(&AJ_widget_SW);  //��Ӧ��������ϵͳ������
		 //�Զ�����
		 if(IsAuto_flash==Auto)
		 {
			 LCD_Fill(0,520 ,78,544,DARKBLUE);//ˢ����������ı���
			 			 /*�¶ȹ��� -������1*/
			 if((Sensor_Data.Temperature > Temperature_Max)&&
					(Last_AJ_widget_SW.Ventilator1 == AJ_OFF))
			 {
				 AJ_widget_SW.Ventilator1 = AJ_ON;
				 Ventilator1_ON();
				 voive_code[3]=Sensor_Data.Temperature;  //��������
				    Voice_control(0x01);
			 } 
			 /*�¶ȹ��� -�����ȵ�*/
			 if(Sensor_Data.Temperature < Temperature_Min&&
					 Last_AJ_widget_SW.HeatLamp == AJ_OFF)
			 {
				 AJ_widget_SW.HeatLamp = AJ_ON;		
         HeatLamp_ON();
           voive_code[3]=Sensor_Data.Temperature;
				    Voice_control(0x02);				 
			 }
			 //�¶�����
			 	 if((Sensor_Data.Temperature < Temperature_Max)&&(Sensor_Data.Temperature >Temperature_Min)&&  //�¶�����
					(Last_AJ_widget_SW.HeatLamp == AJ_ON||Last_AJ_widget_SW.Ventilator1 == AJ_ON))  //��һ������
			 {
				 AJ_widget_SW.Ventilator1 = AJ_OFF;
				Ventilator1_OFF	();
				HeatLamp_OFF	();		
				Voice_control(0x0E);
							 
			 }
			 
			 /*ʪ�ȹ��� -������2*/
			 if((Sensor_Data.Humidity > Humidity_Max)&&
				 Last_AJ_widget_SW.Ventilator2 == AJ_OFF)
			 {
				 AJ_widget_SW.Ventilator2 = AJ_ON;		
				 Ventilator2_ON();
				    voive_code[3]=Sensor_Data.Humidity;
				    Voice_control(0x03);	
			 }
			 /*ʪ������ -�ط���2*/
			 if(Sensor_Data.Humidity < Humidity_Max&&
				 Last_AJ_widget_SW.Ventilator2 == AJ_ON)
			 {
				 AJ_widget_SW.Ventilator2 = AJ_OFF;
Ventilator2_OFF	();			
				 				Voice_control(0x10);
           				 
			 }
			 
			 			 /*���նȹ��� -��������*/
			 if((Sensor_Data.illuminance < illuminance_Max)&&
				 Last_AJ_widget_SW.Floodlight == AJ_OFF)
			 {
				 AJ_widget_SW.Floodlight = AJ_ON;		
				 Floodlight_ON();
				 voive_code[3]=Sensor_Data.illuminance;
				 Voice_control(0x04);	
			 }
			 /*���ն����� -��������*/
			 if(Sensor_Data.illuminance > illuminance_Max&&
				 Last_AJ_widget_SW.Floodlight == AJ_ON)
			 {
				 AJ_widget_SW.Floodlight = AJ_OFF;	
				Floodlight_OFF();		
				Voice_control(0x14);				 
			 }
			 
			  /*����ʪ�ȹ��� -��������*/
			 if((Sensor_Data.Soil_Moisture < Soil_Moisture_Max)&&
				 Last_AJ_widget_SW.Irrigation_Motor == AJ_OFF)
			 {
				 AJ_widget_SW.Irrigation_Motor = AJ_ON;		 
				 Irrigation_Motor_ON();
				    Voice_control(0x05);	
			 }
			 /*����ʪ������ -�ع�����*/
			 if(Sensor_Data.Soil_Moisture > Soil_Moisture_Max&&
				 Last_AJ_widget_SW.Irrigation_Motor == AJ_ON)
			 {
				 AJ_widget_SW.Irrigation_Motor = AJ_OFF;	
				Irrigation_Motor_OFF();
				Voice_control(0x14);				 
			 }
			 
			  /*ˮλ���� -�ع�����*/
			 if((Sensor_Data.Water_Level > Water_Level_Max)&&
				 Last_AJ_widget_SW.Irrigation_Motor == AJ_ON)
			 {
				 AJ_widget_SW.Irrigation_Motor = AJ_OFF;	
				Irrigation_Motor_OFF();		
				    Voice_control(0x06);					 
			 }
			
			 	/*�����Ƕȹ��� -��������*/
			 if((Sensor_Data.Air_Quality > Air_Quality_Max)&&
					(Last_AJ_widget_SW.Buzzer == AJ_OFF))
			 {
				 AJ_widget_SW.Buzzer = AJ_ON;
				 Buzzer_ON();
				    Voice_control(0x07);	
			 } 
			 /*�����Ƕ����� -�ط�����*/
			 if(Sensor_Data.Air_Quality < Air_Quality_Max&&
					 Last_AJ_widget_SW.Buzzer == AJ_ON)
			 {
				 AJ_widget_SW.Buzzer = AJ_OFF;
				 Buzzer_OFF();
				 Voice_control(0x1A);
			 }
			 
			 	/*���ָ������ -�����*/
			 if((Sensor_Data.Raindrop > Raindrop_Max)&&
					(Last_AJ_widget_SW.Skylight == AJ_OFF))
			 {
				 AJ_widget_SW.Skylight = AJ_ON;
				 Open_Skylight();
				    Voice_control(0x08);	
			 } 
			 /*���ָ������ -�ض��*/
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
					//Voice_control(0x01);	//�Ḳ��֮ǰδ˵��Ļ�
				}
				else
				{
					AJ_widget_SW.WarningLamp = AJ_OFF;
					WarningLamp_OFF();
					Voice_control(0x18);
				}
		 }
		 
		 		 //�ֶ�����///////////////////////////////������������ƽ̨
		 if(IsAuto_flash==Manual)
		 {
			 LCD_Fill(0,520,78,544,BRRED);//ˢ����������ı���
			 //��������1
				if((AJ_widget_SW.Ventilator1 == AJ_ON)&&
					 (Last_AJ_widget_SW.Ventilator1 == AJ_OFF))
				{
					Ventilator1_ON();
				  Voice_control(0x0d);
				}
			 //��������1
				if((AJ_widget_SW.Ventilator1 == AJ_OFF)&&
					 (Last_AJ_widget_SW.Ventilator1 == AJ_ON))
				{
					Ventilator1_OFF();
				    Voice_control(0x0E);
				}

				
			 //��������2
				if((AJ_widget_SW.Ventilator2 == AJ_ON)&&
					 (Last_AJ_widget_SW.Ventilator2 == AJ_OFF))
				{
					Ventilator2_ON();
				    Voice_control(0x0F);
				}
			 //��������2
				if((AJ_widget_SW.Ventilator2 == AJ_OFF)&&
					 (Last_AJ_widget_SW.Ventilator2 == AJ_ON))
				{
					Ventilator2_OFF();
				    Voice_control(0x10);
				}	

				
			 //���촰
				if((AJ_widget_SW.Skylight == AJ_ON)&&
					 (Last_AJ_widget_SW.Skylight == AJ_OFF))
				{
					Open_Skylight();
				    Voice_control(0x1B);
				}
			 //���촰
				if((AJ_widget_SW.Skylight == AJ_OFF)&&
					 (Last_AJ_widget_SW.Skylight == AJ_ON))
				{
					Close_Skylight();
				    Voice_control(0x1C);
				}	
		
				
			 //��������
				if((AJ_widget_SW.Irrigation_Motor == AJ_ON)&&
					 (Last_AJ_widget_SW.Irrigation_Motor == AJ_OFF))
				{
					Irrigation_Motor_ON();
				    Voice_control(0x11);
				}
			 //�ع�����
				if((AJ_widget_SW.Irrigation_Motor == AJ_OFF)&&
					 (Last_AJ_widget_SW.Irrigation_Motor == AJ_ON))
				{
					Irrigation_Motor_OFF();
				    Voice_control(0x12);
				}	

       //��������
				if((AJ_widget_SW.Floodlight == AJ_ON)&&
					 (Last_AJ_widget_SW.Floodlight == AJ_OFF))
				{
					Floodlight_ON();
				    Voice_control(0x13);
				}
			 //��������
				if((AJ_widget_SW.Floodlight == AJ_OFF)&&
					 (Last_AJ_widget_SW.Floodlight == AJ_ON))
				{
					Floodlight_OFF();
					Voice_control(0x14);
				}	

       //�����ȵ�
				if((AJ_widget_SW.HeatLamp == AJ_ON)&&
					 (Last_AJ_widget_SW.HeatLamp == AJ_OFF))
				{
					HeatLamp_ON();
					Voice_control(0x15);
				}
			 //�ؼ��ȵ�
				if((AJ_widget_SW.HeatLamp == AJ_OFF)&&
					 (Last_AJ_widget_SW.HeatLamp == AJ_ON))
				{
					HeatLamp_OFF();
					Voice_control(0x16);
				}	

       //��������
				if((AJ_widget_SW.WarningLamp == AJ_ON)&&
					 (Last_AJ_widget_SW.WarningLamp == AJ_OFF))
				{
					WarningLamp_ON();
					Voice_control(0x17);
				}
			 //�ؾ�����
				if((AJ_widget_SW.WarningLamp == AJ_OFF)&&
					 (Last_AJ_widget_SW.WarningLamp == AJ_ON))
				{
					WarningLamp_OFF();
					Voice_control(0x18);
				}	

				//��������
				if((AJ_widget_SW.Buzzer == AJ_ON)&&
					 (Last_AJ_widget_SW.Buzzer == AJ_OFF))
				{
					Buzzer_ON();
					Voice_control(0x19);
				}
			 //�ط�����
				if((AJ_widget_SW.Buzzer == AJ_OFF)&&
					 (Last_AJ_widget_SW.Buzzer == AJ_ON))
				{
					Buzzer_OFF();
					Voice_control(0x1A);
				}					
				
				//printf("Manual....\r\n");				
		 }
		 
		 	 
		 //�����ϴοؼ����ص�״̬
		 if(!AJ_widget_SW_IsEqual())
		 {  
			 printf("AJ_widget_SW_Save\r\n");
			 AJ_widget_SW_Save();		 
		 }
			vTaskDelay(500);
	}

}

//�����ϴοؼ����ص�״̬
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

//�ж�ǰ�����οؼ����ص�״̬�Ƿ����
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




////��Ӧ��������ϵͳ������
void Voice_request(AJ_widget_t *widget_Data)
{
	if(USART5_RX_data[3]==0x01)  //��������1
	{
		widget_Data->Ventilator1=AJ_ON;
		USART5_RX_data[3]=0x00;
	}
	if(USART5_RX_data[3]==0x02)  //��������1
	{
		widget_Data->Ventilator1=AJ_OFF;
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x03)  //��������2
	{
		widget_Data->Ventilator2=AJ_ON;
		USART5_RX_data[3]=0x00;
	}
	if(USART5_RX_data[3]==0x04)  //��������2
	{
		widget_Data->Ventilator2=AJ_OFF;
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x05)  //��������
	{
		widget_Data->Irrigation_Motor=AJ_ON;
		USART5_RX_data[3]=0x00;
	}
	if(USART5_RX_data[3]==0x06)  //�ع�����
	{
		widget_Data->Irrigation_Motor=AJ_OFF;
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x07)   //����ɫ���ȵ�
	{
		widget_Data->HeatLamp=AJ_ON;
		USART5_RX_data[3]=0x00;
	}
	if(USART5_RX_data[3]==0x08)  //�غ�ɫ���ȵ�
	{
		widget_Data->HeatLamp=AJ_OFF;
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x09)  //����ɫ������
	{
		widget_Data->WarningLamp=AJ_ON;
		USART5_RX_data[3]=0x00;
	}
	if(USART5_RX_data[3]==0x10)  //����ɫ������
	{
		widget_Data->WarningLamp=AJ_OFF;
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x011)  //����ɫ������
	{
		widget_Data->Floodlight=AJ_ON;
		USART5_RX_data[3]=0x00;
	}
	if(USART5_RX_data[3]==0x12)  //����ɫ������
	{
		widget_Data->Floodlight=AJ_OFF;
		USART5_RX_data[3]=0x00;
	}
	
		if(USART5_RX_data[3]==0x013)  //��������
	{
		widget_Data->Buzzer=AJ_ON;
		USART5_RX_data[3]=0x00;
	}
	if(USART5_RX_data[3]==0x14)  //�ط�����
	{
		widget_Data->Buzzer=AJ_OFF;
		USART5_RX_data[3]=0x00;
	}
	
			if(USART5_RX_data[3]==0x15)  //���촰
	{
		widget_Data->Skylight=AJ_ON;
		USART5_RX_data[3]=0x00;
	}
	if(USART5_RX_data[3]==0x16)  //���촰
	{
		widget_Data->Skylight=AJ_OFF;
		USART5_RX_data[3]=0x00;
	}
}
