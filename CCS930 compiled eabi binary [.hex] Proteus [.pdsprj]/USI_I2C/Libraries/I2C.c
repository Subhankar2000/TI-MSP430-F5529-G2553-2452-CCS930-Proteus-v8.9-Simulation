#include "I2C.h"


static unsigned char usi_cnt_flag = FALSE;


// function to generate I2C START condition
void i2c_usi_mst_gen_start(void)
{
  // make sure SDA line is in HIGH level
  FORCING_SDA_HIGH();

  // small delay
  _delay_cycles(100);

  // pull down SDA to create START condition
  FORCING_SDA_LOW();
}


// function to generate I2C REPEATED START condition
void i2c_usi_mst_gen_repeated_start(void)
{
  USICTL0 |= USIOE;
  USISRL = 0xFF;
  USICNT = 1;

  // wait for USIIFG is set
  i2c_usi_mst_wait_usi_cnt_flag();

  // small delay
  _delay_cycles(100);

  // pull down SDA to create START condition
  FORCING_SDA_LOW();

  // small delay
  _delay_cycles(100);
}


// function to generate I2C STOP condition
void i2c_usi_mst_gen_stop(void)
{
  USICTL0 |= USIOE;
  USISRL = 0x00;
  USICNT = 1;

  // wait for USIIFG is set
  i2c_usi_mst_wait_usi_cnt_flag();

  FORCING_SDA_HIGH();
}


// function to wait for I2C counter flag condition
void i2c_usi_mst_wait_usi_cnt_flag(void)
{
  while(usi_cnt_flag == FALSE)
  {
    //__bis_SR_register(LPM0_bits);
  }

  // reset flag
  usi_cnt_flag = FALSE;
}


// function to send a byte
unsigned char i2c_usi_mst_send_byte(unsigned char data_byte)
{
  // send address and R/W bit
  SET_SDA_AS_OUTPUT();
  USISRL = data_byte;
  USICNT = (USICNT & 0xE0) + 8;

  // wait until USIIFG is set
  i2c_usi_mst_wait_usi_cnt_flag();

  // check NACK/ACK
  SET_SDA_AS_INPUT();
  USICNT = (USICNT & 0xE0) + 1;

  // wait for USIIFG is set
  i2c_usi_mst_wait_usi_cnt_flag();

  if(USISRL & 0x01)
  {
    // NACK received returns FALSE
    return FALSE;
  }

  return TRUE;
}


// function to read a byte
unsigned char i2c_usi_mst_read_byte(void)
{
  SET_SDA_AS_INPUT();
  USICNT = (USICNT & 0xE0) + 8;

  // wait for USIIFG is set
  i2c_usi_mst_wait_usi_cnt_flag();

  return USISRL;
}


// function to send (N)ACK bit
void i2c_usi_mst_send_n_ack(unsigned char ack)
{
  // send (N)ack bit
  SET_SDA_AS_OUTPUT();
  if(ack)
  {
    USISRL = 0x00;
  }
  else
  {
    USISRL = 0xFF;
  }
  USICNT = (USICNT & 0xE0) + 1;

  // wait until USIIFG is set
  i2c_usi_mst_wait_usi_cnt_flag();

  // set SDA as input
  SET_SDA_AS_INPUT();
}


// function to send I2C address with R/W bit
unsigned char i2c_usi_mst_send_address(unsigned char addr, unsigned char r_w)
{
  addr <<= 1;
  if(r_w)
  {
    addr |= 0x01;
  }
  return(i2c_usi_mst_send_byte(addr));
}


// USI I2C ISR function
#pragma vector=USI_VECTOR
__interrupt void USI_ISR (void)
{
  if(USICTL1 & USISTTIFG)
  {
    // do something if necessary

    // clear flag
    USICTL1 &= ~USISTTIFG;
  }

  if(USICTL1 & USIIFG)
  {
    // USI counter interrupt flag
    usi_cnt_flag = TRUE;

    // clear flag
    USICTL1 &= ~USIIFG;
  }

  //__bic_SR_register_on_exit(LPM0_bits);
}
