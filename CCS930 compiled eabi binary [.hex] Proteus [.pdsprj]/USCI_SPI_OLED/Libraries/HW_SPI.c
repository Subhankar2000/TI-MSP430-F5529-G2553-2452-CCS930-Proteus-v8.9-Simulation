#include "HW_SPI.h"


void HW_SPI_init(void)
{
  UCB0CTL1 |= UCSWRST;
  UCB0CTL0 = UCCKPH | UCMSB | UCMST | UCMODE_1 | UCSYNC;
  UCB0CTL1 = UCSSEL_2;
  UCB0BR0 = 8;
  UCB0BR1 = 0;
  UCB0CTL1 &= ~UCSWRST;
}


void SPI_write(unsigned char tx_data)
{
  while(!(IFG2 & UCB0TXIFG));
  UCB0TXBUF = tx_data;
  while(UCB0STAT & UCBUSY);
}


unsigned char SPI_read(void)
{
  unsigned char rx_data = 0;

  while(!(IFG2 & UCB0RXIFG));
  rx_data = UCB0RXBUF;
  while(UCB0STAT & UCBUSY);

  return rx_data;
}


unsigned char SPI_transfer(unsigned char tx_data)
{
  unsigned char rx_data = 0;

  while(!(IFG2 & UCB0TXIFG));
  UCB0TXBUF = tx_data;
  while(UCB0STAT & UCBUSY);

  while(!(IFG2 & UCB0RXIFG));
  rx_data = UCB0RXBUF;
  while(UCB0STAT & UCBUSY);

  return rx_data;
}
