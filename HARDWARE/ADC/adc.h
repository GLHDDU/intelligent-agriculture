#ifndef __ADC1_H
#define __ADC1_H	
#include "sys.h"
						  
  void adc_gpio_init(void);
 void adc_init(void);
	uint16_t read_adc_value(uint8_t ch);
#endif 

