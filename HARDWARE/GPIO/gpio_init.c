#include "gpio_init.h"


 void STM32_GPIOx_Init(uint32_t RCC_APB2Periph,		/* GPIO��Ӧʱ�� */
				  GPIO_TypeDef* GPIOx,			/* ����Ӧ��GPIO�� */
				  uint16_t GPIO_Pinx,			/* GPIO��Ӧ�Ĺܽź� */                       
				  GPIOSpeed_TypeDef GPIO_SPEED,	/* GPIO��ת�ٶ� */
				  GPIOMode_TypeDef GPIO_MODE)		/* GPIO����ģʽ */
					   
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//����������
	assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
	assert_param(IS_GPIO_PIN(GPIO_Pinx));
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_SPEED(GPIO_SPEED));
	assert_param(IS_GPIO_MODE(GPIO_MODE)); 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph, ENABLE);	//��������ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pinx;
	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
	GPIO_InitStructure.GPIO_Mode = GPIO_MODE;

	GPIO_Init(GPIOx, &GPIO_InitStructure);	//��ʼ��GPIO��ؽṹ��
}
