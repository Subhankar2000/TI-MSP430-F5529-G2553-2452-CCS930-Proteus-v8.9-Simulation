#include <msp430.h>


void I2C_USCI_init(unsigned char address);
void I2C_USCI_set_address(unsigned char address);
unsigned char I2C_USCI_read_byte(unsigned char address);
unsigned char I2C_USCI_read_word(unsigned char address,unsigned char *value, unsigned char length);
unsigned char I2C_USCI_write_byte(unsigned char address, unsigned char value);
