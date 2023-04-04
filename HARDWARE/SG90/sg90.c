#include "sg90.h"
#include "usart.h"

//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void PWM_Init(u16 arr,u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;              //����GPIO�ṹ�� ��Ҫ�ýṹ�����Ͳ���ʱ��һ��Ҫ����ǰ�涨��
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;    //����TIMx��ʱ���ṹ��
    TIM_OCInitTypeDef TIM_OCInitStructure;            //���嶨ʱ���������ƽṹ��
    
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4,ENABLE);          //ʹ��TIM3��4ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);//ʹ��GPIOAʱ�Ӻ�GPIOBʱ��
    
//    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);                     //TIM3������ӳ�� TIM3_CH2->PB5
	
    //���ø�����Ϊ�����������,���TIM3��4 ��PWM���岨��	GPIOB.0��1��6��7��8��9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_9;// GPIO_Pin_8|                              
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                         //������� 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;                       //�����������
    GPIO_Init(GPIOB,&GPIO_InitStructure);                                   //��ʼ��GPIOB
	
	 //���ø�����Ϊ�����������,���TIM3 ��PWM���岨��	GPIOA 6��7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;                               
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                         //������� 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;                       //�����������
    GPIO_Init(GPIOA,&GPIO_InitStructure);                                   //��ʼ��GPIOA
	

	
	//��ʼ��TIM3
    TIM_TimeBaseStructure.TIM_Period = arr;                                 //�����Զ���װ�ؼĴ������ڵ�ֵ arr=value-1
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                              //����Ԥ��Ƶֵ psc=value-1
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                            //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;             //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);                          //��ʼ��TIMxʱ�����
    
	//��ʼ��TIM4
    TIM_TimeBaseStructure.TIM_Period = arr;                                 //�����Զ���װ�ؼĴ������ڵ�ֵ arr=value-1
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                              //����Ԥ��Ƶֵ psc=value-1
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                            //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;             //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);                          //��ʼ��TIMxʱ�����
	
	//��ʼ��TIM3 Channel2 PWMģʽ     
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                       //ѡ��ʱ��ģʽ:TIM������ȵ���ģʽ1���ı�ռ�ձȣ�
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;           //ʹ�ܱȽ����
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;               //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM3,&TIM_OCInitStructure);                                 //����Tָ���Ĳ�����ʼ������TIM3 OC1
    TIM_OC2Init(TIM3,&TIM_OCInitStructure);                                 //����Tָ���Ĳ�����ʼ������TIM3 OC2
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);                                 //����Tָ���Ĳ�����ʼ������TIM3 OC2
	TIM_OC4Init(TIM3,&TIM_OCInitStructure);                                 //����Tָ���Ĳ�����ʼ������TIM3 OC2
	
	
    //��ʼ��TIM4 Channel2 PWMģʽ     
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                       //ѡ��ʱ��ģʽ:TIM������ȵ���ģʽ1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;           //ʹ�ܱȽ����
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;               //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM4,&TIM_OCInitStructure);                                 //����Tָ���Ĳ�����ʼ������TIM4 OC1
    TIM_OC2Init(TIM4,&TIM_OCInitStructure);                                 //����Tָ���Ĳ�����ʼ������TIM4 OC2
	TIM_OC3Init(TIM4,&TIM_OCInitStructure);                                 //����Tָ���Ĳ�����ʼ������TIM4 OC2
	TIM_OC4Init(TIM4,&TIM_OCInitStructure);                                 //����Tָ���Ĳ�����ʼ������TIM4 OC2

 
//    TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);                        //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
    TIM_Cmd(TIM3, ENABLE);                                                  //ʹ��TIM3
	TIM_Cmd(TIM4, ENABLE);                                                  //ʹ��TIM4
}