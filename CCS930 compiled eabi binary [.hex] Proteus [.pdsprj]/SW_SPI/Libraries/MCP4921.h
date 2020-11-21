#include <msp430.h>
#include "delay.h"


#define SW_SPI_DIR          P2DIR
#define SW_SPI_OUT          P2OUT
#define SW_SPI_IN           P2IN

#define SCK_pin             BIT0
#define CS_pin              BIT1
#define SDI_pin             BIT2
#define LDAC_pin            BIT3

#define SCK_DIR_OUT()       do{SW_SPI_DIR |= SCK_pin;}while(0)
#define SCK_DIR_IN()        do{SW_SPI_DIR &= ~SCK_pin;}while(0)
#define CS_DIR_OUT()        do{SW_SPI_DIR |= CS_pin;}while(0)
#define CS_DIR_IN()         do{SW_SPI_DIR &= ~CS_pin;}while(0)
#define SDI_DIR_OUT()       do{SW_SPI_DIR |= SDI_pin;}while(0)
#define SDI_DIR_IN()        do{SW_SPI_DIR &= ~SDI_pin;}while(0)
#define LDAC_DIR_OUT()      do{SW_SPI_DIR |= LDAC_pin;}while(0)
#define LDAC_DIR_IN()       do{SW_SPI_DIR &= ~LDAC_pin;}while(0)

#define SCK_HIGH()          do{SW_SPI_OUT |= SCK_pin;}while(0)
#define SCK_LOW()           do{SW_SPI_OUT &= ~SCK_pin;}while(0)
#define CS_HIGH()           do{SW_SPI_OUT |= CS_pin;}while(0)
#define CS_LOW()            do{SW_SPI_OUT &= ~CS_pin;}while(0)
#define SDI_HIGH()          do{SW_SPI_OUT |= SDI_pin;}while(0)
#define SDI_LOW()           do{SW_SPI_OUT &= ~SDI_pin;}while(0)
#define LDAC_HIGH()         do{SW_SPI_OUT |= LDAC_pin;}while(0)
#define LDAC_LOW()          do{SW_SPI_OUT &= ~LDAC_pin;}while(0)
                                       
#define ignore_cmd          0x80
#define DAC_write_cmd       0x00
#define Buffer_on           0x40
#define Buffer_off          0x00
#define Gain_1X             0x20
#define Gain_2X             0x00
#define Run_cmd             0x10
#define Shutdown            0x00


void MCP4921_init(void);
void MCP4921_write(unsigned char cmd, unsigned int dac_value);
