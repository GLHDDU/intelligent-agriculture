#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "WIFI_task.h"
#include "User_task.h"
#include "Sensors_task.h"
#include "Adjustment_task.h"

/*��ʼ����*/
#define START_TASK_PRIO		1//�������ȼ�
#define START_STK_SIZE 		128 //�����ջ��С	 
TaskHandle_t StartTask_Handler;//������
void start_task(void *pvParameters);//������

/*WIFI����*/
#define WIFI_TASK_PRIO		2//�������ȼ�
#define WIFI_STK_SIZE 		256  //�����ջ��С	
TaskHandle_t WIFITask_Handler;//������

/*�˻���������*/
#define USER_TASK_PRIO		3//�������ȼ�
#define USER_STK_SIZE 		256  //�����ջ��С	
TaskHandle_t USERTask_Handler;//������

/*��������*/
#define Adjustment_TASK_PRIO		4//�������ȼ�
#define Adjustment_STK_SIZE 		516  //�����ջ��С	
TaskHandle_t AdjustmentTask_Handler;//������

int main(void)
{
	//printf("//������ʼ����");//������ʼ����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4	 
	delay_init();	    				//��ʱ������ʼ��	 
	uart_init(115200);					//��ʼ������
	//printf("//������ʼ����");//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}

//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
	
    //����WIFI����
    xTaskCreate((TaskFunction_t )WIFI_task,             
                (const char*    )"WIFI_task",           
                (uint16_t       )WIFI_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )WIFI_TASK_PRIO,        
                (TaskHandle_t*  )&WIFITask_Handler);
								
    //����USER����
    xTaskCreate((TaskFunction_t )USER_task,             
                (const char*    )"USER_task",           
                (uint16_t       )USER_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )USER_TASK_PRIO,        
                (TaskHandle_t*  )&USERTask_Handler);


    //������������
    xTaskCreate((TaskFunction_t )Adjustment_task,             
                (const char*    )"Adjustment_task",           
                (uint16_t       )Adjustment_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )Adjustment_TASK_PRIO,        
                (TaskHandle_t*  )&AdjustmentTask_Handler);									
								
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}
