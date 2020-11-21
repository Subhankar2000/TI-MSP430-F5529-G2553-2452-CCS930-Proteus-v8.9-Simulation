#include "PCF8574.h"


unsigned char PCF8574_read(void)
{
    unsigned char port_byte = 0x00;

    i2c_usi_mst_gen_start();
    i2c_usi_mst_send_address(PCF8574_address, rd);
    port_byte = i2c_usi_mst_read_byte();
    i2c_usi_mst_send_n_ack(0);
    i2c_usi_mst_gen_stop();

    return port_byte;
}


void PCF8574_write(unsigned char data_byte)
{
    i2c_usi_mst_gen_start();
    i2c_usi_mst_send_address(PCF8574_address, wr);
    i2c_usi_mst_send_byte(data_byte);
    i2c_usi_mst_gen_stop();
}
