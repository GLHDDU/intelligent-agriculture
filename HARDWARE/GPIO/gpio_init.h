#ifndef __GPIO_H_
#define __GPIO_H_
#include "sys.h"

 void STM32_GPIOx_Init(uint32_t RCC_APB2Periph,		/* GPIO��Ӧʱ�� */
				  GPIO_TypeDef* GPIOx,			/* ����Ӧ��GPIO�� */
				  uint16_t GPIO_Pinx,			/* GPIO��Ӧ�Ĺܽź� */                       
				  GPIOSpeed_TypeDef GPIO_SPEED,	/* GPIO��ת�ٶ� */
				  GPIOMode_TypeDef GPIO_MODE);/* GPIO����ģʽ */


#endif

