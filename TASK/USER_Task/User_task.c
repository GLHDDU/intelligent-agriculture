#include "User_task.h"
#include "task.h"
#include "delay.h"
#include "lcd.h"
#include "key.h"
#include "queue.h"
#include "string.h"
#include "user.h"
#include "sensors.h"
#include "hanzi.h"
#include "widget.h"
#include "uart5.h"
#include "rc522.h" 
extern char voive_code[];
int KeyValue;
void key_test(void);
void lcd_sensor_refresh(int temp,int humi,int llu,int soil,int level,int sky,int rain);
void draw_main_page(void);
void lcd_actuator_refresh1(void);//�����������ˢ��ִ������ɫ
extern Sensor_Data_t Sensor_Data;//����������
extern AJ_widget_t AJ_widget_SW;	//���ڿؼ�����


//User������(�˻�����)
void USER_task(void *pvParameters)
{		
	printf("USER_task\r\n");
	

	KEY_Init();							//��ʼ������

	LCD_Init();							//��ʼ��LCD

	LCD_Clear(GRAY);	//���ƻ�ɫ����
	draw_main_page();  //����������
	
	RC522_SPI_Init();//////////Ҫ
	PCD_Init();//////////Ҫ	

	while(1)
	{
		lcd_sensor_refresh(Sensor_Data.Temperature,Sensor_Data.Humidity,Sensor_Data.illuminance,Sensor_Data.Soil_Moisture,Sensor_Data.Water_Level,Sensor_Data.Air_Quality,Sensor_Data.Raindrop);	  //��ʾ��ҳ��
		key_test();			//�������̣�����ִ����ͼ��
			lcd_actuator_refresh1();//�����������ˢ��ִ������ɫ
		  RFID_Product_Roots();//��Դˢ������
		vTaskDelay(100);		
	}
	
}

