#include "uart5.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"  //包含需要的头文件
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	 
#include "stdlib.h"

char voive_code[]={0xaa,0x55,0x00,0x1a,0x55,0xaa};   //请求语音数据帧
	vu16 USART5_RX_STA=1; 
	u8 USART5_RX_BUF[USART5_MAX_RECV_LEN]; 				//接收缓冲,最大USART5_MAX_RECV_LEN个字节.
u8 USART5_RX_data[USART5_MAX_RECV_LEN]; 				//接收缓冲,最大USART5_MAX_RECV_LEN个字节.

void USART5_RX_Data(void );
void UART5_Configuration(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;        

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE );
RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE );
 
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //UART5 TX；
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出；
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOC, &GPIO_InitStructure); //端口C；

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //UART5 RX；
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入；
GPIO_Init(GPIOD, &GPIO_InitStructure); //端口D；
 
USART_InitStructure.USART_BaudRate = 9600; //波特率；
USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位；
USART_InitStructure.USART_StopBits = USART_StopBits_1; //停止位1位；
USART_InitStructure.USART_Parity = USART_Parity_No ; //无校验位；
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//无硬件流控；
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//收发模式；
USART_Init(UART5, &USART_InitStructure);//配置串口参数；

 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置中断组，4位抢占优先级，4位响应优先级；
NVIC_InitStructure.NVIC_IRQChannel = 53; //中断号；
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级；
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //响应优先级；
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure); 

USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
USART_Cmd(UART5, ENABLE); //使能串口；
}

 

void UART5_Send_Byte(u16 Data) //发送一个字节；
{
USART_SendData(UART5,Data);
while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
}

void UART5_Send_String(u16 *Data) //发送字符串；
{
while(*Data){
UART5_Send_Byte(*Data&0xFF);
Data++;}
}

extern char voive_code[];
//串口5,printf 函数
//确保一次发送数据不超过USART5_MAX_SEND_LEN字节
void u5_printf(char* fmt,...) //...表示可变参数（多个可变参数组成一个列表，后面有专门的指针指向他），不限定个数和类型 
{  
//	char*USART5_TX_BUF[256];
	  //请求身体健康数据数据帧
	int j;
	for(j=0;j<(6-2);j++)							//循环发送数据
	{
	  while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
		//USART_SendData(USART3,USART3_TX_BUF[j]);  //把格式化字符串从开发板串口送出去 
		USART_SendData(UART5,voive_code[j]&0xFF);  //把格式化字符串从开发板串口送出去 
	}
	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
				USART_SendData(UART5,voive_code[1]&0xFF);  //发送55
	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
				USART_SendData(UART5,voive_code[0]&0xFF);  //发送aa
	printf("语音系统发送数据为：%s\r\n",voive_code);

}


void UART5_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;

	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
			USART_ClearFlag(UART5, USART_IT_RXNE); //清除标志位；
		Res =USART_ReceiveData(UART5);	//读取接收到的数据
			//printf("%x:%x\r\n",USART5_RX_STA,Res);
		if((USART5_RX_STA&0x8000)==0)//接收未完成
			{
				
			  if(USART5_RX_STA<6)//接收到了0x55
				{
				USART5_RX_BUF[USART5_RX_STA&0X3FFF]=Res ;
					USART5_RX_STA++;
					
				}
				if((USART5_RX_BUF[USART5_RX_STA-2]==0x55)&&(USART5_RX_BUF[USART5_RX_STA-1]==0xaa))
				{	USART5_RX_STA|=0x8000;	//接收完成了  
					USART5_RX_Data();
					}		 
			} 
     } 
} 



void Voice_control(int state)
{
				voive_code[2]=state;
			u5_printf("%x",voive_code);	//语音模块
}

void USART5_RX_Data(void )
{
	u8 len=0;
	u8 i=0;
	if(USART5_RX_STA&0x8000)
		{	
							   
			len=USART5_RX_STA&0X7FFF;//得到此次接收到的数据长度
			USART5_RX_BUF[len]=0;	 	//加入结束符
			
			if(len>USART5_MAX_RECV_LEN-2)
			{
				len=USART5_MAX_RECV_LEN-1;
				USART5_RX_BUF[len]=0;	 	//加入结束符
			}
			
			USART5_RX_BUF[USART5_MAX_RECV_LEN-1]=0x01;
			for(i=0;i<USART5_MAX_RECV_LEN;i++)
			{
				USART5_RX_data[i]=USART5_RX_BUF[i];
			}
			printf("语音系统接收数据为：%x\r\n",USART5_RX_BUF[3]);
			USART5_RX_STA=1;
		}
 
}
