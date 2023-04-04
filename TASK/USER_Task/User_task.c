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
void lcd_actuator_refresh1(void);//根据语音结果刷新执行器颜色
extern Sensor_Data_t Sensor_Data;//传感器数据
extern AJ_widget_t AJ_widget_SW;	//调节控件开关


//User任务函数(人机交互)
void USER_task(void *pvParameters)
{		
	printf("USER_task\r\n");
	

	KEY_Init();							//初始化按键

	LCD_Init();							//初始化LCD

	LCD_Clear(GRAY);	//绘制灰色背景
	draw_main_page();  //绘制主界面
	
	RC522_SPI_Init();//////////要
	PCD_Init();//////////要	

	while(1)
	{
		lcd_sensor_refresh(Sensor_Data.Temperature,Sensor_Data.Humidity,Sensor_Data.illuminance,Sensor_Data.Soil_Moisture,Sensor_Data.Water_Level,Sensor_Data.Air_Quality,Sensor_Data.Raindrop);	  //显示主页面
		key_test();			//按键进程，控制执行器图标
			lcd_actuator_refresh1();//根据语音结果刷新执行器颜色
		  RFID_Product_Roots();//溯源刷卡函数
		vTaskDelay(100);		
	}
	
}

void lcd_actuator_refresh(int keystrokes[])  //LCD执行器更新
 {
		 	switch(keystrokes[0])
					{
						case 1:  //如果是风扇2设置模式
							Draw_word(7+118*3,678,112,112,0xffff,duoji[0]);  //前面那个恢复
								Draw_word(7,558,112,112,0x8e388e,paiqishang1[0]);  //选中的，变色
						break;
						case 2:  //如果是风扇2设置模式
							Draw_word(7,558,112,112,0xffff,paiqishang1[0]);
							Draw_word(7+118,558,112,112,0x8e388e,paiqishang2[0]);
						break;
						case 3:  ////如果是抽水机设置模式
							Draw_word(7+118,558,112,112,0xffff,paiqishang2[0]);
							Draw_word(7+118*2,558,112,112,0x8e388e,choushuiji[0]);
						break;
						case 4:  //如果是照明灯设置模式
							Draw_word(7+118*2,558,112,112,0xffff,choushuiji[0]);
							Draw_word(7+118*3,558,112,112,0x8e388e,zhaomingdeng[0]);														
						break;
						case 5:  //如果是加热设置模式
							Draw_word(7+118*3,558,112,112,0xffff,zhaomingdeng[0]);
							Draw_word(7,678,112,112,0x8e388e,jiaredeng[0]);
						break;
						case 6:  ////如果是报警灯设置模式
							Draw_word(7,678,112,112,0xffff,jiaredeng[0]);
							Draw_word(7+118,678,112,112,0x8e388e,jbaodeng[0]);
						break;
								case 7:  //如果是蜂鸣器设置模式
							Draw_word(7+118,678,112,112,0xffff,jbaodeng[0]);		
							Draw_word(7+118*2,678,112,112,0x8e388e,fmqi[0]);
						break;
						case 8:  //如果是舵机设置模式	
							Draw_word(7+118*2,678,112,112,0xffff,fmqi[0]);
							Draw_word(7+118*3,678,112,112,0x8e388e,duoji[0]);
						break;
						default:
            printf("++key is error\n");
            break;
					}
		
		if(keystrokes[1]==1||AJ_widget_SW.Ventilator1==1)
			Draw_word(7,558,112,112,0xcd2626,paiqishang1[0]);  //选中的，变色
		else
		  Draw_word(7,558,112,112,0xffff,paiqishang1[0]);  //不变色
		if(keystrokes[2]==1||AJ_widget_SW.Ventilator2==1)
			Draw_word(7+118,558,112,112,0xcd2626,paiqishang2[0]);  //选中的，变色
		else
		  Draw_word(7+118,558,112,112,0xffff,paiqishang2[0]);  //不变色
		if(keystrokes[3]==1||AJ_widget_SW.Irrigation_Motor==1)
			Draw_word(7+118*2,558,112,112,0xcd2626,choushuiji[0]); //选中的，变色
		else
		  Draw_word(7+118*2,558,112,112,0xffff,choushuiji[0]);  //不变色
		if(keystrokes[4]==1||AJ_widget_SW.Floodlight==1)
			Draw_word(7+118*3,558,112,112,0xcd2626,zhaomingdeng[0]);  //选中的，变色
		else
		  Draw_word(7+118*3,558,112,112,0xffff,zhaomingdeng[0]);  //不变色
		if(keystrokes[5]==1||AJ_widget_SW.HeatLamp==1)
			Draw_word(7,678,112,112,0xcd2626,jiaredeng[0]); //选中的，变色
		else
		  Draw_word(7,678,112,112,0xffff,jiaredeng[0]);  //不变色
		if(keystrokes[6]==1||AJ_widget_SW.WarningLamp==1)
			Draw_word(7+118,678,112,112,0xcd2626,jbaodeng[0]);  //选中的，变色
		else
		  Draw_word(7+118,678,112,112,0xffff,jbaodeng[0]);   //不变色
		if(keystrokes[7]==1||AJ_widget_SW.Buzzer==1)
			Draw_word(7+118*2,678,112,112,0xcd2626,fmqi[0]);  //选中的，变色
		else
		  Draw_word(7+118*2,678,112,112,0xffff,fmqi[0]);  //不变色
		if(keystrokes[8]==1||AJ_widget_SW.Skylight==1)
			Draw_word(7+118*3,678,112,112,0xcd2626,duoji[0]);  //选中的，变色
		else
		  Draw_word(7+118*3,678,112,112,0xffff,duoji[0]);  //不变色

 }


 void key_test(void)
{
		 vu8 key=0;
static	int keystrokes[9]={0};
key=KEY_Scan(0);	//得到键值
	   	if(key)
		{						   
			switch(key)
			{				 
				case WKUP_PRES:	//控制蜂鸣器
					
					keystrokes[0]++;
				keystrokes[0]=(keystrokes[0]%9);
				printf("WKUP_PRES:%d\r\n",keystrokes[0]);
					break; 
				case KEY1_PRES:	//加加按钮	按下
					switch(keystrokes[0])
					{
						case 1:  //如果是风扇1设置模式
							keystrokes[1]=!keystrokes[1];
						AJ_widget_SW.Ventilator1 =!AJ_widget_SW.Ventilator1;
						break;
						case 2:  //如果是风扇2设置模式
							keystrokes[2]=!keystrokes[2];
						AJ_widget_SW.Ventilator2 =!AJ_widget_SW.Ventilator2;
						break;
						case 3:  ////如果是抽水机设置模式
							keystrokes[3]=!keystrokes[3];
						AJ_widget_SW.Irrigation_Motor =!AJ_widget_SW.Irrigation_Motor;
						break;
						case 4:  //如果是照明灯设置模式
							keystrokes[4]=!keystrokes[4];
						AJ_widget_SW.Floodlight =!AJ_widget_SW.Floodlight;
						break;
						case 5:  //如果是加热设置模式
							keystrokes[5]=!keystrokes[5];
						AJ_widget_SW.HeatLamp =!AJ_widget_SW.HeatLamp;
						break;
						case 6:  ////如果是报警灯设置模式
							keystrokes[6]=!keystrokes[6];
						AJ_widget_SW.WarningLamp =!AJ_widget_SW.WarningLamp;
						break;
								case 7:  //如果是蜂鸣器设置模式
							keystrokes[7]=!keystrokes[7];
								AJ_widget_SW.Buzzer =!AJ_widget_SW.Buzzer;
						break;
						case 8:  //如果是舵机设置模式
							keystrokes[8]=!keystrokes[8];
						AJ_widget_SW.Skylight =!AJ_widget_SW.Skylight;
						break;
						default:
            printf("++key is error\n");
            break;
					}
					printf("KEY1_PRES:%d,%d\r\n",keystrokes[0],keystrokes[keystrokes[0]]);
					break;
				case KEY0_PRES:	//减减按钮按下
					break;
					
			}
			lcd_actuator_refresh(keystrokes);
		}
		
	}

	
	void draw_main_page(void)
	{
				    //标题
        Draw_word(42,22,56,56,0xcd2626,title[0]);
        Draw_word(42+66,22,56,56,0xcd2626,title[1]);
        Draw_word(42+66*2,22,56,56,0xcd2626,title[2]);
        Draw_word(42+66*3,22,56,56,0xcd2626,title[3]);
        Draw_word(42+66*4,22,56,56,0xcd2626,title[4]);
        Draw_word(42+66*5,22,56,56,0xcd2626,title[5]);
		
		LCD_Fill(40, 80, 42+66*4+66, 83, 0XF81F); //绘制线条
		LCD_Fill(42+66, 87, 42+66*6+2, 90, MAGENTA); //绘制线条
				//环境温度
        Draw_word(100,100,48,48,0xff00,hj_wendu[0]);
        Draw_word(100+48,100,48,48,0xff00,hj_wendu[1]);
        Draw_word(100+48*2,100,48,48,0xff00,hj_wendu[2]);
        Draw_word(100+48*3,100,48,48,0xff00,hj_wendu[3]);
				Draw_word(100+48*4,100,48,48,0xff00,fenghao[0]);
				//环境湿度
        Draw_word(100,156,48,48,0xff00,hj_shidu[0]);
        Draw_word(100+48,156,48,48,0xff00,hj_shidu[1]);
        Draw_word(100+48*2,156,48,48,0xff00,hj_shidu[2]);
        Draw_word(100+48*3,156,48,48,0xff00,hj_shidu[3]);	
				Draw_word(100+48*4,156,48,48,0xff00,fenghao[0]);
				//环境光照
        Draw_word(100,212,48,48,0xff00,hj_guanzao[0]);
        Draw_word(100+48,212,48,48,0xff00,hj_guanzao[1]);
        Draw_word(100+48*2,212,48,48,0xff00,hj_guanzao[2]);
        Draw_word(100+48*3,212,48,48,0xff00,hj_guanzao[3]);
				Draw_word(100+48*4,212,48,48,0xff00,fenghao[0]);
				//土壤湿度
        Draw_word(100,268,48,48,0xff00,turanshidu[0]);
        Draw_word(100+48,268,48,48,0xff00,turanshidu[1]);
        Draw_word(100+48*2,268,48,48,0xff00,turanshidu[2]);
        Draw_word(100+48*3,268,48,48,0xff00,turanshidu[3]);
				Draw_word(100+48*4,268,48,48,0xff00,fenghao[0]);
				//水位高度
        Draw_word(100,324,48,48,0xff00,shuiweigaodu[0]);
        Draw_word(100+48,324,48,48,0xff00,shuiweigaodu[1]);
        Draw_word(100+48*2,324,48,48,0xff00,shuiweigaodu[2]);
        Draw_word(100+48*3,324,48,48,0xff00,shuiweigaodu[3]);
				Draw_word(100+48*4,324,48,48,0xff00,fenghao[0]);
				//空气质量
        Draw_word(100,380,48,48,0xff00,kongqizhiliang[0]);
        Draw_word(100+48,380,48,48,0xff00,kongqizhiliang[1]);
        Draw_word(100+48*2,380,48,48,0xff00,kongqizhiliang[2]);
        Draw_word(100+48*3,380,48,48,0xff00,kongqizhiliang[3]);
				Draw_word(100+48*4,380,48,48,0xff00,fenghao[0]);
				//雨滴检测
        Draw_word(100,436,48,48,0xff00,yudijiance[0]);
        Draw_word(100+48,436,48,48,0xff00,yudijiance[1]);
        Draw_word(100+48*2,436,48,48,0xff00,yudijiance[2]);
        Draw_word(100+48*3,436,48,48,0xff00,yudijiance[3]);
				Draw_word(100+48*4,436,48,48,0xff00,fenghao[0]);
				
				//数据标志绘制
				Draw_word(24,100,56,56,0xff11,wendu_biao[0]);
				Draw_word(24,156,56,56,0xff11,shidu_biao[0]);
				Draw_word(24,212,56,56,0xff11,guanzao_biao[0]);
				Draw_word(24,268,56,56,0xff11,turan_biao[0]);
				Draw_word(24,324,56,56,0xff11,shuiwei_biao[0]);
				Draw_word(24,380,56,56,0xff11,kongqi_biao[0]);
				Draw_word(24,436,56,56,0xff11,yudi_biao[0]);
						
				
				//执行器图标
				Draw_word(7,558,112,112,0xffff,paiqishang1[0]);
				Draw_word(7+118,558,112,112,0xffff,paiqishang2[0]);
				Draw_word(7+118*2,558,112,112,0xffff,choushuiji[0]);
				Draw_word(7+118*3,558,112,112,0xffff,zhaomingdeng[0]);
				Draw_word(7,678,112,112,0xffff,jiaredeng[0]);
				Draw_word(7+118,678,112,112,0xffff,jbaodeng[0]);
				Draw_word(7+118*2,678,112,112,0xffff,fmqi[0]);
				Draw_word(7+118*3,678,112,112,0xffff,duoji[0]);
	}
	
	//刷新lcd传感器数据
	void lcd_sensor_refresh(int temp,int humi,int llu,int soil,int level,int sky,int rain)
	{
						
				//温度数据
		    LCD_Fill(100+48*4+24,100 ,100+48*5+24*3,100+48,GRAY);//刷新数字区域的背景
		    if(temp<0)
				Draw_word(100+48*4+24,100,24,48,0xff00,fuhao[0]);
				Draw_word(100+48*5,100,24,48,0xff00,shuzhi[(temp/100)%10]);
				Draw_word(100+48*5+24,100,24,48,0xff00,shuzhi[(temp/10)%10]);
				Draw_word(100+48*5+24*2,100,24,48,0xff00,shuzhi[temp%10]);
				//湿度数据
				LCD_Fill(100+48*4+24,156 ,100+48*5+24*3,156+48,GRAY);//刷新数字区域的背景
		    if(humi<0)
				Draw_word(100+48*4+24,156,24,48,0xff00,fuhao[0]);
				Draw_word(100+48*5,156,24,48,0xff00,shuzhi[(humi/100)%10]);
				Draw_word(100+48*5+24,156,24,48,0xff00,shuzhi[(humi/10)%10]);
				Draw_word(100+48*5+24*2,156,24,48,0xff00,shuzhi[humi%10]);
				//光照数据
				LCD_Fill(100+48*4+24,212 ,100+48*5+24*3,212+48,GRAY);//刷新数字区域的背景
		    if(llu<0)
				Draw_word(100+48*4+24,212,24,48,0xff00,fuhao[0]);
				Draw_word(100+48*5,212,24,48,0xff00,shuzhi[(llu/100)%10]);
				Draw_word(100+48*5+24,212,24,48,0xff00,shuzhi[(llu/10)%10]);
				Draw_word(100+48*5+24*2,212,24,48,0xff00,shuzhi[llu%10]);
				//土壤湿度数据
				LCD_Fill(100+48*4+24,268 ,100+48*5+24*3,268+48,GRAY);//刷新数字区域的背景
		    if(soil<0)
				Draw_word(100+48*4+24,268,24,48,0xff00,fuhao[0]);
				Draw_word(100+48*5,268,24,48,0xff00,shuzhi[(soil/100)%10]);
				Draw_word(100+48*5+24,268,24,48,0xff00,shuzhi[(soil/10)%10]);
				Draw_word(100+48*5+24*2,268,24,48,0xff00,shuzhi[soil%10]);
				//数位高度数据
				LCD_Fill(100+48*4+24,324 ,100+48*5+24*3,324+48,GRAY);//刷新数字区域的背景
		    if(level<0)
				Draw_word(100+48*4+24,324,24,48,0xff00,fuhao[0]);
				Draw_word(100+48*5,324,24,48,0xff00,shuzhi[(level/100)%10]);
				Draw_word(100+48*5+24,324,24,48,0xff00,shuzhi[(level/10)%10]);
				Draw_word(100+48*5+24*2,324,24,48,0xff00,shuzhi[level%10]);
				//空气质量数据
				LCD_Fill(100+48*4+24,380 ,100+48*5+24*3,380+48,GRAY);//刷新数字区域的背景
		    if(sky<0)
				Draw_word(100+48*4+24,380,24,48,0xff00,fuhao[0]);
				Draw_word(100+48*5,380,24,48,0xff00,shuzhi[(sky/100)%10]);
				Draw_word(100+48*5+24,380,24,48,0xff00,shuzhi[(sky/10)%10]);
				Draw_word(100+48*5+24*2,380,24,48,0xff00,shuzhi[sky%10]);
				//降雨指数数据
				LCD_Fill(100+48*4+24,436 ,100+48*5+24*3,436+48,GRAY);//刷新数字区域的背景
		    if(rain<0)
				Draw_word(100+48*4+24,436,24,48,0xff00,fuhao[0]);
				Draw_word(100+48*5,436,24,48,0xff00,shuzhi[(rain/100)%10]);
				Draw_word(100+48*5+24,436,24,48,0xff00,shuzhi[(rain/10)%10]);
				Draw_word(100+48*5+24*2,436,24,48,0xff00,shuzhi[rain%10]);

	}
	
		void lcd_sensorlogo_refresh(int data)
		{
			//雨滴标志
			if(Sensor_Data.Raindrop > Raindrop_Max)
							Draw_word(24,436,56,56,RED,yudi_biao[0]);
			else
				      Draw_word(24,436,56,56,0xff11,yudi_biao[0]);
			//空气浊度标志
			if(Sensor_Data.Air_Quality > Air_Quality_Max)
						  Draw_word(24,380,56,56,RED,kongqi_biao[0]);
			else
				      Draw_word(24,380,56,56,0xff11,kongqi_biao[0]);
			//水位标志
			if(Sensor_Data.Water_Level > Water_Level_Max)
							Draw_word(24,324,56,56,RED,shuiwei_biao[0]);
			else
				      Draw_word(24,324,56,56,0xff11,shuiwei_biao[0]);
			 //土壤标志
			if(Sensor_Data.Soil_Moisture > Soil_Moisture_Max)
							Draw_word(24,268,56,56,RED,turan_biao[0]);
			else
				      Draw_word(24,268,56,56,0xff11,turan_biao[0]);
			//光照标志
			if(Sensor_Data.illuminance < illuminance_Max)
							Draw_word(24,212,56,56,RED,guanzao_biao[0]);
			else
				      Draw_word(24,212,56,56,0xff11,guanzao_biao[0]);
			//湿度标志
			if(Sensor_Data.Humidity > Humidity_Max)
							Draw_word(24,156,56,56,RED,shidu_biao[0]);
			else
				      Draw_word(24,156,56,56,0xff11,shidu_biao[0]);
			 //温度标志
			if((Sensor_Data.Temperature > Temperature_Max)||(Sensor_Data.Temperature < Temperature_Min))
							Draw_word(24,100,56,56,RED,wendu_biao[0]);
			else
				      Draw_word(24,100,56,56,0xff11,wendu_biao[0]);
										
		}
	
