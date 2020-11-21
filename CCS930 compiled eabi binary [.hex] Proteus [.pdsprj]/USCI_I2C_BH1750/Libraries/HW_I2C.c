#include "HW_I2C.h"


void I2C_USCI_init(unsigned char address)
{
    P1DIR &= ~(BIT6 + BIT7);
    P1OUT |= (BIT6 + BIT7);
    P1SEL2 |= (BIT6 | BIT7);
    P1SEL |= (BIT6 | BIT7);
    
    UCB0CTL1 |= UCSWRST;
    UCB0CTL0 = (UCMST | UCMODE_3 | UCSYNC);
    UCB0CTL1 = (UCSSEL_2 | UCSWRST);
    UCB0BR0 = 20;
    UCB0I2CSA = address;
    UCB0CTL1 &= ~UCSWRST;
}


void I2C_USCI_set_address(unsigned char address)
{
    UCB0CTL1 |= UCSWRST;
    UCB0I2CSA = address;
    UCB0CTL1 &= ~UCSWRST;
}


unsigned char I2C_USCI_read_byte(unsigned char address)
{
    while(UCB0CTL1 & UCTXSTP);
    UCB0CTL1 |= (UCTR | UCTXSTT);

    while(!(IFG2 & UCB0TXIFG));
    UCB0TXBUF = address;

    while(!(IFG2 & UCB0TXIFG));
    UCB0CTL1 &= ~UCTR;
    UCB0CTL1 |= UCTXSTT;
    IFG2 &= ~UCB0TXIFG;

    while(UCB0CTL1 & UCTXSTT);
    UCB0CTL1 |= UCTXSTP;

    return UCB0RXBUF;
}


unsigned char I2C_USCI_read_word(unsigned char address,unsigned char *value, unsigned char length)
{
    unsigned char i = 0;

    while (UCB0CTL1 & UCTXSTP);

    UCB0CTL1 |= (UCTR | UCTXSTT);

    while (!(IFG2 & UCB0TXIFG));
    
    IFG2 &= ~UCB0TXIFG;
    
    if(UCB0STAT & UCNACKIFG) 
    {
        return UCB0STAT;
    }

    UCB0TXBUF = address;

    while (!(IFG2 & UCB0TXIFG));
    
    if(UCB0STAT & UCNACKIFG) 
    {
        return UCB0STAT;
    }

    UCB0CTL1 &= ~UCTR;
    UCB0CTL1 |= UCTXSTT;
    IFG2 &= ~UCB0TXIFG;
    
    while (UCB0CTL1 & UCTXSTT);

    for(i = 0; i < (length - 1); i++)
    {
        while (!(IFG2&UCB0RXIFG));
        IFG2 &= ~UCB0TXIFG;
        value[i] = UCB0RXBUF;
    }

    while (!(IFG2 & UCB0RXIFG));

    IFG2 &= ~UCB0TXIFG;
    UCB0CTL1 |= UCTXSTP;
    value[length - 1] = UCB0RXBUF;
    IFG2 &= ~UCB0TXIFG;

    return 0;
}

unsigned char I2C_USCI_write_byte(unsigned char address, unsigned char value)
{
    while(UCB0CTL1 & UCTXSTP);

    UCB0CTL1 |= (UCTR | UCTXSTT);

    while(!(IFG2 & UCB0TXIFG));
    
    if(UCB0STAT & UCNACKIFG) 
    {
        return UCB0STAT;
    }

    UCB0TXBUF = address;


    while(!(IFG2 & UCB0TXIFG));

    if(UCB0STAT & UCNACKIFG) 
    {
        return UCB0STAT;
    }

    UCB0TXBUF = value;

    while(!(IFG2 & UCB0TXIFG));

    if(UCB0STAT & UCNACKIFG) 
    {
        return UCB0STAT;
    }

    UCB0CTL1 |= UCTXSTP;
    IFG2 &= ~UCB0TXIFG;

    return 0;
}
