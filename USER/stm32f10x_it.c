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
#include "stm32f10x.h"     //������Ҫ��ͷ�ļ�
#include "stm32f10x_it.h"  //������Ҫ��ͷ�ļ�
#include "main.h"          //������Ҫ��ͷ�ļ�
#include "delay.h"         //������Ҫ��ͷ�ļ�
#include "usart.h"        //������Ҫ��ͷ�ļ�
#include "usart2.h"        //������Ҫ��ͷ�ļ�
#include "USART3.h"        //������Ҫ��ͷ�ļ�
#include "UART4.h"        //������Ҫ��ͷ�ļ�
#include "timer5.h"        //������Ҫ��ͷ�ļ�
#include "timer4.h"        //������Ҫ��ͷ�ļ�
#include "wifi.h"	       //������Ҫ��ͷ�ļ�
#include "timer.h"        //������Ҫ��ͷ�ļ�
#include "led.h"           //������Ҫ��ͷ�ļ�
#include "mqtt.h"          //������Ҫ��ͷ�ļ�
#include "WIFI_task.h"
#include "SENSORS.h"
#include "timer6.h"        //������Ҫ��ͷ�ļ�
#include "WIFI_task.h"

extern u8  USART3_RX_TEMP[USART3_MAX_RECV_LEN]; 		

/*-------------------------------------------------*/
/*������������2�����жϺ���                        */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void USART2_IRQHandler(void)   
{                      
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){  //���USART_IT_RXNE��־��λ����ʾ�����ݵ��ˣ�����if��֧
		if(Connect_flag==0){                                //���Connect_flag����0����ǰ��û�����ӷ�����������ָ������״̬
			if(USART2->DR){                                 //����ָ������״̬ʱ������ֵ�ű��浽������	
				Usart2_RxBuff[Usart2_RxCounter]=USART2->DR; //���浽������	
				Usart2_RxCounter ++;                        //ÿ����1���ֽڵ����ݣ�Usart2_RxCounter��1����ʾ���յ���������+1 
			}		
		}else{		                                        //��֮Connect_flag����1�������Ϸ�������	
			Usart2_RxBuff[Usart2_RxCounter] = USART2->DR;   //�ѽ��յ������ݱ��浽Usart2_RxBuff��				
			if(Usart2_RxCounter == 0){    					//���Usart2_RxCounter����0����ʾ�ǽ��յĵ�1�����ݣ�����if��֧				
				TIM_Cmd(TIM4,ENABLE); 
			}else{                        					//else��֧����ʾ��Usart2_RxCounter������0�����ǽ��յĵ�һ������
				TIM_SetCounter(TIM4,0);  
			}	
			Usart2_RxCounter ++;         				    //ÿ����1���ֽڵ����ݣ�Usart2_RxCounter��1����ʾ���յ���������+1 
		}
	}
} 
/*-------------------------------------------------*/
/*����������ʱ��4�жϷ�����                      */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){                //���TIM_IT_Update��λ����ʾTIM4����жϣ�����if	
		
		memcpy(&MQTT_RxDataInPtr[2],Usart2_RxBuff,Usart2_RxCounter);  //�������ݵ����ջ�����
		MQTT_RxDataInPtr[0] = Usart2_RxCounter/256;                   //��¼���ݳ��ȸ��ֽ�
		MQTT_RxDataInPtr[1] = Usart2_RxCounter%256;                   //��¼���ݳ��ȵ��ֽ�
		MQTT_RxDataInPtr+=BUFF_UNIT;                                  //ָ������
		if(MQTT_RxDataInPtr==MQTT_RxDataEndPtr)                       //���ָ�뵽������β����
			MQTT_RxDataInPtr = MQTT_RxDataBuf[0];                     //ָ���λ����������ͷ
		Usart2_RxCounter = 0;                                         //����2������������������
		TIM_SetCounter(TIM5, 0);                                      //���㶨ʱ��6�����������¼�ʱping������ʱ��
		TIM_Cmd(TIM4, DISABLE);                        				  //�ر�TIM4��ʱ��
		TIM_SetCounter(TIM4, 0);                        			  //���㶨ʱ��4������
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);     			  //���TIM4����жϱ�־ 	
	}
}
/*-------------------------------------------------*/
/*����������ʱ��5�жϷ�����                      */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
int i=1;
void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET){   //���TIM_IT_Update��λ����ʾTIM5����жϣ�����if	
		switch(Ping_flag){                               //�ж�Ping_flag��״̬
			case 0:										 //���Ping_flag����0����ʾ����״̬������Ping����  
					MQTT_PingREQ(); 					 //���Ping���ĵ����ͻ�����  
					break;
			case 1:										 //���Ping_flag����1��˵����һ�η��͵���ping���ģ�û���յ��������ظ�������1û�б����Ϊ0�������������쳣������Ҫ��������pingģʽ
					TIM5_ENABLE_2S(); 					 //���ǽ���ʱ��6����Ϊ2s��ʱ,���ٷ���Ping����
					MQTT_PingREQ();  					 //���Ping���ĵ����ͻ�����  
					break;
			case 2:										 //���Ping_flag����2��˵����û���յ��������ظ�
			case 3:				                         //���Ping_flag����3��˵����û���յ��������ظ�
			case 4:				                         //���Ping_flag����4��˵����û���յ��������ظ�	
					MQTT_PingREQ();  					 //���Ping���ĵ����ͻ����� 
					break;
			case 5:										 //���Ping_flag����5��˵�����Ƿ����˶��ping�����޻ظ���Ӧ�������������⣬������������
					Connect_flag = 0;                    //����״̬��0����ʾ�Ͽ���û���Ϸ�����
					TIM_Cmd(TIM5,DISABLE);               //��TIM5 				
					break;			
		}
		Ping_flag++;           		             		 //Ping_flag����1����ʾ�ַ�����һ��ping���ڴ��������Ļظ�
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);      //���TIM5����жϱ�־ 	
	}
}
/*------------------------------------------------------------------------------*/
/*����������ʱ��2�жϷ�����                     															*/
/*��  ������                                       															*/
/*����ֵ����                                      															*/
/*��  �ܣ���ʱ��ȡ���������ݲ���ͨwifi�ϴ��������Ʒ������ͽ��հ����Ʒ�����������*/
/*------------------------------------------------------------------------------*/
extern Sensor_Data_t Sensor_Data;
extern unsigned char USART4_RX_BUF[USART_REC_LEN];
void TIM2_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){  //���TIM_IT_Update��λ����ʾTIM2����жϣ�����if	
		printf("--------------------------\r\n");
		
		//��ȡ������������
		printf("��ȡ����������...\r\n");
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
		//�ϴ�����������	
		printf("�ϴ�...\r\n");
		Upload_Sensors_Data(&Sensor_Data);
		
		//���շ����� �·�����
		printf("����...\r\n");
		USART3_RX_Data();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);     //���TIM2����жϱ�־ 	
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
