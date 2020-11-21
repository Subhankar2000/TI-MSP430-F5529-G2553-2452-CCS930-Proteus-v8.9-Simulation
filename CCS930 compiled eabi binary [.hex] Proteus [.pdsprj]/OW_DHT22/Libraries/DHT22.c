#include "DHT22.h"


unsigned char values[5];


void DHT22_init(void)
{
   DHT22_DIR_IN();
   delay_ms(1000);
}


unsigned char DHT22_get_byte(void)
{
   unsigned char s = 8;
   unsigned char value = 0;
   
   DHT22_DIR_IN();

   while(s > 0)
   {
      value <<= 1;
      
      while(DHT22_IN() == FALSE);
      delay_us(30);
      
      if(DHT22_IN())
      {
          value |= 1;
      }

      while(DHT22_IN());
      s--;
   }

   return value;
}


unsigned char DHT22_get_data(void)
{
	unsigned char chk = FALSE;
	unsigned char s = 0;
	unsigned char check_sum = 0;
	
	DHT22_DIR_OUT();
	
	DHT22_OUT_HIGH();
	DHT22_OUT_LOW();
	
	delay_ms(1);
	
	DHT22_OUT_HIGH();
	
	delay_us(32);
	DHT22_DIR_IN();
	
	chk = DHT22_IN();
	delay_us(2);
	
	if(chk == TRUE)
	{
		return 1;
	}
	
	delay_us(80);
	
	chk = DHT22_IN();
	
	if(chk == FALSE)
	{
		return 2;
	}
	
	delay_us(80);
	
	for(s = 0; s <= 4; s += 1)
	{
		values[s] = DHT22_get_byte();
	}
	
	DHT22_DIR_OUT();
	DHT22_OUT_HIGH(); 
	
	for(s = 0; s < 4; s++)
	{
	check_sum += values[s];
	}
	
	if(check_sum != values[4])
	{
		return 3;
	}
	else
	{
		return 0;
	}
}
