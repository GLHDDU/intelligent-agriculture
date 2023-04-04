#include "WIFI_task.h"
#include "sensors.h"
#include "queue.h"
#include "main.h"
#include "usart2.h"  
#include "wifi.h"
#include "mqtt.h"
#include "dht11.h"
#include "led.h"
#include <stdio.h>
#include <stdlib.h>
#include "timer2.h"
#include "timer4.h"
#include "timer5.h"
#include "widget.h"
#include "Adjustment_task.h"

//传感器数据
extern Sensor_Data_t Sensor_Data;
extern Sensor_Data_t Last_Sensor_Data;

//调节控件开关
extern AJ_widget_t AJ_widget_SW;	
extern AJ_widget_t Last_AJ_widget_SW;

//自动调节标志位
extern char IsAuto_flash;

char *cmdid = NULL;  //储存上位机命令的关键字

//WIFI任务函数
void WIFI_task(void *pvParameters)
{
	printf("WIFI_task\r\n");
	
	WIFI_init();//WIFI初始化
	
  vTaskDelay(100);
	
	while(1)		
	{	
		if(Connect_flag==1) //连接服务器成功  
		{   		
			Handle_TxDataBuff(); /*1、处理发送缓冲区数据*/		
			Handle_RxDataBuff(); /*2、处理接收缓冲区数据*/			
			Handle_CMDBuf();		 /*3、处理命令缓冲区数据*/
		}	
		else								//重启连接服务器
		{ 			
			ReconnectSever();
		}
	  vTaskDelay(100);
	}	
}

//WIFI初始化
void WIFI_init(void)
{
	Usart2_Init(115200);//WIFI		//WIFI模块串口
	TIM4_Init(300,7200);//?       //TIM4初始化，定时时间 300*7200*1000/72000000 = 30ms

	/*while(DHT11_Init())	//DHT11初始化	
	{
		printf("DHT11_INIT error \r\n"); 
	}
		printf("DHT11_INIT success \r\n"); 	*/
	
	WiFi_ResetIO_Init();            //初始化WiFi的复位IO
  MQTT_Buff_Init();               //初始化接收,发送,命令数据的 缓冲区 以及各状态参数
	AliIoT_Parameter_Init();	    //初始化连接阿里云IoT平台MQTT服务器的参数
	
}


	
/*处理发送缓冲区数据*/
void Handle_TxDataBuff(void)
{
		if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr){//发送缓冲区有数据了
		//3种情况可进入if
		//第1种：0x10 连接报文
		//第2种：0x82 订阅报文，且ConnectPack_flag置位，表示连接报文成功
		//第3种：SubcribePack_flag置位，说明连接和订阅均成功，其他报文可发
		if((MQTT_TxDataOutPtr[2]==0x10)||((MQTT_TxDataOutPtr[2]==0x82)&&(ConnectPack_flag==1))||(SubcribePack_flag==1)){    
			printf("发送数据:0x%x\r\n",MQTT_TxDataOutPtr[2]);  //串口提示信息
			MQTT_TxData(MQTT_TxDataOutPtr);                       //发送数据（串口2发送缓冲区中的数据）
			MQTT_TxDataOutPtr += BUFF_UNIT;                       //指针下移
			if(MQTT_TxDataOutPtr==MQTT_TxDataEndPtr)              //如果指针到缓冲区尾部了
				MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];              //指针归位到缓冲区开头
		} 				
	}	
}

