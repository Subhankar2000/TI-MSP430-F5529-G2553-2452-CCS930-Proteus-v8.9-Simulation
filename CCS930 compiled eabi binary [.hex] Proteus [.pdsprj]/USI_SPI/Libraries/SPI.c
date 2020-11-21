#include "SPI.h"


unsigned char SPI_transfer(unsigned char data_out)
{
    unsigned char data_in = 0;

    USISRL = data_out;              // Load shift register with data byte to be TXed
    USICNT = 8;                     // Load bit-counter to send/receive data byte
    while (!(USIIFG & USICTL1));    // Loop until data byte transmitted
    data_in = USISRL;               // Read out the received data

    return data_in;
}