void lcd_actuator_refresh1(void)
		{
				if(AJ_widget_SW.Ventilator1==1)
			Draw_word(7,558,112,112,0xcd2626,paiqishang1[0]);  //选中的，变色
		else
		  Draw_word(7,558,112,112,0xffff,paiqishang1[0]);  //不变色
		if(AJ_widget_SW.Ventilator2==1)
			Draw_word(7+118,558,112,112,0xcd2626,paiqishang2[0]);  //选中的，变色
		else
		  Draw_word(7+118,558,112,112,0xffff,paiqishang2[0]);  //不变色
		if(AJ_widget_SW.Irrigation_Motor==1)
			Draw_word(7+118*2,558,112,112,0xcd2626,choushuiji[0]); //选中的，变色
		else
		  Draw_word(7+118*2,558,112,112,0xffff,choushuiji[0]);  //不变色
		if(AJ_widget_SW.Floodlight==1)
			Draw_word(7+118*3,558,112,112,0xcd2626,zhaomingdeng[0]);  //选中的，变色
		else
		  Draw_word(7+118*3,558,112,112,0xffff,zhaomingdeng[0]);  //不变色
		if(AJ_widget_SW.HeatLamp==1)
			Draw_word(7,678,112,112,0xcd2626,jiaredeng[0]); //选中的，变色
		else
		  Draw_word(7,678,112,112,0xffff,jiaredeng[0]);  //不变色
		if(AJ_widget_SW.WarningLamp==1)
			Draw_word(7+118,678,112,112,0xcd2626,jbaodeng[0]);  //选中的，变色
		else
		  Draw_word(7+118,678,112,112,0xffff,jbaodeng[0]);   //不变色
		if(AJ_widget_SW.Buzzer==1)
			Draw_word(7+118*2,678,112,112,0xcd2626,fmqi[0]);  //选中的，变色
		else
		  Draw_word(7+118*2,678,112,112,0xffff,fmqi[0]);  //不变色
		if(AJ_widget_SW.Skylight==1)
			Draw_word(7+118*3,678,112,112,0xcd2626,duoji[0]);  //选中的，变色
		else
		  Draw_word(7+118*3,678,112,112,0xffff,duoji[0]);  //不变色
		}

