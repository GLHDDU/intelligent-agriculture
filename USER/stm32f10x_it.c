/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"     //包含需要的头文件
#include "stm32f10x_it.h"  //包含需要的头文件
#include "main.h"          //包含需要的头文件
#include "delay.h"         //包含需要的头文件
#include "usart.h"        //包含需要的头文件
#include "usart2.h"        //包含需要的头文件
#include "USART3.h"        //包含需要的头文件
#include "UART4.h"        //包含需要的头文件
#include "timer5.h"        //包含需要的头文件
#include "timer4.h"        //包含需要的头文件
#include "wifi.h"	       //包含需要的头文件
#include "timer.h"        //包含需要的头文件
#include "led.h"           //包含需要的头文件
#include "mqtt.h"          //包含需要的头文件
#include "WIFI_task.h"
#include "SENSORS.h"
#include "timer6.h"        //包含需要的头文件
#include "WIFI_task.h"

extern u8  USART3_RX_TEMP[USART3_MAX_RECV_LEN]; 		

/*-------------------------------------------------*/
/*函数名：串口2接收中断函数                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void USART2_IRQHandler(void)   
{                      
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){  //如果USART_IT_RXNE标志置位，表示有数据到了，进入if分支
		if(Connect_flag==0){                                //如果Connect_flag等于0，当前还没有连接服务器，处于指令配置状态
			if(USART2->DR){                                 //处于指令配置状态时，非零值才保存到缓冲区	
				Usart2_RxBuff[Usart2_RxCounter]=USART2->DR; //保存到缓冲区	
				Usart2_RxCounter ++;                        //每接收1个字节的数据，Usart2_RxCounter加1，表示接收的数据总量+1 
			}		
		}else{		                                        //反之Connect_flag等于1，连接上服务器了	
			Usart2_RxBuff[Usart2_RxCounter] = USART2->DR;   //把接收到的数据保存到Usart2_RxBuff中				
			if(Usart2_RxCounter == 0){    					//如果Usart2_RxCounter等于0，表示是接收的第1个数据，进入if分支				
				TIM_Cmd(TIM4,ENABLE); 
			}else{                        					//else分支，表示果Usart2_RxCounter不等于0，不是接收的第一个数据
				TIM_SetCounter(TIM4,0);  
			}	
			Usart2_RxCounter ++;         				    //每接收1个字节的数据，Usart2_RxCounter加1，表示接收的数据总量+1 
		}
	}
} 
/*-------------------------------------------------*/
/*函数名：定时器4中断服务函数                      */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){                //如果TIM_IT_Update置位，表示TIM4溢出中断，进入if	
		
		memcpy(&MQTT_RxDataInPtr[2],Usart2_RxBuff,Usart2_RxCounter);  //拷贝数据到接收缓冲区
		MQTT_RxDataInPtr[0] = Usart2_RxCounter/256;                   //记录数据长度高字节
		MQTT_RxDataInPtr[1] = Usart2_RxCounter%256;                   //记录数据长度低字节
		MQTT_RxDataInPtr+=BUFF_UNIT;                                  //指针下移
		if(MQTT_RxDataInPtr==MQTT_RxDataEndPtr)                       //如果指针到缓冲区尾部了
			MQTT_RxDataInPtr = MQTT_RxDataBuf[0];                     //指针归位到缓冲区开头
		Usart2_RxCounter = 0;                                         //串口2接收数据量变量清零
		TIM_SetCounter(TIM5, 0);                                      //清零定时器6计数器，重新计时ping包发送时间
		TIM_Cmd(TIM4, DISABLE);                        				  //关闭TIM4定时器
		TIM_SetCounter(TIM4, 0);                        			  //清零定时器4计数器
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);     			  //清除TIM4溢出中断标志 	
	}
}
/*-------------------------------------------------*/
/*函数名：定时器5中断服务函数                      */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
int i=1;
void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET){   //如果TIM_IT_Update置位，表示TIM5溢出中断，进入if	
		switch(Ping_flag){                               //判断Ping_flag的状态
			case 0:										 //如果Ping_flag等于0，表示正常状态，发送Ping报文  
					MQTT_PingREQ(); 					 //添加Ping报文到发送缓冲区  
					break;
			case 1:										 //如果Ping_flag等于1，说明上一次发送到的ping报文，没有收到服务器回复，所以1没有被清除为0，可能是连接异常，我们要启动快速ping模式
					TIM5_ENABLE_2S(); 					 //我们将定时器6设置为2s定时,快速发送Ping报文
					MQTT_PingREQ();  					 //添加Ping报文到发送缓冲区  
					break;
			case 2:										 //如果Ping_flag等于2，说明还没有收到服务器回复
			case 3:				                         //如果Ping_flag等于3，说明还没有收到服务器回复
			case 4:				                         //如果Ping_flag等于4，说明还没有收到服务器回复	
					MQTT_PingREQ();  					 //添加Ping报文到发送缓冲区 
					break;
			case 5:										 //如果Ping_flag等于5，说明我们发送了多次ping，均无回复，应该是连接有问题，我们重启连接
					Connect_flag = 0;                    //连接状态置0，表示断开，没连上服务器
					TIM_Cmd(TIM5,DISABLE);               //关TIM5 				
					break;			
		}
		Ping_flag++;           		             		 //Ping_flag自增1，表示又发送了一次ping，期待服务器的回复
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);      //清除TIM5溢出中断标志 	
	}
}
/*------------------------------------------------------------------------------*/
/*函数名：定时器2中断服务函数                     															*/
/*参  数：无                                       															*/
/*返回值：无                                      															*/
/*功  能：定时获取传感器数据并且通wifi上传到阿里云服务器和接收阿里云服务器的数据*/
/*------------------------------------------------------------------------------*/
extern Sensor_Data_t Sensor_Data;
extern unsigned char USART4_RX_BUF[USART_REC_LEN];
void TIM2_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){  //如果TIM_IT_Update置位，表示TIM2溢出中断，进入if	
		printf("--------------------------\r\n");
		
		//获取传感器新数据
		printf("获取传感器数据...\r\n");
	  Get_Sensors_Data(&Sensor_Data); 
	/*	sscanf((const char *)USART3_RX_TEMP,"%2d",&(Sensor_Data.Temperature));
		sscanf((const char *)USART3_RX_TEMP+2,"%2d",&(Sensor_Data.Humidity));
		sscanf((const char *)USART3_RX_TEMP+4,"%3d",&(Sensor_Data.illuminance));
		
		sscanf((const char *)USART3_RX_TEMP+10,"%2d",&(Sensor_Data.Soil_Moisture));
		sscanf((const char *)USART3_RX_TEMP+12,"%2d",&(Sensor_Data.Water_Level));
		sscanf((const char *)USART3_RX_TEMP+14,"%2d",&(Sensor_Data.Air_Quality));
		
		Sensor_Data.Soil_Moisture=Sensor_Data.Soil_Moisture*29;
		Sensor_Data.Water_Level=Sensor_Data.Water_Level*29;
		Sensor_Data.Air_Quality=Sensor_Data.Air_Quality*29;
		Sensor_Data.Raindrop=Sensor_Data.Raindrop*3;*/
		//上传传感器数据	
		printf("上传...\r\n");
		Upload_Sensors_Data(&Sensor_Data);
		
		//接收服务器 下发数据
		printf("接收...\r\n");
		USART3_RX_Data();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);     //清除TIM2溢出中断标志 	
	}
}

void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  
//	__asm voalite ("BKPT #1");
	/* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
//void SVC_Handler(void)
//{
//}
 
void DebugMon_Handler(void)
{
}
 
//void PendSV_Handler(void)
//{
//}
// 
//void SysTick_Handler(void)
//{
//}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
