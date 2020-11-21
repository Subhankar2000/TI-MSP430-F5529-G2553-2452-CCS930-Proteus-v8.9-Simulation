#include <MSP430.h>
#include "delay.h"
#include "SPI.h"


#define HW_SPI_DIR              P1DIR
#define HW_SPI_OUT              P1OUT
#define HW_SPI_IN               P1IN

#define CS_pin                  BIT4

#define CS_DIR_OUT()            do{HW_SPI_DIR |= CS_pin;}while(0)
#define CS_DIR_IN()             do{HW_SPI_DIR &= ~CS_pin;}while(0)

#define CS_HIGH()               do{HW_SPI_OUT |= CS_pin;}while(0)
#define CS_LOW()                do{HW_SPI_OUT &= ~CS_pin;}while(0)

#define NOP                     0x00
#define DIG0                    0x01
#define DIG1                    0x02
#define DIG2                    0x03
#define DIG3                    0x04
#define DIG4                    0x05
#define DIG5                    0x06
#define DIG6                    0x07
#define DIG7                    0x08
#define decode_mode_reg         0x09
#define intensity_reg           0x0A
#define scan_limit_reg          0x0B
#define shutdown_reg            0x0C
#define display_test_reg        0x0F

#define shutdown_cmd            0x00
#define run_cmd                 0x01

#define no_test_cmd             0x00
#define test_cmd                0x01


void MAX72xx_init(void);
void MAX72xx_write(unsigned char address, unsigned char value);