void lcd_actuator_refresh(int keystrokes[])  //LCDִ��������
 {
		 	switch(keystrokes[0])
					{
						case 1:  //����Ƿ���2����ģʽ
							Draw_word(7+118*3,678,112,112,0xffff,duoji[0]);  //ǰ���Ǹ��ָ�
								Draw_word(7,558,112,112,0x8e388e,paiqishang1[0]);  //ѡ�еģ���ɫ
						break;
						case 2:  //����Ƿ���2����ģʽ
							Draw_word(7,558,112,112,0xffff,paiqishang1[0]);
							Draw_word(7+118,558,112,112,0x8e388e,paiqishang2[0]);
						break;
						case 3:  ////����ǳ�ˮ������ģʽ
							Draw_word(7+118,558,112,112,0xffff,paiqishang2[0]);
							Draw_word(7+118*2,558,112,112,0x8e388e,choushuiji[0]);
						break;
						case 4:  //���������������ģʽ
							Draw_word(7+118*2,558,112,112,0xffff,choushuiji[0]);
							Draw_word(7+118*3,558,112,112,0x8e388e,zhaomingdeng[0]);														
						break;
						case 5:  //����Ǽ�������ģʽ
							Draw_word(7+118*3,558,112,112,0xffff,zhaomingdeng[0]);
							Draw_word(7,678,112,112,0x8e388e,jiaredeng[0]);
						break;
						case 6:  ////����Ǳ���������ģʽ
							Draw_word(7,678,112,112,0xffff,jiaredeng[0]);
							Draw_word(7+118,678,112,112,0x8e388e,jbaodeng[0]);
						break;
								case 7:  //����Ƿ���������ģʽ
							Draw_word(7+118,678,112,112,0xffff,jbaodeng[0]);		
							Draw_word(7+118*2,678,112,112,0x8e388e,fmqi[0]);
						break;
						case 8:  //����Ƕ������ģʽ	
							Draw_word(7+118*2,678,112,112,0xffff,fmqi[0]);
							Draw_word(7+118*3,678,112,112,0x8e388e,duoji[0]);
						break;
						default:
            printf("++key is error\n");
            break;
					}
		
		if(keystrokes[1]==1||AJ_widget_SW.Ventilator1==1)
			Draw_word(7,558,112,112,0xcd2626,paiqishang1[0]);  //ѡ�еģ���ɫ
		else
		  Draw_word(7,558,112,112,0xffff,paiqishang1[0]);  //����ɫ
		if(keystrokes[2]==1||AJ_widget_SW.Ventilator2==1)
			Draw_word(7+118,558,112,112,0xcd2626,paiqishang2[0]);  //ѡ�еģ���ɫ
		else
		  Draw_word(7+118,558,112,112,0xffff,paiqishang2[0]);  //����ɫ
		if(keystrokes[3]==1||AJ_widget_SW.Irrigation_Motor==1)
			Draw_word(7+118*2,558,112,112,0xcd2626,choushuiji[0]); //ѡ�еģ���ɫ
		else
		  Draw_word(7+118*2,558,112,112,0xffff,choushuiji[0]);  //����ɫ
		if(keystrokes[4]==1||AJ_widget_SW.Floodlight==1)
			Draw_word(7+118*3,558,112,112,0xcd2626,zhaomingdeng[0]);  //ѡ�еģ���ɫ
		else
		  Draw_word(7+118*3,558,112,112,0xffff,zhaomingdeng[0]);  //����ɫ
		if(keystrokes[5]==1||AJ_widget_SW.HeatLamp==1)
			Draw_word(7,678,112,112,0xcd2626,jiaredeng[0]); //ѡ�еģ���ɫ
		else
		  Draw_word(7,678,112,112,0xffff,jiaredeng[0]);  //����ɫ
		if(keystrokes[6]==1||AJ_widget_SW.WarningLamp==1)
			Draw_word(7+118,678,112,112,0xcd2626,jbaodeng[0]);  //ѡ�еģ���ɫ
		else
		  Draw_word(7+118,678,112,112,0xffff,jbaodeng[0]);   //����ɫ
		if(keystrokes[7]==1||AJ_widget_SW.Buzzer==1)
			Draw_word(7+118*2,678,112,112,0xcd2626,fmqi[0]);  //ѡ�еģ���ɫ
		else
		  Draw_word(7+118*2,678,112,112,0xffff,fmqi[0]);  //����ɫ
		if(keystrokes[8]==1||AJ_widget_SW.Skylight==1)
			Draw_word(7+118*3,678,112,112,0xcd2626,duoji[0]);  //ѡ�еģ���ɫ
		else
		  Draw_word(7+118*3,678,112,112,0xffff,duoji[0]);  //����ɫ

 }


 void key_test(void)
{
		 vu8 key=0;
static	int keystrokes[9]={0};
key=KEY_Scan(0);	//�õ���ֵ
	   	if(key)
		{						   
			switch(key)
			{				 
				case WKUP_PRES:	//���Ʒ�����
					
					keystrokes[0]++;
				keystrokes[0]=(keystrokes[0]%9);
				printf("WKUP_PRES:%d\r\n",keystrokes[0]);
					break; 
				case KEY1_PRES:	//�ӼӰ�ť	����
					switch(keystrokes[0])
					{
						case 1:  //����Ƿ���1����ģʽ
							keystrokes[1]=!keystrokes[1];
						AJ_widget_SW.Ventilator1 =!AJ_widget_SW.Ventilator1;
						break;
						case 2:  //����Ƿ���2����ģʽ
							keystrokes[2]=!keystrokes[2];
						AJ_widget_SW.Ventilator2 =!AJ_widget_SW.Ventilator2;
						break;
						case 3:  ////����ǳ�ˮ������ģʽ
							keystrokes[3]=!keystrokes[3];
						AJ_widget_SW.Irrigation_Motor =!AJ_widget_SW.Irrigation_Motor;
						break;
						case 4:  //���������������ģʽ
							keystrokes[4]=!keystrokes[4];
						AJ_widget_SW.Floodlight =!AJ_widget_SW.Floodlight;
						break;
						case 5:  //����Ǽ�������ģʽ
							keystrokes[5]=!keystrokes[5];
						AJ_widget_SW.HeatLamp =!AJ_widget_SW.HeatLamp;
						break;
						case 6:  ////����Ǳ���������ģʽ
							keystrokes[6]=!keystrokes[6];
						AJ_widget_SW.WarningLamp =!AJ_widget_SW.WarningLamp;
						break;
								case 7:  //����Ƿ���������ģʽ
							keystrokes[7]=!keystrokes[7];
								AJ_widget_SW.Buzzer =!AJ_widget_SW.Buzzer;
						break;
						case 8:  //����Ƕ������ģʽ
							keystrokes[8]=!keystrokes[8];
						AJ_widget_SW.Skylight =!AJ_widget_SW.Skylight;
						break;
						default:
            printf("++key is error\n");
            break;
					}
					printf("KEY1_PRES:%d,%d\r\n",keystrokes[0],keystrokes[keystrokes[0]]);
					break;
				case KEY0_PRES:	//������ť����
					break;
					
			}
			lcd_actuator_refresh(keystrokes);
		}
		
	}

	
	void draw_main_page(void)
	{
				    //����
        Draw_word(42,22,56,56,0xcd2626,title[0]);
        Draw_word(42+66,22,56,56,0xcd2626,title[1]);
        Draw_word(42+66*2,22,56,56,0xcd2626,title[2]);
        Draw_word(42+66*3,22,56,56,0xcd2626,title[3]);
        Draw_word(42+66*4,22,56,56,0xcd2626,title[4]);
        Draw_word(42+66*5,22,56,56,0xcd2626,title[5]);
		
		LCD_Fill(40, 80, 42+66*4+66, 83, 0XF81F); //��������
		LCD_Fill(42+66, 87, 42+66*6+2, 90, MAGENTA); //��������
				//�����¶�
        Draw_word(100,100,48,48,0xff00,hj_wendu[0]);
        Draw_word(100+48,100,48,48,0xff00,hj_wendu[1]);
        Draw_word(100+48*2,100,48,48,0xff00,hj_wendu[2]);
        Draw_word(100+48*3,100,48,48,0xff00,hj_wendu[3]);
				Draw_word(100+48*4,100,48,48,0xff00,fenghao[0]);
				//����ʪ��
        Draw_word(100,156,48,48,0xff00,hj_shidu[0]);
        Draw_word(100+48,156,48,48,0xff00,hj_shidu[1]);
        Draw_word(100+48*2,156,48,48,0xff00,hj_shidu[2]);
        Draw_word(100+48*3,156,48,48,0xff00,hj_shidu[3]);	
				Draw_word(100+48*4,156,48,48,0xff00,fenghao[0]);
				//��������
        Draw_word(100,212,48,48,0xff00,hj_guanzao[0]);
        Draw_word(100+48,212,48,48,0xff00,hj_guanzao[1]);
        Draw_word(100+48*2,212,48,48,0xff00,hj_guanzao[2]);
        Draw_word(100+48*3,212,48,48,0xff00,hj_guanzao[3]);
				Draw_word(100+48*4,212,48,48,0xff00,fenghao[0]);
				//����ʪ��
        Draw_word(100,268,48,48,0xff00,turanshidu[0]);
        Draw_word(100+48,268,48,48,0xff00,turanshidu[1]);
        Draw_word(100+48*2,268,48,48,0xff00,turanshidu[2]);
        Draw_word(100+48*3,268,48,48,0xff00,turanshidu[3]);
				Draw_word(100+48*4,268,48,48,0xff00,fenghao[0]);
				//ˮλ�߶�
        Draw_word(100,324,48,48,0xff00,shuiweigaodu[0]);
        Draw_word(100+48,324,48,48,0xff00,shuiweigaodu[1]);
        Draw_word(100+48*2,324,48,48,0xff00,shuiweigaodu[2]);
        Draw_word(100+48*3,324,48,48,0xff00,shuiweigaodu[3]);
				Draw_word(100+48*4,324,48,48,0xff00,fenghao[0]);
				//��������
        Draw_word(100,380,48,48,0xff00,kongqizhiliang[0]);
        Draw_word(100+48,380,48,48,0xff00,kongqizhiliang[1]);
        Draw_word(100+48*2,380,48,48,0xff00,kongqizhiliang[2]);
        Draw_word(100+48*3,380,48,48,0xff00,kongqizhiliang[3]);
				Draw_word(100+48*4,380,48,48,0xff00,fenghao[0]);
				//��μ��
        Draw_word(100,436,48,48,0xff00,yudijiance[0]);
        Draw_word(100+48,436,48,48,0xff00,yudijiance[1]);
        Draw_word(100+48*2,436,48,48,0xff00,yudijiance[2]);
        Draw_word(100+48*3,436,48,48,0xff00,yudijiance[3]);
				Draw_word(100+48*4,436,48,48,0xff00,fenghao[0]);
				
				//���ݱ�־����
				Draw_word(24,100,56,56,0xff11,wendu_biao[0]);
				Draw_word(24,156,56,56,0xff11,shidu_biao[0]);
				Draw_word(24,212,56,56,0xff11,guanzao_biao[0]);
				Draw_word(24,268,56,56,0xff11,turan_biao[0]);
				Draw_word(24,324,56,56,0xff11,shuiwei_biao[0]);
				Draw_word(24,380,56,56,0xff11,kongqi_biao[0]);
				Draw_word(24,436,56,56,0xff11,yudi_biao[0]);
						
				
				//ִ����ͼ��
				Draw_word(7,558,112,112,0xffff,paiqishang1[0]);
				Draw_word(7+118,558,112,112,0xffff,paiqishang2[0]);
				Draw_word(7+118*2,558,112,112,0xffff,choushuiji[0]);
				Draw_word(7+118*3,558,112,112,0xffff,zhaomingdeng[0]);
				Draw_word(7,678,112,112,0xffff,jiaredeng[0]);
				Draw_word(7+118,678,112,112,0xffff,jbaodeng[0]);
				Draw_word(7+118*2,678,112,112,0xffff,fmqi[0]);
				Draw_word(7+118*3,678,112,112,0xffff,duoji[0]);
	}
	
	//ˢ��lcd����������
	void lcd_sensor_refresh(int temp,int humi,int llu,int soil,int level,int sky,int rain)
	{
						
				//�¶�����
		    LCD_Fill(100+48*4+24,100 ,100+48*5+24*3,100+48,GRAY);//ˢ����������ı���
		    if(temp<0)
				Draw_word(100+48*4+24,100,24,48,0xff00,fuhao[0]);
				Draw_word(100+48*5,100,24,48,0xff00,shuzhi[(temp/100)%10]);
				Draw_word(100+48*5+24,100,24,48,0xff00,shuzhi[(temp/10)%10]);
				Draw_word(100+48*5+24*2,100,24,48,0xff00,shuzhi[temp%10]);
				//ʪ������
				LCD_Fill(100+48*4+24,156 ,100+48*5+24*3,156+48,GRAY);//ˢ����������ı���
		    if(humi<0)
				Draw_word(100+48*4+24,156,24,48,0xff00,fuhao[0]);
				Draw_word(100+48*5,156,24,48,0xff00,shuzhi[(humi/100)%10]);
				Draw_word(100+48*5+24,156,24,48,0xff00,shuzhi[(humi/10)%10]);
				Draw_word(100+48*5+24*2,156,24,48,0xff00,shuzhi[humi%10]);
				//��������
				LCD_Fill(100+48*4+24,212 ,100+48*5+24*3,212+48,GRAY);//ˢ����������ı���
		    if(llu<0)
				Draw_word(100+48*4+24,212,24,48,0xff00,fuhao[0]);
				Draw_word(100+48*5,212,24,48,0xff00,shuzhi[(llu/100)%10]);
				Draw_word(100+48*5+24,212,24,48,0xff00,shuzhi[(llu/10)%10]);
				Draw_word(100+48*5+24*2,212,24,48,0xff00,shuzhi[llu%10]);
				//����ʪ������
				LCD_Fill(100+48*4+24,268 ,100+48*5+24*3,268+48,GRAY);//ˢ����������ı���
		    if(soil<0)
				Draw_word(100+48*4+24,268,24,48,0xff00,fuhao[0]);
				Draw_word(100+48*5,268,24,48,0xff00,shuzhi[(soil/100)%10]);
				Draw_word(100+48*5+24,268,24,48,0xff00,shuzhi[(soil/10)%10]);
				Draw_word(100+48*5+24*2,268,24,48,0xff00,shuzhi[soil%10]);
				//��λ�߶�����
				LCD_Fill(100+48*4+24,324 ,100+48*5+24*3,324+48,GRAY);//ˢ����������ı���
		    if(level<0)
				Draw_word(100+48*4+24,324,24,48,0xff00,fuhao[0]);
				Draw_word(100+48*5,324,24,48,0xff00,shuzhi[(level/100)%10]);
				Draw_word(100+48*5+24,324,24,48,0xff00,shuzhi[(level/10)%10]);
				Draw_word(100+48*5+24*2,324,24,48,0xff00,shuzhi[level%10]);
				//������������
				LCD_Fill(100+48*4+24,380 ,100+48*5+24*3,380+48,GRAY);//ˢ����������ı���
		    if(sky<0)
				Draw_word(100+48*4+24,380,24,48,0xff00,fuhao[0]);
				Draw_word(100+48*5,380,24,48,0xff00,shuzhi[(sky/100)%10]);
				Draw_word(100+48*5+24,380,24,48,0xff00,shuzhi[(sky/10)%10]);
				Draw_word(100+48*5+24*2,380,24,48,0xff00,shuzhi[sky%10]);
				//����ָ������
				LCD_Fill(100+48*4+24,436 ,100+48*5+24*3,436+48,GRAY);//ˢ����������ı���
		    if(rain<0)
				Draw_word(100+48*4+24,436,24,48,0xff00,fuhao[0]);
				Draw_word(100+48*5,436,24,48,0xff00,shuzhi[(rain/100)%10]);
				Draw_word(100+48*5+24,436,24,48,0xff00,shuzhi[(rain/10)%10]);
				Draw_word(100+48*5+24*2,436,24,48,0xff00,shuzhi[rain%10]);

	}
	
		void lcd_sensorlogo_refresh(int data)
		{
			//��α�־
			if(Sensor_Data.Raindrop > Raindrop_Max)
							Draw_word(24,436,56,56,RED,yudi_biao[0]);
			else
				      Draw_word(24,436,56,56,0xff11,yudi_biao[0]);
			//�����Ƕȱ�־
			if(Sensor_Data.Air_Quality > Air_Quality_Max)
						  Draw_word(24,380,56,56,RED,kongqi_biao[0]);
			else
				      Draw_word(24,380,56,56,0xff11,kongqi_biao[0]);
			//ˮλ��־
			if(Sensor_Data.Water_Level > Water_Level_Max)
							Draw_word(24,324,56,56,RED,shuiwei_biao[0]);
			else
				      Draw_word(24,324,56,56,0xff11,shuiwei_biao[0]);
			 //������־
			if(Sensor_Data.Soil_Moisture > Soil_Moisture_Max)
							Draw_word(24,268,56,56,RED,turan_biao[0]);
			else
				      Draw_word(24,268,56,56,0xff11,turan_biao[0]);
			//���ձ�־
			if(Sensor_Data.illuminance < illuminance_Max)
							Draw_word(24,212,56,56,RED,guanzao_biao[0]);
			else
				      Draw_word(24,212,56,56,0xff11,guanzao_biao[0]);
			//ʪ�ȱ�־
			if(Sensor_Data.Humidity > Humidity_Max)
							Draw_word(24,156,56,56,RED,shidu_biao[0]);
			else
				      Draw_word(24,156,56,56,0xff11,shidu_biao[0]);
			 //�¶ȱ�־
			if((Sensor_Data.Temperature > Temperature_Max)||(Sensor_Data.Temperature < Temperature_Min))
							Draw_word(24,100,56,56,RED,wendu_biao[0]);
			else
				      Draw_word(24,100,56,56,0xff11,wendu_biao[0]);
										
		}
	