/*处理接收缓冲区数据*/
void Handle_RxDataBuff(void)
{
		if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr){//接收缓冲区有数据了														
			printf("接收到数据:\r\n");
			
			/*2.1如果收到的是CONNECT报文*/
			if(MQTT_RxDataOutPtr[2]==0x20){//收到的是CONNECT报文             			
					switch(MQTT_RxDataOutPtr[5]){//判断CONNECT报文是否成功					
					case 0x00 : printf("CONNECT报文成功\r\n");  
									ConnectPack_flag = 1;                                        //CONNECT报文成功，订阅报文可发
						break;                                                                                           
					case 0x01 : printf("连接已拒绝，不支持的协议版本，准备重启\r\n");
								Connect_flag = 0;                                            //Connect_flag置零，重启连接
								break;
					case 0x02 : printf("连接已拒绝，不合格的客户端标识符，准备重启\r\n");
								Connect_flag = 0;                                            //Connect_flag置零，重启连接
								break;
					case 0x03 : printf("连接已拒绝，服务端不可用，准备重启\r\n");
								Connect_flag = 0;                                            //Connect_flag置零，重启连接
								break;
					case 0x04 : printf("连接已拒绝，无效的用户名或密码，准备重启\r\n");
								Connect_flag = 0;                                            //Connect_flag置零，重启连接						
								break;
					case 0x05 : printf("连接已拒绝，未授权，准备重启\r\n");
								Connect_flag = 0;                                            //Connect_flag置零，重启连接						
								break; 
					default   : printf("连接已拒绝，未知状态，准备重启\r\n");
								Connect_flag = 0;                                            //Connect_flag置零，重启连接					
								break;							
				}				
			}		
			
			/*2.2如果收到的是SUBACK报文*/
			else if(MQTT_RxDataOutPtr[2]==0x90){//判断SUBACK报文是否成功		
				switch(MQTT_RxDataOutPtr[6]){					
					case 0x00 :
					case 0x01 : printf("订阅成功\r\n");
											SubcribePack_flag = 1;                //SubcribePack_flag置1，表示订阅报文成功，其他报文可发送
											Ping_flag = 0;                        //Ping_flag清零
											TIM5_ENABLE_30S();                    //启动30s的PING定时器
											TIM2_ENABLE_30S();                    //启动30s的上传数据的定时器  //目前是30/6=5S上报一次  
//											TempHumi_State();                     //先发一次数据
								break;                               
					default   : printf("订阅失败，准备重启\r\n"); 
											Connect_flag = 0;                     //Connect_flag置零，重启连接
								break;						
				}					
			}
			
			/*2.3如果收到的是PINGRESP报文*/
			else if(MQTT_RxDataOutPtr[2]==0xD0){ 
				printf("PING报文回复\r\n");
				if(Ping_flag==1){             //如果Ping_flag=1，表示第一次发送
					 Ping_flag = 0;    				  //要清除Ping_flag标志
				}else if(Ping_flag>1){ 				//如果Ping_flag>1，表示是多次发送了，而且是2s间隔的快速发送
					Ping_flag = 0;     				  //要清除Ping_flag标志
					TIM5_ENABLE_30S(); 				  //PING定时器重回30s的时间
				}				
			}	
			
			/*2.4如果收到的是服务器发来的推送数据*/			
			else if((MQTT_RxDataOutPtr[2]==0x30)){ //我们要提取控制命令
				printf("服务器等级0推送\r\n");
				MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);  //处理等级0推送数据
			}				
							
			MQTT_RxDataOutPtr += BUFF_UNIT;                   //指针下移
			if(MQTT_RxDataOutPtr==MQTT_RxDataEndPtr)          //如果指针到缓冲区尾部了
				MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];          //指针归位到缓冲区开头                        
		}	
}




