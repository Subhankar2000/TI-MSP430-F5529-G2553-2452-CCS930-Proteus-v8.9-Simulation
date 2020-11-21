#include <msp430.h>
#include "delay.h"


#define DS18B20_DIR 					P2DIR
#define DS18B20_OUT_PORT				P2OUT
#define DS18B20_IN_PORT                 P2IN
#define DS18B20_PIN 					BIT0

#define DS18B20_OUTPUT()				do{DS18B20_DIR |= DS18B20_PIN;}while(0)
#define DS18B20_INPUT()					do{DS18B20_DIR &= ~DS18B20_PIN;}while(0)

#define DS18B20_IN()					(DS18B20_IN_PORT & DS18B20_PIN)

#define DS18B20_OUT_LOW()				do{DS18B20_OUT_PORT &= ~DS18B20_PIN;}while(0)
#define DS18B20_OUT_HIGH()				do{DS18B20_OUT_PORT |= DS18B20_PIN;}while(0)

#define TRUE                            1
#define FALSE                           0


unsigned char onewire_reset(void); 
void onewire_write_bit(unsigned char bit_value);
unsigned char onewire_read_bit(void);
void onewire_write(unsigned char value);    
unsigned char onewire_read(void);
