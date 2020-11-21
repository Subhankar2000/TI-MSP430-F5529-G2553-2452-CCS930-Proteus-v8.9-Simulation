#include <msp430.h>
#include "delay.h"


#define SW_UART_RXD_DIR			P1DIR
#define SW_UART_TXD_DIR			P1DIR
#define SW_UART_RXD_OUT         P1OUT
#define SW_UART_TXD_OUT			P1OUT
#define SW_UART_RXD_IN			P1IN
#define SW_UART_RXD_IN_RES		P1REN

#define SW_UART_RXD_PIN			BIT1
#define SW_UART_TXD_PIN			BIT2

#define SW_UART_RXD_DIR_IN()	do{SW_UART_RXD_OUT |= SW_UART_RXD_PIN; SW_UART_RXD_DIR &= ~SW_UART_RXD_PIN; SW_UART_RXD_IN_RES |= SW_UART_RXD_PIN;}while(0)
#define SW_UART_TXD_DIR_OUT()	do{SW_UART_TXD_DIR |= SW_UART_TXD_PIN;}while(0)

#define SW_UART_TXD_OUT_HIGH()	do{SW_UART_TXD_OUT |= SW_UART_TXD_PIN;}while(0)
#define SW_UART_TXD_OUT_LOW()	do{SW_UART_TXD_OUT &= ~SW_UART_TXD_PIN;}while(0)

#define SW_UART_RXD_INPUT()     (SW_UART_RXD_IN & SW_UART_RXD_PIN)

#define baudrate				4800
#define no_of_bits				8
#define one_bit_delay			(1000000 / baudrate)
#define half_bit_delay			(one_bit_delay / 2)


void SW_UART_init(void);
void SW_UART_transmit(unsigned char value);
unsigned char SW_UART_receive(void);
