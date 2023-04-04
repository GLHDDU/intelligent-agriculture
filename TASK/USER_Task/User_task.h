#ifndef __USER_TASK_H
#define __USER_TASK_H

#include "sys.h"
#include "user.h"

/*充值、扣款状态机*/
#define IsRecharge			1		//充值状态
#define IsDeduction			0		//扣款状态 
#define IsInitWhiteCard	2		//初始化白卡 
#define IsInitBlueCard 	3		//初始化蓝卡

void USER_task(void *pvParameters);
void simulate_lock(int8_t UnlockFlag);
void show_lcd(void);
void calculate_balance(void);
uint16_t pay_switch(u8 keyvalue);
void RFID_Product_Roots(void);  //RFID产品溯源
#endif
