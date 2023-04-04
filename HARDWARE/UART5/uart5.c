#include "uart5.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	 
#include "stdlib.h"

char voive_code[]={0xaa,0x55,0x00,0x1a,0x55,0xaa};   //������������֡
	vu16 USART5_RX_STA=1; 
	u8 USART5_RX_BUF[USART5_MAX_RECV_LEN]; 				//���ջ���,���USART5_MAX_RECV_LEN���ֽ�.
u8 USART5_RX_data[USART5_MAX_RECV_LEN]; 				//���ջ���,���USART5_MAX_RECV_LEN���ֽ�.

void USART5_RX_Data(void );
void UART5_Configuration(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;        

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE );
RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE );
 
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //UART5 TX��
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�������������
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOC, &GPIO_InitStructure); //�˿�C��

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //UART5 RX��
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //�������룻
GPIO_Init(GPIOD, &GPIO_InitStructure); //�˿�D��
 
USART_InitStructure.USART_BaudRate = 9600; //�����ʣ�
USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ8λ��
USART_InitStructure.USART_StopBits = USART_StopBits_1; //ֹͣλ1λ��
USART_InitStructure.USART_Parity = USART_Parity_No ; //��У��λ��
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//��Ӳ�����أ�
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//�շ�ģʽ��
USART_Init(UART5, &USART_InitStructure);//���ô��ڲ�����

 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�����ж��飬4λ��ռ���ȼ���4λ��Ӧ���ȼ���
NVIC_InitStructure.NVIC_IRQChannel = 53; //�жϺţ�
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ���
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //��Ӧ���ȼ���
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure); 

USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
USART_Cmd(UART5, ENABLE); //ʹ�ܴ��ڣ�
}

 

void UART5_Send_Byte(u16 Data) //����һ���ֽڣ�
{
USART_SendData(UART5,Data);
while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
}

void UART5_Send_String(u16 *Data) //�����ַ�����
{
while(*Data){
UART5_Send_Byte(*Data&0xFF);
Data++;}
}

extern char voive_code[];
//����5,printf ����
//ȷ��һ�η������ݲ�����USART5_MAX_SEND_LEN�ֽ�
void u5_printf(char* fmt,...) //...��ʾ�ɱ����������ɱ�������һ���б�������ר�ŵ�ָ��ָ�����������޶����������� 
{  
//	char*USART5_TX_BUF[256];
	  //�������彡����������֡
	int j;
	for(j=0;j<(6-2);j++)							//ѭ����������
	{
	  while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
		//USART_SendData(USART3,USART3_TX_BUF[j]);  //�Ѹ�ʽ���ַ����ӿ����崮���ͳ�ȥ 
		USART_SendData(UART5,voive_code[j]&0xFF);  //�Ѹ�ʽ���ַ����ӿ����崮���ͳ�ȥ 
	}
	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
				USART_SendData(UART5,voive_code[1]&0xFF);  //����55
	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
				USART_SendData(UART5,voive_code[0]&0xFF);  //����aa
	printf("����ϵͳ��������Ϊ��%s\r\n",voive_code);

}


void UART5_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;

	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
			USART_ClearFlag(UART5, USART_IT_RXNE); //�����־λ��
		Res =USART_ReceiveData(UART5);	//��ȡ���յ�������
			//printf("%x:%x\r\n",USART5_RX_STA,Res);
		if((USART5_RX_STA&0x8000)==0)//����δ���
			{
				
			  if(USART5_RX_STA<6)//���յ���0x55
				{
				USART5_RX_BUF[USART5_RX_STA&0X3FFF]=Res ;
					USART5_RX_STA++;
					
				}
				if((USART5_RX_BUF[USART5_RX_STA-2]==0x55)&&(USART5_RX_BUF[USART5_RX_STA-1]==0xaa))
				{	USART5_RX_STA|=0x8000;	//���������  
					USART5_RX_Data();
					}		 
			} 
     } 
} 



void Voice_control(int state)
{
				voive_code[2]=state;
			u5_printf("%x",voive_code);	//����ģ��
}

void USART5_RX_Data(void )
{
	u8 len=0;
	u8 i=0;
	if(USART5_RX_STA&0x8000)
		{	
							   
			len=USART5_RX_STA&0X7FFF;//�õ��˴ν��յ������ݳ���
			USART5_RX_BUF[len]=0;	 	//���������
			
			if(len>USART5_MAX_RECV_LEN-2)
			{
				len=USART5_MAX_RECV_LEN-1;
				USART5_RX_BUF[len]=0;	 	//���������
			}
			
			USART5_RX_BUF[USART5_MAX_RECV_LEN-1]=0x01;
			for(i=0;i<USART5_MAX_RECV_LEN;i++)
			{
				USART5_RX_data[i]=USART5_RX_BUF[i];
			}
			printf("����ϵͳ��������Ϊ��%x\r\n",USART5_RX_BUF[3]);
			USART5_RX_STA=1;
		}
 
}
