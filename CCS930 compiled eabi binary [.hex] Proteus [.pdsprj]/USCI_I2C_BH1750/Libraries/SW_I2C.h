#include <msp430.h>
#include "delay.h"


#define SW_I2C_DIR      P2DIR
#define SW_I2C_OUT      P2OUT
#define SW_I2C_IN       P2IN

#define SDA_pin         BIT0
#define SCL_pin         BIT1

#define SDA_DIR_OUT()   do{SW_I2C_DIR |= SDA_pin;}while(0)
#define SDA_DIR_IN()    do{SW_I2C_DIR &= ~SDA_pin;}while(0)
#define SCL_DIR_OUT()   do{SW_I2C_DIR |= SCL_pin;}while(0)
#define SCL_DIR_IN()    do{SW_I2C_DIR &= ~SCL_pin;}while(0)

#define SDA_HIGH()      do{SW_I2C_OUT |= SDA_pin;}while(0)
#define SDA_LOW()       do{SW_I2C_OUT &= ~SDA_pin;}while(0)
#define SCL_HIGH()      do{SW_I2C_OUT |= SCL_pin;}while(0)
#define SCL_LOW()       do{SW_I2C_OUT &= ~SCL_pin;}while(0)

#define SDA_IN()        (SW_I2C_IN & SDA_pin)

#define I2C_ACK         0xFF
#define I2C_NACK        0x00

#define I2C_timeout     1000


void SW_I2C_init(void);
void SW_I2C_start(void);
void SW_I2C_stop(void);
unsigned char SW_I2C_read(unsigned char ack);
void SW_I2C_write(unsigned char value);
void SW_I2C_ACK_NACK(unsigned char mode);
unsigned char SW_I2C_wait_ACK(void);