/*处理命令缓冲区数据*/
void Handle_CMDBuf(void)
{
		if(MQTT_CMDOutPtr != MQTT_CMDInPtr){//说明命令缓冲区有数据了			       
			printf("命令:%s\r\n",&MQTT_CMDOutPtr[2]);
			
			/*3.1简单布尔型结果分析*/
			
			//开关灯demo
			if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"powerstate\":1}"))
			{                
//				led_green_on();
				printf("led_green_on !        \r\n");
			}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"powerstate\":0}"))
			{
			//	led_green_off();
				printf("led_green_off !       \r\n");
			}			
			
						//灌溉电机
			if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Auto\":1}"))
			{                
				IsAuto_flash=Auto;
				printf("Auto controling!\r\n");
			}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Auto\":0}"))
			{
				IsAuto_flash=Manual;	
				printf("Manual controing!\r\n");
			}		
			//排风扇1
			if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Ventilator1\":1}"))
			{                
				AJ_widget_SW.Ventilator1=AJ_ON;	
			}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Ventilator1\":0}"))
			{
				AJ_widget_SW.Ventilator1=AJ_OFF;	
			}			
			//排风扇2
			if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Ventilator2\":1}"))
			{                
				AJ_widget_SW.Ventilator2=AJ_ON;	
			}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Ventilator2\":0}"))
			{
				AJ_widget_SW.Ventilator2=AJ_OFF;	
			}						
			//开关天窗
			if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Skylight\":1}"))
			{                
				AJ_widget_SW.Skylight=AJ_ON;	
			}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Skylight\":0}"))
			{
				AJ_widget_SW.Skylight=AJ_OFF;	
			}			
			//灌溉电机
			if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Irrigation_Motor\":1}"))
			{                
				AJ_widget_SW.Irrigation_Motor=AJ_ON;	
			}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Irrigation_Motor\":0}"))
			{
				AJ_widget_SW.Irrigation_Motor=AJ_OFF;	
			}	
			//红色加热灯
			if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"HeatLamp\":1}"))
			{                
				AJ_widget_SW.HeatLamp=AJ_ON;	
			}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"HeatLamp\":0}"))
			{
				AJ_widget_SW.HeatLamp=AJ_OFF;	
			}	
			//绿色警告灯
			if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"WarningLamp\":1}"))
			{                
				AJ_widget_SW.WarningLamp=AJ_ON;	
			}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"WarningLamp\":0}"))
			{
				AJ_widget_SW.WarningLamp=AJ_OFF;	
			}	
			//蓝色照明灯
			if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Floodlight\":1}"))
			{                
				AJ_widget_SW.Floodlight=AJ_ON;	
			}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Floodlight\":0}"))
			{
				AJ_widget_SW.Floodlight=AJ_OFF;	
			}	
			//蜂鸣器
			if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Buzzer\":1}"))
			{                
				AJ_widget_SW.Buzzer=AJ_ON;	
			}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Buzzer\":0}"))
			{
				AJ_widget_SW.Buzzer=AJ_OFF;	
			}				
			

		/*3.2专业数据分析*/		
			
//		//排风扇1 demo
//		if(strstr((char *)MQTT_CMDOutPtr+2, (const char *)"\"params\":{\"Ventilator1\"") != NULL)//专业数据分析，包括英文和数字
//		{			 
//			cmdid=USER_GetSteeringJsonValue((char *)MQTT_CMDOutPtr+2, (char *)"Ventilator1");
//			printf("分析到的数据：%s\r\n",cmdid);
			//if(strcmp(cmdid,"LED ON")==0)
//			if(*cmdid=='1')
//			{
//				AJ_widget_SW.Ventilator1=AJ_ON;	
//			}				
//			else if(*cmdid=='0')
//			{
//				AJ_widget_SW.Ventilator1=AJ_OFF;		
//			}
//		}			
			
				 MQTT_CMDOutPtr += BUFF_UNIT;                             //指针下移
				 if(MQTT_CMDOutPtr==MQTT_CMDEndPtr)           	          //如果指针到缓冲区尾部了
					MQTT_CMDOutPtr = MQTT_CMDBuf[0];          	            //指针归位到缓冲区开头				
			}	
}
/*重连服务器*/
void ReconnectSever(void)
{
		printf("需要连接服务器\r\n");                //串口输出信息
		TIM_Cmd(TIM4,DISABLE);                          //关闭TIM4 
		TIM_Cmd(TIM5,DISABLE);                          //关闭TIM5  
		WiFi_RxCounter=0;                               //WiFi接收数据量变量清零                        
		memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //清空WiFi接收缓冲区 
		if(WiFi_Connect_IoTServer()==0){   			     		//如果WiFi连接云服务器函数返回0，表示正确，进入if
			printf("建立TCP连接成功\r\n");             //串口输出信息
			Connect_flag = 1;                             //Connect_flag置1，表示连接成功	
			WiFi_RxCounter=0;                             //WiFi接收数据量变量清零                        
			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);       //清空WiFi接收缓冲区 		
			MQTT_Buff_ReInit();                           //重新初始化发送缓冲区                    
		}			
}

