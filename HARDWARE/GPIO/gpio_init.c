#include "gpio_init.h"


 void STM32_GPIOx_Init(uint32_t RCC_APB2Periph,		/* GPIO对应时钟 */
				  GPIO_TypeDef* GPIOx,			/* 所对应的GPIO组 */
				  uint16_t GPIO_Pinx,			/* GPIO对应的管脚号 */                       
				  GPIOSpeed_TypeDef GPIO_SPEED,	/* GPIO翻转速度 */
				  GPIOMode_TypeDef GPIO_MODE)		/* GPIO设置模式 */
					   
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//检测输入参数
	assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
	assert_param(IS_GPIO_PIN(GPIO_Pinx));
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_SPEED(GPIO_SPEED));
	assert_param(IS_GPIO_MODE(GPIO_MODE)); 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph, ENABLE);	//开启外设时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pinx;
	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
	GPIO_InitStructure.GPIO_Mode = GPIO_MODE;

	GPIO_Init(GPIOx, &GPIO_InitStructure);	//初始化GPIO相关结构体
}
