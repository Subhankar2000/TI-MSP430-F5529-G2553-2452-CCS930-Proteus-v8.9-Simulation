#include "MCP4921.h"


void MCP4921_init(void)
{
   CS_DIR_OUT();
   SCK_DIR_OUT();
   SDI_DIR_OUT();
   LDAC_DIR_OUT();

   CS_HIGH();
   LDAC_HIGH();
   SCK_HIGH();
   SDI_HIGH();
}


void MCP4921_write(unsigned char cmd, unsigned int dac_value)
{
    unsigned char s = 16;
    
    unsigned int value = 0;
    
    value = cmd;
    value <<= 8;
    value |= (dac_value & 0x0FFF);
    
    CS_LOW();
    
    while(s > 0)
    {
        if((value & 0x8000) != 0)
        {
            SDI_HIGH();
        }
        else
        {
            SDI_LOW();
        }

        SCK_LOW();
        SCK_HIGH();
        value <<= 1;
        s--;
    }
    
    LDAC_LOW();
    CS_HIGH();
    delay_us(10);
    LDAC_HIGH();
}
