#include "PCF8591.h"


void PCF8591_write(unsigned char control_value, unsigned char data_value)
{
     SW_I2C_start();
     SW_I2C_write(PCF8591_write_cmd);
     SW_I2C_wait_ACK();
     SW_I2C_write((control_value & 0xFF));
     SW_I2C_wait_ACK();
     SW_I2C_write(data_value);
     SW_I2C_wait_ACK();
     SW_I2C_stop();
}


unsigned char PCF8591_read(unsigned char control_value)
{
     unsigned char value = 0;

     SW_I2C_start();
     SW_I2C_write(PCF8591_write_cmd);
     SW_I2C_wait_ACK();
     SW_I2C_write((control_value & 0xFF));
     SW_I2C_ACK_NACK(I2C_ACK);
     SW_I2C_stop();

     SW_I2C_start();
     SW_I2C_write(PCF8591_read_cmd);
     SW_I2C_wait_ACK();
     value = SW_I2C_read(0);
     SW_I2C_wait_ACK();
     SW_I2C_stop();

     return value;
}