void RFID_Product_Roots(void)  //RFID产品溯源
{
	 uint8_t RxBuffer[4];
   char Card_ID[8];
	char status;
	  PCD_Request(PICC_REQALL, RxBuffer);
    status=PCD_Anticoll(RxBuffer);
    sprintf(Card_ID,"%x%x%x%x",RxBuffer[0],RxBuffer[1],RxBuffer[2],RxBuffer[3]);
    if(status==PCD_OK)
	  printf("ID=%s\r\n",Card_ID);
    
     if(strcmp(Card_ID,"884c5e6")==0)  //红色植物-玫瑰
    {
			Voice_control(0x09);
      memset(RxBuffer, 0, sizeof(RxBuffer));
    }
    else if(strcmp(Card_ID,"8849b8")==0)  //水仙
    {
			Voice_control(0x0A);
      memset(RxBuffer, 0, sizeof(RxBuffer));
    }
		 else if(strcmp(Card_ID,"884c3f5")==0)  //尤加利-百合
    {
			Voice_control(0x0B);
      memset(RxBuffer, 0, sizeof(RxBuffer));
    }
		 else if(strcmp(Card_ID,"884a89")==0)  //莲花
    {
			Voice_control(0x0C);
      memset(RxBuffer, 0, sizeof(RxBuffer));
    }
	
	
}
