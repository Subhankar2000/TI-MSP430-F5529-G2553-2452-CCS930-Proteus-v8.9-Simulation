#include <msp430.h>
#include "SW_I2C.h"


#define PCF8591_address                               		0x90
                                                     
#define PCF8591_read_cmd                              		(PCF8591_address | 0x01)
#define PCF8591_write_cmd                             		PCF8591_address

#define AIN0                                          		0x00
#define AIN1                                          		0x01
#define AIN2                                          		0x02
#define AIN3                                          		0x03

#define Auto_Increment_Enable                         		0x04
#define Auto_Increment_Disable                        		0x00                                              

#define Four_Channel_ADC                              		0x00
#define Three_differential_Inputs                     		0x10
#define AIN0_and_1_Single_AIN2_and_AIN3_Differential  		0x20
#define All_Differential                              		0x30

#define AOut_enable                                   		0x40
#define AOut_disable                                  		0x00
              

void PCF8591_write(unsigned char control_value, unsigned char data_value);
unsigned char PCF8591_read(unsigned char control_value);
