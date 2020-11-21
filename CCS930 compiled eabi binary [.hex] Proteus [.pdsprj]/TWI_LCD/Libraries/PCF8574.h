#include <msp430.h>
#include "I2C.h"


#define PCF8574_address                 0x27


unsigned char PCF8574_read(void);
void PCF8574_write(unsigned char data_byte);