/*上传传感器数据*/
void Upload_Sensors_Data(Sensor_Data_t *Sensor_Data)
{	
	char temp[256];
	sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Water_Level\":%2d,\"Soil_Moisture\":%2d,\"Air_Quality\":%2d,\"Raindrop\":%2d,\"Humidity\":%2d,\"Temperature\":%2d,\"illuminance\":%2d},\"version\":\"1.0.0\"}",\
				Sensor_Data->Water_Level,Sensor_Data->Soil_Moisture,Sensor_Data->Air_Quality,\
				Sensor_Data->Raindrop,Sensor_Data->Humidity,Sensor_Data->Temperature,Sensor_Data->illuminance);  //构建回复湿度温度数据
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器	
	//sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Temperature\":%2d,\"Humidity\":%2d,\"illuminance\":%2d},\"version\":\"1.0.0\"}",  //构建回复湿度温度数据
	//			Sensor_Data->Temperature,Sensor_Data->Humidity,Sensor_Data->illuminance);  //构建回复湿度温度数据
	//MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器	
}


/*发送demo(采集温湿度，并发布给服务器)*/
/*void TempHumi_State(void)    
{  
	u8 tempdata,humidata;	
	char temp[256];  
	//DHT11_Read_Data(&tempdata,&humidata);	//读取温湿度值(测试过了，在定时器中断里面调用这个函数程序会卡死，原因不明)	
	tempdata=10;
	humidata=20;
	//下面这三个先不用
	//printf("test温度：%d  湿度：%d\r\n",tempdata,humidata);

	//sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"CuTemperature\":%2d,\"CurrentHumidity\":%2d},\"version\":\"1.0.0\"}",tempdata,humidata);  //构建回复湿度温度数据
	//MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器	
	
}*/
/*******************************************************************
*函数：unsigned char *USER_GetSteeringJsonValue(char *cJson, char *Tag)
*功能：json为字符串序列，将json格式中的目标对象Tag对应的值字符串转换为字符串
*输入：
		char *cJson json字符串
		char *Tag 要操作的对象标签
*输出：返回数值
*特殊说明：用户可以在此基础上改造和扩展该函数，这里只是个简单的DEMO
*******************************************************************/
char temp1[16];  // 目标字符数组
char *USER_GetSteeringJsonValue(char *cJson, char *Tag)
{ 
	char *target = NULL;
  char *dest_str; // 目标字符串
	int8_t i=0;
	memset(temp1, 0x00, 128);
	sprintf(temp1,"\"%s\":\"",Tag);
	target=strstr((const char *)cJson, (const char *)temp1);
	if(target == NULL)
	{
		printf("空字符！\r\n");
		return NULL;
	}
	i=strlen((const char *)temp1);
	target=target+i;   //地址为数据地址
	memset(temp1, 0x00, 128);
	for(i=0; i<12; i++, target++)//数值超过10个位为非法，由于2^32=4294967296
	{
		if((*target!='"'))
		{
			temp1[i]=*target;
		}
		else
		{
			break;
		}
	}
	temp1[i+1] = '\0';
		//u1_printf("提取到数据:%s\r\n",temp1);   
    dest_str = (char *)malloc(sizeof(char) * (sizeof(temp1) + 1)); /* 为字符串分配堆空间 */
    strncpy(dest_str, temp1, sizeof(temp1));  // 用C标准库函数strncpy拷贝字符
	  printf("分析命令报文获得关键字:%s\r\n",dest_str);
	return dest_str;
}


