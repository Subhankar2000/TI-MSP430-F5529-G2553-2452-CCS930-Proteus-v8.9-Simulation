#include <msp430.h>
#include "delay.h"
#include "HW_SPI.h"


#define LOW                   			0
#define HIGH                  			1

#define PRESH              				0x80
#define PRESL              				0x82
#define TEMPH              				0x84
#define TEMPL              				0x86

#define A0_H               				0x88                                                
#define A0_L               				0x8A
#define B1_H               				0x8C
#define B1_L               				0x8E
#define B2_H               				0x90             
#define B2_L               				0x92
#define C12_H              				0x94
#define C12_L              				0x96                   

#define conv_cmd           				0x24             

#define MPL115A1_CSN_PORT_OUT			P2OUT
#define MPL115A1_SDN_PORT_OUT			P2OUT

#define MPL115A1_CSN_PORT_DIR			P2DIR
#define MPL115A1_SDN_PORT_DIR			P2DIR

#define MPL115A1_SDN_pin				BIT0
#define MPL115A1_CSN_pin				BIT1

#define MPL115A1_SDN_HIGH()				P2OUT |= MPL115A1_SDN_pin 
#define MPL115A1_SDN_LOW()				P2OUT &= ~MPL115A1_SDN_pin   
#define MPL115A1_CSN_HIGH()				P2OUT |= MPL115A1_CSN_pin 
#define MPL115A1_CSN_LOW()				P2OUT &= ~MPL115A1_CSN_pin   


struct
{
  float A0;
  float B1;
  float B2;
  float C12;
}coefficients; 
                                

void MPL115A1_init(void);
unsigned char MPL115A1_read(unsigned char address);
void MPL115A1_write(unsigned char address, unsigned char value);
void MPL115A1_get_coefficients(void);
void MPL115A1_get_bytes(unsigned int *hb, unsigned int *lb, unsigned char address);
void MPL115A1_get_data(float *pres, float *temp);
