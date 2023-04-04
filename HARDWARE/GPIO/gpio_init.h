#ifndef __GPIO_H_
#define __GPIO_H_
#include "sys.h"

 void STM32_GPIOx_Init(uint32_t RCC_APB2Periph,		/* GPIO对应时钟 */
				  GPIO_TypeDef* GPIOx,			/* 所对应的GPIO组 */
				  uint16_t GPIO_Pinx,			/* GPIO对应的管脚号 */                       
				  GPIOSpeed_TypeDef GPIO_SPEED,	/* GPIO翻转速度 */
				  GPIOMode_TypeDef GPIO_MODE);/* GPIO设置模式 */


#endif

