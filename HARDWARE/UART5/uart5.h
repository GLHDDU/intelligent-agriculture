#ifndef __USART5_H
#define __USART5_H	 
#include "sys.h"  
 #define USART5_MAX_RECV_LEN		60					//�����ջ����ֽ���
void Voice_control(int state);
void UART5_Configuration(void);
void UART5_Send_Byte(u16 Data); //����һ���ֽڣ�
void UART5_Send_String(u16 *Data); //�����ַ�����
void u5_printf(char* fmt,...) ;
void USART5_RX_Data(void);
#endif
