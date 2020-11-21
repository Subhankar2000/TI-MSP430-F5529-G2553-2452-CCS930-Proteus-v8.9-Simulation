#include "HW_SPI.h"


void HW_SPI_init(void)
{
	UCA0CTL1 |= UCSWRST;
	UCA0CTL0 = UCCKPH | UCMSB | UCMST | UCMODE_1 | UCSYNC;
	UCA0CTL1 = UCSSEL_2 | UCSWRST;
	UCA0BR0 = 8;
	UCA0CTL1 &= ~UCSWRST;
}


void SPI_write(unsigned char tx_data)
{
	while(!(IFG2 & UCA0TXIFG));
	UCA0TXBUF = tx_data;
	while(UCA0STAT & UCBUSY);
}


unsigned char SPI_read(void)
{
	unsigned char rx_data = 0;

	while(!(IFG2 & UCA0RXIFG));
	rx_data = UCA0RXBUF;
	while(UCA0STAT & UCBUSY);

	return rx_data;
}


unsigned char SPI_transfer(unsigned char tx_data)
{
	unsigned char rx_data = 0;

	while(!(IFG2 & UCA0TXIFG));
	UCA0TXBUF = tx_data;
	while(UCA0STAT & UCBUSY);

	while(!(IFG2 & UCA0RXIFG));
	rx_data = UCA0RXBUF;
	while(UCA0STAT & UCBUSY);

	return rx_data;
}
