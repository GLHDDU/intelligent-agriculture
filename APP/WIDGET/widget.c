#include "widget.h"
#include "gpio_init.h"
#include "sg90.h"
#include "beep.h"
#include "led.h"
#include "mqtt.h"
#include <string.h>
AJ_widget_t AJ_widget_SW;	//调节控件开关
AJ_widget_t Last_AJ_widget_SW;	//调节控件开关

/****************************初始化**********************************************/
void AJ_widgets_Init(void)
{
	Ventilator1_Init();			/*排气扇1初始化*/
	Ventilator2_Init();			/*排气扇2初始化*/
	Skylight_Init();				/*天窗舵机初始化*/
	Irrigation_Motor_Init();/*灌溉马达初始化*/
	Floodlight_Init();	/*蓝色照明灯初始化*/
	HeatLamp_Init();	/*红色加热灯初始化*/
	WarningLamp_Init();	/*绿色警告灯初始化*/
	Buzzer_Init();/*蜂鸣器初始化*/
	
	Clear_AJ_widget_SW();		/*开关标志位清零*/
}

/*开关标志位清零*/
void Clear_AJ_widget_SW(void)
{
		AJ_widget_SW.Ventilator1=0;
		AJ_widget_SW.Ventilator2=0;
		AJ_widget_SW.Skylight=0;
		AJ_widget_SW.Irrigation_Motor=0;
		AJ_widget_SW.Floodlight=0;
		AJ_widget_SW.HeatLamp=0;
		AJ_widget_SW.WarningLamp=0;
		AJ_widget_SW.Buzzer=0;
}

/*排气扇1初始化*/
void Ventilator1_Init(void)
{
	STM32_GPIOx_Init(PC1_IN_Init);
}

/*排气扇2初始化*/
void Ventilator2_Init(void)
{
	STM32_GPIOx_Init(PC2_IN_Init);
}

/*天窗舵机初始化*/
void Skylight_Init(void)
{
	PWM_Init(199,7199); //舵机初始化   不分频。PWM频率=72000000/900=80Khz
}

/*灌溉马达初始化*/
void Irrigation_Motor_Init(void)
{
	STM32_GPIOx_Init(PC3_IN_Init);
}

/*蓝色照明灯初始化*/
void Floodlight_Init(void)
{
	STM32_GPIOx_Init(PC13_IN_Init);
}

/*红色加热灯初始化*/
void HeatLamp_Init(void)
{
	STM32_GPIOx_Init(PC4_IN_Init);
	
}

/*绿色警告灯初始化*/
void WarningLamp_Init(void)
{
	STM32_GPIOx_Init(PC5_IN_Init);
}

/*蜂鸣器初始化*/
void Buzzer_Init(void)
{
	BEEP_Init();
	STM32_GPIOx_Init(PC14_IN_Init);
}
/*********************************控件实现***************************************/
char temp[256];
//开排气扇1
void Ventilator1_ON(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_1);
	printf("Ventilator1_ON\r\n");
	sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Ventilator1\":1},\"version\":\"1.0.0\"}");  //构建回复湿度温度数据
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器
}
//关排气扇1
void Ventilator1_OFF(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	printf("Ventilator1_OFF\r\n");
	sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Ventilator1\":0},\"version\":\"1.0.0\"}");  //构建回复湿度温度数据
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器
}

//开排气扇2
void Ventilator2_ON(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_2);
	printf("Ventilator2_ON\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Ventilator2\":1},\"version\":\"1.0.0\"}");  //构建回复湿度温度数据
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器
}
//关排气扇2
void Ventilator2_OFF(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);
	printf("Ventilator2_OFF\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Ventilator2\":0},\"version\":\"1.0.0\"}");  //构建回复湿度温度数据
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器
}

//舵机开天窗
void Open_Skylight(void)
{
	//TIM_SetCompare1(TIM4,5);//定时器4通道1---0度 PB6
	TIM_SetCompare1(TIM3,5); //pa6
	printf("Open_Skylight\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Skylight\":1},\"version\":\"1.0.0\"}");  //构建回复湿度温度数据
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器
}
//舵机关天窗
void Close_Skylight(void)
{
	TIM_SetCompare1(TIM3,25); //pa6
	//TIM_SetCompare1(TIM4,25);//定时器4通道1---180度 PB6
	printf("Close_Skylight\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Skylight\":0},\"version\":\"1.0.0\"}");  //构建回复湿度温度数据
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器
}
//开灌溉马达
void Irrigation_Motor_ON(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_3);
	printf("Irrigation_Motor_ON\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Irrigation_Motor\":1},\"version\":\"1.0.0\"}");  //构建回复湿度温度数据
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器
}
//关灌溉马达
void Irrigation_Motor_OFF(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_3);
	printf("Irrigation_Motor_OFF\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Irrigation_Motor\":0},\"version\":\"1.0.0\"}");  //构建回复湿度温度数据
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器
}
//开照明灯
void Floodlight_ON(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	printf("Floodlight_ON\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Floodlight\":1},\"version\":\"1.0.0\"}");  //构建回复湿度温度数据
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器
}
//关照明灯
void Floodlight_OFF(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	printf("Floodlight_OFF\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Floodlight\":0},\"version\":\"1.0.0\"}");  //构建回复湿度温度数据
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器
}
//开红色加热灯
void HeatLamp_ON(void)
{
	
	GPIO_SetBits(GPIOC, GPIO_Pin_4);
	printf("HeatLamp_ON\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"HeatLamp\":1},\"version\":\"1.0.0\"}");  //构建回复湿度温度数据
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器
}
//关红色加热灯
void HeatLamp_OFF(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_4);
	printf("HeatLamp_OFF\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"HeatLamp\":0},\"version\":\"1.0.0\"}");  //构建回复湿度温度数据
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器
}
//开绿色警报灯
void WarningLamp_ON(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_5);
	printf("WarningLamp_ON\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"WarningLamp\":1},\"version\":\"1.0.0\"}");  //构建回复湿度温度数据
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器
}
//关绿色警报灯
void WarningLamp_OFF(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_5);
	printf("WarningLamp_OFF\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"WarningLamp\":0},\"version\":\"1.0.0\"}");  //构建回复湿度温度数据
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器
}
//开蜂鸣器
void Buzzer_ON(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
	GPIO_SetBits(GPIOC, GPIO_Pin_14);
	printf("Buzzer_ON\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Buzzer\":1},\"version\":\"1.0.0\"}");  //构建回复湿度温度数据
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器
}
//关蜂鸣器
void Buzzer_OFF(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	GPIO_ResetBits(GPIOC, GPIO_Pin_14);
	printf("Buzzer_OFF\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Buzzer\":0},\"version\":\"1.0.0\"}");  //构建回复湿度温度数据
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器
}



