#include "I2C.h"


#define PCF8574_address                 0x20


unsigned char PCF8574_read(void);
void PCF8574_write(unsigned char data_byte);
