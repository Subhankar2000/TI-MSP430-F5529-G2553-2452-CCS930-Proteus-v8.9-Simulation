#include <msp430.h>


void HW_SPI_init(void);
void SPI_write(unsigned char tx_data);
unsigned char SPI_read(void);
unsigned char SPI_transfer(unsigned char tx_data);
