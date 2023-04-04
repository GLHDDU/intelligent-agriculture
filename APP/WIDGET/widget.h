#ifndef _WIDGET_h_
#define _WIDGET_h_

#include "sys.h"

//调节控件（1:开  0:关）
typedef struct
{
	char Ventilator1;//排气扇1
	char Ventilator2;//排气扇2	
	char Skylight;//天窗舵机
	char Irrigation_Motor;//灌溉马达
	char Floodlight;//照明灯
	char HeatLamp;//加热灯
	char WarningLamp;//警报灯
	char Buzzer;//蜂鸣器
	
}AJ_widget_t;

void Clear_AJ_widget_SW(void);

void AJ_widgets_Init(void);
void Ventilator1_Init(void);
void Ventilator2_Init(void);
void Skylight_Init(void);
void Irrigation_Motor_Init(void);
void 	Floodlight_Init(void );	/*蓝色照明灯初始化*/
void 	HeatLamp_Init(void );	/*红色加热灯初始化*/
void 	WarningLamp_Init(void );	/*绿色警告灯初始化*/
void 	Buzzer_Init(void );/*蜂鸣器初始化*/

void Ventilator1_ON(void);
void Ventilator1_OFF(void);
void Ventilator2_ON(void);
void Ventilator2_OFF(void);
void Open_Skylight(void);
void Close_Skylight(void);
void Irrigation_Motor_ON(void);
void Irrigation_Motor_OFF(void);
void Floodlight_ON(void);//照明灯
void Floodlight_OFF(void);//照明灯
void HeatLamp_ON(void);//加热灯
void HeatLamp_OFF(void);//加热灯
void WarningLamp_ON(void);//警报灯
void WarningLamp_OFF(void);//警报灯
void Buzzer_ON(void);//蜂鸣器
void Buzzer_OFF(void);//蜂鸣器

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
