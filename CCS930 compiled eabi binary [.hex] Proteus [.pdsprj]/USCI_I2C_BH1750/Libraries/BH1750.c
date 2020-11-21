#include "BH1750.h"


void BH1750_init(void)
{ 
   I2C_USCI_init(BH1750_addr);
   delay_ms(10);  
   BH1750_write(power_down);
}                


void BH1750_write(unsigned char cmd)
{  
    I2C_USCI_write_byte(BH1750_addr, cmd);
}


unsigned int BH1750_read_word(void)
{                      
  unsigned long value = 0x0000;
  unsigned char bytes[2] = {0x00, 0x00};

  I2C_USCI_read_word(0x11, bytes, 2);   

  value = ((bytes[1] << 8) | bytes[0]);  

  return value;
} 


unsigned int get_lux_value(unsigned char mode, unsigned int delay_time)
{
  unsigned long lux_value = 0x00;  
  unsigned char dly = 0x00;
  unsigned char s = 0x08;

  while(s)
  {
     BH1750_write(power_up);
     BH1750_write(mode);
     lux_value += BH1750_read_word();
     for(dly = 0; dly < delay_time; dly += 1)
     {
         delay_ms(1);
     }
     BH1750_write(power_down);
   s--;
  }
  lux_value >>= 3;

  return ((unsigned int)lux_value);
}                                
