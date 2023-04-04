#include "widget.h"
#include "gpio_init.h"
#include "sg90.h"
#include "beep.h"
#include "led.h"
#include "mqtt.h"
#include <string.h>
AJ_widget_t AJ_widget_SW;	//���ڿؼ�����
AJ_widget_t Last_AJ_widget_SW;	//���ڿؼ�����

/****************************��ʼ��**********************************************/
void AJ_widgets_Init(void)
{
	Ventilator1_Init();			/*������1��ʼ��*/
	Ventilator2_Init();			/*������2��ʼ��*/
	Skylight_Init();				/*�촰�����ʼ��*/
	Irrigation_Motor_Init();/*�������ʼ��*/
	Floodlight_Init();	/*��ɫ�����Ƴ�ʼ��*/
	HeatLamp_Init();	/*��ɫ���ȵƳ�ʼ��*/
	WarningLamp_Init();	/*��ɫ����Ƴ�ʼ��*/
	Buzzer_Init();/*��������ʼ��*/
	
	Clear_AJ_widget_SW();		/*���ر�־λ����*/
}

/*���ر�־λ����*/
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

/*������1��ʼ��*/
void Ventilator1_Init(void)
{
	STM32_GPIOx_Init(PC1_IN_Init);
}

/*������2��ʼ��*/
void Ventilator2_Init(void)
{
	STM32_GPIOx_Init(PC2_IN_Init);
}

/*�촰�����ʼ��*/
void Skylight_Init(void)
{
	PWM_Init(199,7199); //�����ʼ��   ����Ƶ��PWMƵ��=72000000/900=80Khz
}

/*�������ʼ��*/
void Irrigation_Motor_Init(void)
{
	STM32_GPIOx_Init(PC3_IN_Init);
}

/*��ɫ�����Ƴ�ʼ��*/
void Floodlight_Init(void)
{
	STM32_GPIOx_Init(PC13_IN_Init);
}

/*��ɫ���ȵƳ�ʼ��*/
void HeatLamp_Init(void)
{
	STM32_GPIOx_Init(PC4_IN_Init);
	
}

/*��ɫ����Ƴ�ʼ��*/
void WarningLamp_Init(void)
{
	STM32_GPIOx_Init(PC5_IN_Init);
}

/*��������ʼ��*/
void Buzzer_Init(void)
{
	BEEP_Init();
	STM32_GPIOx_Init(PC14_IN_Init);
}
/*********************************�ؼ�ʵ��***************************************/
char temp[256];
//��������1
void Ventilator1_ON(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_1);
	printf("Ventilator1_ON\r\n");
	sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Ventilator1\":1},\"version\":\"1.0.0\"}");  //�����ظ�ʪ���¶�����
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������
}
//��������1
void Ventilator1_OFF(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	printf("Ventilator1_OFF\r\n");
	sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Ventilator1\":0},\"version\":\"1.0.0\"}");  //�����ظ�ʪ���¶�����
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������
}

//��������2
void Ventilator2_ON(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_2);
	printf("Ventilator2_ON\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Ventilator2\":1},\"version\":\"1.0.0\"}");  //�����ظ�ʪ���¶�����
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������
}
//��������2
void Ventilator2_OFF(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);
	printf("Ventilator2_OFF\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Ventilator2\":0},\"version\":\"1.0.0\"}");  //�����ظ�ʪ���¶�����
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������
}

//������촰
void Open_Skylight(void)
{
	//TIM_SetCompare1(TIM4,5);//��ʱ��4ͨ��1---0�� PB6
	TIM_SetCompare1(TIM3,5); //pa6
	printf("Open_Skylight\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Skylight\":1},\"version\":\"1.0.0\"}");  //�����ظ�ʪ���¶�����
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������
}
//������촰
void Close_Skylight(void)
{
	TIM_SetCompare1(TIM3,25); //pa6
	//TIM_SetCompare1(TIM4,25);//��ʱ��4ͨ��1---180�� PB6
	printf("Close_Skylight\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Skylight\":0},\"version\":\"1.0.0\"}");  //�����ظ�ʪ���¶�����
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������
}
//��������
void Irrigation_Motor_ON(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_3);
	printf("Irrigation_Motor_ON\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Irrigation_Motor\":1},\"version\":\"1.0.0\"}");  //�����ظ�ʪ���¶�����
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������
}
//�ع�����
void Irrigation_Motor_OFF(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_3);
	printf("Irrigation_Motor_OFF\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Irrigation_Motor\":0},\"version\":\"1.0.0\"}");  //�����ظ�ʪ���¶�����
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������
}
//��������
void Floodlight_ON(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	printf("Floodlight_ON\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Floodlight\":1},\"version\":\"1.0.0\"}");  //�����ظ�ʪ���¶�����
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������
}
//��������
void Floodlight_OFF(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	printf("Floodlight_OFF\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Floodlight\":0},\"version\":\"1.0.0\"}");  //�����ظ�ʪ���¶�����
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������
}
//����ɫ���ȵ�
void HeatLamp_ON(void)
{
	
	GPIO_SetBits(GPIOC, GPIO_Pin_4);
	printf("HeatLamp_ON\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"HeatLamp\":1},\"version\":\"1.0.0\"}");  //�����ظ�ʪ���¶�����
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������
}
//�غ�ɫ���ȵ�
void HeatLamp_OFF(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_4);
	printf("HeatLamp_OFF\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"HeatLamp\":0},\"version\":\"1.0.0\"}");  //�����ظ�ʪ���¶�����
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������
}
//����ɫ������
void WarningLamp_ON(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_5);
	printf("WarningLamp_ON\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"WarningLamp\":1},\"version\":\"1.0.0\"}");  //�����ظ�ʪ���¶�����
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������
}
//����ɫ������
void WarningLamp_OFF(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_5);
	printf("WarningLamp_OFF\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"WarningLamp\":0},\"version\":\"1.0.0\"}");  //�����ظ�ʪ���¶�����
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������
}
//��������
void Buzzer_ON(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
	GPIO_SetBits(GPIOC, GPIO_Pin_14);
	printf("Buzzer_ON\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Buzzer\":1},\"version\":\"1.0.0\"}");  //�����ظ�ʪ���¶�����
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������
}
//�ط�����
void Buzzer_OFF(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	GPIO_ResetBits(GPIOC, GPIO_Pin_14);
	printf("Buzzer_OFF\r\n");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Buzzer\":0},\"version\":\"1.0.0\"}");  //�����ظ�ʪ���¶�����
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������
}