void lcd_actuator_refresh1(void)
		{
				if(AJ_widget_SW.Ventilator1==1)
			Draw_word(7,558,112,112,0xcd2626,paiqishang1[0]);  //ѡ�еģ���ɫ
		else
		  Draw_word(7,558,112,112,0xffff,paiqishang1[0]);  //����ɫ
		if(AJ_widget_SW.Ventilator2==1)
			Draw_word(7+118,558,112,112,0xcd2626,paiqishang2[0]);  //ѡ�еģ���ɫ
		else
		  Draw_word(7+118,558,112,112,0xffff,paiqishang2[0]);  //����ɫ
		if(AJ_widget_SW.Irrigation_Motor==1)
			Draw_word(7+118*2,558,112,112,0xcd2626,choushuiji[0]); //ѡ�еģ���ɫ
		else
		  Draw_word(7+118*2,558,112,112,0xffff,choushuiji[0]);  //����ɫ
		if(AJ_widget_SW.Floodlight==1)
			Draw_word(7+118*3,558,112,112,0xcd2626,zhaomingdeng[0]);  //ѡ�еģ���ɫ
		else
		  Draw_word(7+118*3,558,112,112,0xffff,zhaomingdeng[0]);  //����ɫ
		if(AJ_widget_SW.HeatLamp==1)
			Draw_word(7,678,112,112,0xcd2626,jiaredeng[0]); //ѡ�еģ���ɫ
		else
		  Draw_word(7,678,112,112,0xffff,jiaredeng[0]);  //����ɫ
		if(AJ_widget_SW.WarningLamp==1)
			Draw_word(7+118,678,112,112,0xcd2626,jbaodeng[0]);  //ѡ�еģ���ɫ
		else
		  Draw_word(7+118,678,112,112,0xffff,jbaodeng[0]);   //����ɫ
		if(AJ_widget_SW.Buzzer==1)
			Draw_word(7+118*2,678,112,112,0xcd2626,fmqi[0]);  //ѡ�еģ���ɫ
		else
		  Draw_word(7+118*2,678,112,112,0xffff,fmqi[0]);  //����ɫ
		if(AJ_widget_SW.Skylight==1)
			Draw_word(7+118*3,678,112,112,0xcd2626,duoji[0]);  //ѡ�еģ���ɫ
		else
		  Draw_word(7+118*3,678,112,112,0xffff,duoji[0]);  //����ɫ
		}

