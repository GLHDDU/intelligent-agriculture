#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "timer6.h"


void TIM6_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //����һ�����ö�ʱ���ı���
	NVIC_InitTypeDef NVIC_InitStructure;                          //����һ�������жϵı���
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //�����ж��������飺��2�� �������ȼ���0 1 2 3 �����ȼ���0 1 2 3		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);           //ʹ��TIM2ʱ��	
	TIM_DeInit(TIM6);                                             //��ʱ��2�Ĵ����ָ�Ĭ��ֵ	
	TIM_TimeBaseInitStructure.TIM_Period = 10000-1; 	          //�����Զ���װ��ֵ60000-1
	TIM_TimeBaseInitStructure.TIM_Prescaler=36000-1;              //���ö�ʱ��Ԥ��Ƶ��
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1��Ƶ
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);            //����TIM2
	
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);                    //�������жϱ�־λ
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);                      //ʹ��TIM2����ж�    
	TIM_Cmd(TIM6,ENABLE);                                         //��TIM2                          
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_IRQn;                 //����TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;       //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;              //�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);                               //�����ж�
}
