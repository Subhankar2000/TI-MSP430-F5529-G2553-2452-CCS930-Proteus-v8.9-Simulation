#include <msp430.h>
#include <delay.h>


#define DHT22_DIR 						P2DIR
#define DHT22_OUT_PORT					P2OUT
#define DHT22_IN_PORT                   P2IN
#define DHT22_PIN 						BIT0

#define DHT22_DIR_OUT()					do{DHT22_DIR |= DHT22_PIN;}while(0)
#define DHT22_DIR_IN()					do{DHT22_DIR &= ~DHT22_PIN;}while(0)

#define DHT22_IN()						(DHT22_IN_PORT & DHT22_PIN)

#define DHT22_OUT_LOW()					do{DHT22_OUT_PORT &= ~DHT22_PIN;}while(0)
#define DHT22_OUT_HIGH()				do{DHT22_OUT_PORT |= DHT22_PIN;}while(0)

#define TRUE                            1
#define FALSE                           0


extern unsigned char values[5]; 


void DHT22_init(void);
unsigned char DHT22_get_byte(void);
unsigned char DHT22_get_data(void);
