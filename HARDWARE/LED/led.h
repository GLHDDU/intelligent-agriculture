#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED0 PBout(5)// PB5 红
#define LED1 PEout(5)// PE5	绿	

//指示灯
#define REDLED_PIN                          GPIO_Pin_5  
#define REDLED_PORT                         GPIOB

#define GREENLED_PIN                        GPIO_Pin_5  
#define GREENLED_PORT                       GPIOE
void LED_Init(void);//初始化
void led_green_off(void);
void led_green_on(void);
void led_green_toggle(void);
void led_red_off(void);
void led_red_on(void);
void led_red_toggle(void);
		 				    
#endif
