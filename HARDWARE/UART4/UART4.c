/******************************************************************************
*
*
*
*
*
*
*
*
*
******************************************************************************/

#include "UART4.h"
#include "stm32f10x_rcc.h"
#include "timer.h"
#include "sys.h"
void MY_NVIC_Init(uint8_t NVIC_PreemptionPriority,uint8_t NVIC_Subpriority,uint8_t NVIC_Channel,uint32_t NVIC_Group)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	//����������
	assert_param(IS_NVIC_PREEMPTION_PRIORITY(NVIC_PreemptionPriority));  
	assert_param(IS_NVIC_SUB_PRIORITY(NVIC_Subpriority));
	
		NVIC_PriorityGroupConfig(NVIC_Group);	//�����жϷ���	
	
	NVIC_InitStructure.NVIC_IRQChannel = NVIC_Channel;								//�����ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_PreemptionPriority;	//�������ȼ�  	 	 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_Subpriority;				//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//ʹ���ж�

	NVIC_Init(&NVIC_InitStructure);	

}
/******************************************************************************
* Function Name --> ��ʼ��IO ����4
* Description   --> none
* Input         --> bound��������	 
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void UART4_Init(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
#ifdef EN_UART4_RX	//���ʹ���˽����ж�
	
	UART4->CR1 |= 1 << 8;	//PE�ж�ʹ��
	UART4->CR1 |= 1 << 5;	//���ջ������ǿ��ж�ʹ��
	
	MY_NVIC_Init(3, 2, UART4_IRQn, NVIC_PriorityGroup_2);	//�жϷ���2
	
#endif
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	//ʹ��UART4��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	USART_DeInit(UART4);  //��λ����4
	
	//UART4_TX   PC.10 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��PC10
   
	//UART4_RX	  PC.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);  //��ʼ��PC11

	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;	//���ò����ʣ�һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;	//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	
	USART_Init(UART4, &USART_InitStructure); //��ʼ������
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(UART4, ENABLE);                    //ʹ�ܴ��� 


//	TIM7_Int_Init(100-1,7200-1);		//10ms

	//TIM_Cmd(TIM7,ENABLE);			//
}

/******************************************************************************
* Function Name --> ����4����һ���ֽ�����
* Description   --> nonevoid USART1_SendData(uint16_t Data)
* Input         --> bound��������	
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void UART4_SendData(uint16_t Data)
{
	while((UART4->SR & 0x40) == 0);	//�ȴ��������
	USART_SendData(UART4, Data);
}

/******************************************************************************
* Function Name --> ����4����һ������
* Description   --> none
* Input         --> *Data:�ַ���	
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void UART4_SendStr(uint8_t *Data)
{
	while(*Data!='\0')
	{
	    UART4_SendData((uint16_t )(*Data));
		Data++;
	}
}

/******************************************************************************
* Function Name --> ����4����һ�����ݣ�ָ�����ݳ���
* Description   --> none
* Input         --> *Data:�ַ�����len����	
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void UART4_SendStrlen(uint8_t *Data, uint8_t len)
{
	while(len--)
	{
		UART4_SendData((uint16_t )(*Data));
		Data++;
	}
}

//=========================================================
 
#ifdef EN_UART4_RX   //���ʹ���˽���

//=========================================================

//����4�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ��� 

uint8_t UART4_RX_BUF[UART4_REC_LEN];	//���ջ���,���UART4_REC_LEN���ֽ�

//����״̬
uint16_t UART4_RX_COUNT=0;	//���յ�����Ч�ֽ���Ŀ�����USART1_REC_LEN�ֽ�
uint8_t F_UART4_RX_FINISH=0;//����״̬���,1:UART4������ɣ�0��UART4����δ���
uint8_t F_UART4_RX_RECEIVING=0;//���ڽ������ڽ����б��,1:�����У�0����ͣ�����
uint8_t UART4_RX_TIMEOUT_COUNT=0;	//���ڽ��ճ�ʱ���������ڳ���3msδ���յ����ݣ���Ϊ��ʱ����λ��ms

/******************************************************************************
* Function Name --> ����4�����жϷ������
* Description   --> none
* Input         --> none
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void UART4_IRQHandler(void)
{
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //���յ�����
	{
		F_UART4_RX_RECEIVING=1;//���ڽ��մ�������
		UART4_RX_TIMEOUT_COUNT=0;//��ʱ��������
		UART4_RX_BUF[UART4_RX_COUNT] = USART_ReceiveData(UART4);	//��ȡ���յ�������
		UART4_RX_COUNT++;
		if(UART4_RX_COUNT>UART4_REC_LEN)//���������
		{UART4_RX_COUNT = 0x0000;}	
		//end ����δ���   		 		 
	}	//end ���յ�����
}
/*
void CLR_Buf4(void)  ;
unsigned char USART4_RX_BUF[USART_REC_LEN];
u16 USART4_RX_STA;
//��֡����ȡ���ݶ�
static uint8_t ucBuffer[9];
static uint8_t ucRxCnt = 0;
void CopeSerial4Data(unsigned char ucData)
{
	uint16_t i;
  ucBuffer[ucRxCnt ++] = ucData;
	//printf("ucBuffer:%d\r\n",ucData);
	if(ucRxCnt >9 ) //���ս���
	{  
		
			if(ucBuffer[7] == Sent_Data_Tail)  //����Ҫ��֡
			{
				//printf("Sent_Data_Tail\r\n");
				 CLR_Buf4();
         for(i=0;i<7;i++)
				 {
					 USART4_RX_BUF[i]=ucBuffer[i+1];
				 }
			}
			else
			{
				ucRxCnt = 0;
				return;
		  }
	}		
}*/
	
/*����3�����жϴ�������*/
/*
void UART4_IRQHandler(void)
{
	    if(USART_GetITStatus(UART4,USART_IT_RXNE) != RESET)
    {  
			printf("444444\r\n");
			USART_ClearFlag(UART4,USART_IT_RXNE);			
			CopeSerial4Data(USART_ReceiveData(UART4));
    }
}*/
/*�������2���ջ���*/
/*void CLR_Buf4(void)     
{
	unsigned int y=0;
	for( y= 0;y < USART_REC_LEN;y ++)
	{
		USART4_RX_BUF[y] = '\0';
	}
	USART4_RX_STA =0;
}
*/
//=========================================================

#endif	//endʹ�ܽ���

//=========================================================




