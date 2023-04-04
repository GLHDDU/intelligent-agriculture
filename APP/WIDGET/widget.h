#ifndef _WIDGET_h_
#define _WIDGET_h_

#include "sys.h"

//���ڿؼ���1:��  0:�أ�
typedef struct
{
	char Ventilator1;//������1
	char Ventilator2;//������2	
	char Skylight;//�촰���
	char Irrigation_Motor;//������
	char Floodlight;//������
	char HeatLamp;//���ȵ�
	char WarningLamp;//������
	char Buzzer;//������
	
}AJ_widget_t;

void Clear_AJ_widget_SW(void);

void AJ_widgets_Init(void);
void Ventilator1_Init(void);
void Ventilator2_Init(void);
void Skylight_Init(void);
void Irrigation_Motor_Init(void);
void 	Floodlight_Init(void );	/*��ɫ�����Ƴ�ʼ��*/
void 	HeatLamp_Init(void );	/*��ɫ���ȵƳ�ʼ��*/
void 	WarningLamp_Init(void );	/*��ɫ����Ƴ�ʼ��*/
void 	Buzzer_Init(void );/*��������ʼ��*/

void Ventilator1_ON(void);
void Ventilator1_OFF(void);
void Ventilator2_ON(void);
void Ventilator2_OFF(void);
void Open_Skylight(void);
void Close_Skylight(void);
void Irrigation_Motor_ON(void);
void Irrigation_Motor_OFF(void);
void Floodlight_ON(void);//������
void Floodlight_OFF(void);//������
void HeatLamp_ON(void);//���ȵ�
void HeatLamp_OFF(void);//���ȵ�
void WarningLamp_ON(void);//������
void WarningLamp_OFF(void);//������
void Buzzer_ON(void);//������
void Buzzer_OFF(void);//������

#define PC1_PIN_PORT		GPIOC
#define PC1_PIN_PORT_CLK	RCC_APB2Periph_GPIOC
#define PC1_PIN			GPIO_Pin_1
#define PC1_IN_Init		PC1_PIN_PORT_CLK,\
							PC1_PIN_PORT,\
							PC1_PIN,GPIO_Speed_50MHz,GPIO_Mode_Out_PP
#define PC1				PCout(1)

#define PC2_PIN_PORT		GPIOC
#define PC2_PIN_PORT_CLK	RCC_APB2Periph_GPIOC
#define PC2_PIN			GPIO_Pin_2
#define PC2_IN_Init		PC2_PIN_PORT_CLK,\
							PC2_PIN_PORT,\
							PC2_PIN,GPIO_Speed_50MHz,GPIO_Mode_Out_PP
#define PC2				PCout(2)

#define PC3_PIN_PORT		GPIOC
#define PC3_PIN_PORT_CLK	RCC_APB2Periph_GPIOC
#define PC3_PIN			GPIO_Pin_3
#define PC3_IN_Init		PC3_PIN_PORT_CLK,\
							PC3_PIN_PORT,\
							PC3_PIN,GPIO_Speed_50MHz,GPIO_Mode_Out_PP
#define PC3				PCout(3)

#define PC4_PIN_PORT		GPIOC
#define PC4_PIN_PORT_CLK	RCC_APB2Periph_GPIOC
#define PC4_PIN			GPIO_Pin_4
#define PC4_IN_Init		PC4_PIN_PORT_CLK,\
							PC4_PIN_PORT,\
							PC4_PIN,GPIO_Speed_50MHz,GPIO_Mode_Out_PP
#define PC4				PCout(4)

#define PC5_PIN_PORT		GPIOC
#define PC5_PIN_PORT_CLK	RCC_APB2Periph_GPIOC
#define PC5_PIN			GPIO_Pin_5
#define PC5_IN_Init		PC5_PIN_PORT_CLK,\
							PC5_PIN_PORT,\
							PC5_PIN,GPIO_Speed_50MHz,GPIO_Mode_Out_PP
#define PC5				PCout(5)


#define PC13_PIN_PORT		GPIOC
#define PC13_PIN_PORT_CLK	RCC_APB2Periph_GPIOC
#define PC13_PIN			GPIO_Pin_13
#define PC13_IN_Init		PC13_PIN_PORT_CLK,\
							PC13_PIN_PORT,\
							PC13_PIN,GPIO_Speed_50MHz,GPIO_Mode_Out_PP
//#define PC13				PCout(13)

#define PC14_PIN_PORT		GPIOC
#define PC14_PIN_PORT_CLK	RCC_APB2Periph_GPIOC
#define PC14_PIN			GPIO_Pin_14
#define PC14_IN_Init		PC14_PIN_PORT_CLK,\
							PC14_PIN_PORT,\
							PC14_PIN,GPIO_Speed_50MHz,GPIO_Mode_Out_PP

#endif
