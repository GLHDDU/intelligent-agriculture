#ifndef __USER_TASK_H
#define __USER_TASK_H

#include "sys.h"
#include "user.h"

/*��ֵ���ۿ�״̬��*/
#define IsRecharge			1		//��ֵ״̬
#define IsDeduction			0		//�ۿ�״̬ 
#define IsInitWhiteCard	2		//��ʼ���׿� 
#define IsInitBlueCard 	3		//��ʼ������

void USER_task(void *pvParameters);
void simulate_lock(int8_t UnlockFlag);
void show_lcd(void);
void calculate_balance(void);
uint16_t pay_switch(u8 keyvalue);
void RFID_Product_Roots(void);  //RFID��Ʒ��Դ
#endif