void RFID_Product_Roots(void)  //RFID��Ʒ��Դ
{
	 uint8_t RxBuffer[4];
   char Card_ID[8];
	char status;
	  PCD_Request(PICC_REQALL, RxBuffer);
    status=PCD_Anticoll(RxBuffer);
    sprintf(Card_ID,"%x%x%x%x",RxBuffer[0],RxBuffer[1],RxBuffer[2],RxBuffer[3]);
    if(status==PCD_OK)
	  printf("ID=%s\r\n",Card_ID);
    
     if(strcmp(Card_ID,"884c5e6")==0)  //��ɫֲ��-õ��
    {
			Voice_control(0x09);
      memset(RxBuffer, 0, sizeof(RxBuffer));
    }
    else if(strcmp(Card_ID,"8849b8")==0)  //ˮ��
    {
			Voice_control(0x0A);
      memset(RxBuffer, 0, sizeof(RxBuffer));
    }
		 else if(strcmp(Card_ID,"884c3f5")==0)  //�ȼ���-�ٺ�
    {
			Voice_control(0x0B);
      memset(RxBuffer, 0, sizeof(RxBuffer));
    }
		 else if(strcmp(Card_ID,"884a89")==0)  //����
    {
			Voice_control(0x0C);
      memset(RxBuffer, 0, sizeof(RxBuffer));
    }
	
	
}
