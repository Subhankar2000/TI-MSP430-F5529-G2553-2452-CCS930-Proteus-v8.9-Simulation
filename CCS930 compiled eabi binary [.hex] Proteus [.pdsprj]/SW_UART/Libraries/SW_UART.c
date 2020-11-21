#include "SW_UART.h"


void SW_UART_init(void)
{
	SW_UART_TXD_DIR_OUT();
	SW_UART_RXD_DIR_IN();
	SW_UART_TXD_OUT_HIGH();
	delay_ms(10);
}


void SW_UART_transmit(unsigned char value)
{
	unsigned char bits = 0;

	SW_UART_TXD_OUT_LOW();
	delay_us(one_bit_delay);

	for(bits = 0; bits < no_of_bits; bits++)
	{
		if((value >> bits) & 0x01)
		{
			SW_UART_TXD_OUT_HIGH();
		}

		else
		{
			SW_UART_TXD_OUT_LOW();
		}

		delay_us(one_bit_delay);
	};

	SW_UART_TXD_OUT_HIGH();
	delay_us(one_bit_delay);
}


unsigned char SW_UART_receive(void)
{
	unsigned char bits = 0;
	unsigned char value = 0;

	while(SW_UART_RXD_INPUT());
	delay_us(one_bit_delay);
	delay_us(half_bit_delay);

	for(bits = 0; bits < no_of_bits; bits++)
	{
		if(SW_UART_RXD_INPUT())
		{
			value += (1 << bits);
		}

		delay_us(one_bit_delay);
	};

	if(SW_UART_RXD_INPUT())
	{
		delay_us(half_bit_delay);

		return value;
	}

	else
	{
		delay_us(half_bit_delay);

		return 0;
	}
}
