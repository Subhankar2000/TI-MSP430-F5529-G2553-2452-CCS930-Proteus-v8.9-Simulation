#include "MAX72xx.h"


void MAX72xx_init(void)
{
    CS_DIR_OUT();
    CS_HIGH();

    MAX72xx_write(shutdown_reg, run_cmd);                 
    MAX72xx_write(decode_mode_reg, 0x00);
    MAX72xx_write(scan_limit_reg, 0x07);
    MAX72xx_write(intensity_reg, 0x04);
    MAX72xx_write(display_test_reg, test_cmd);
    delay_ms(100);     
    MAX72xx_write(display_test_reg, no_test_cmd);            
}


void MAX72xx_write(unsigned char address, unsigned char value)
{
    CS_LOW();

    SPI_transfer(address);
    SPI_transfer(value);

    CS_HIGH();
}
