#include "MPL115A1.h"


void MPL115A1_init(void)
{
  MPL115A1_SDN_PORT_DIR |= MPL115A1_SDN_pin;
  MPL115A1_CSN_PORT_DIR |= MPL115A1_CSN_pin;

  MPL115A1_SDN_HIGH();
  MPL115A1_CSN_HIGH();
  HW_SPI_init();
  MPL115A1_get_coefficients();
}       
                                        

unsigned char MPL115A1_read(unsigned char address)
{
  unsigned char value = 0;

  MPL115A1_CSN_LOW();
  delay_ms(3);
  SPI_write(address);
  value = SPI_read();

  value = SPI_transfer(address);
  MPL115A1_CSN_HIGH();
  
  return value;        
}                                           
                                               
    
void MPL115A1_write(unsigned char address, unsigned char value)
{                                                     
  MPL115A1_CSN_LOW();
  delay_ms(3);
  SPI_write((address & 0x7F));
  SPI_write(value);
  MPL115A1_CSN_HIGH();
} 


void MPL115A1_get_coefficients(void)
{     
  unsigned int hb = 0;
  unsigned int lb = 0;
    
  MPL115A1_get_bytes(&hb, &lb, A0_H);
  coefficients.A0 = ((hb << 5) + (lb >> 3) + ((lb & 0x07) / 8.0));
            
  MPL115A1_get_bytes(&hb, &lb, B1_H);              
  coefficients.B1 = (((((hb & 0x1F) * 0x0100) + lb) / 8192.0) - 3.0);
    
  MPL115A1_get_bytes(&hb, &lb, B2_H);    
  coefficients.B2 = (((((hb - 0x80) << 8) + lb) / 16384.0) - 2.0);
                                        
  MPL115A1_get_bytes(&hb, &lb, C12_H);                 
  coefficients.C12 = (((hb * 0x100) + lb) / 16777216.0);
}  


void MPL115A1_get_bytes(unsigned int *hb, unsigned int *lb, unsigned char address) 
{ 
  *hb = ((unsigned int)MPL115A1_read(address));
  *lb = ((unsigned int)MPL115A1_read((address + 2)));
}
                                                                                                          
                 
void MPL115A1_get_data(float *pres, float *temp)   
{   
   unsigned int hb = 0;
   unsigned int lb = 0;

   signed long Padc = 0;
   signed long Tadc = 0;
   
   MPL115A1_write(conv_cmd, 0); 

   MPL115A1_get_bytes(&hb, &lb, PRESH);
   Padc = (((hb << 8) + lb) >> 6);
   
   MPL115A1_get_bytes(&hb, &lb, TEMPH); 
   Tadc = (((hb << 8) + lb) >> 6);
   
   *pres = ( coefficients.A0 + (( coefficients.B1 + ( coefficients.C12 * Tadc)) * Padc) + ( coefficients.B2 * Tadc));
   *pres = (((*pres * 65.0) / 1023.0) + 50.0);      
   
   *temp = (30.0 + ((Tadc - 472) / (-5.35)));
} 
