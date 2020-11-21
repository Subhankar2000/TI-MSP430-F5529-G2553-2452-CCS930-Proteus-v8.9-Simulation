#include <msp430.h>


#define FALSE                           0
#define TRUE                            1

#define wr                              FALSE
#define rd                              TRUE

#define SET_SDA_AS_OUTPUT()             (USICTL0 |= USIOE)
#define SET_SDA_AS_INPUT()              (USICTL0 &= ~USIOE)


#define FORCING_SDA_HIGH()             \
        {                              \
          USISRL = 0xFF;               \
          USICTL0 |= USIGE;            \
          USICTL0 &= ~(USIGE + USIOE); \
        }

#define FORCING_SDA_LOW()              \
        {                              \
          USISRL = 0x00;               \
          USICTL0 |= (USIGE + USIOE);  \
          USICTL0 &= ~USIGE;           \
        }


void i2c_usi_mst_gen_start(void);
void i2c_usi_mst_gen_repeated_start(void);
void i2c_usi_mst_gen_stop(void);
void i2c_usi_mst_wait_usi_cnt_flag(void);
unsigned char i2c_usi_mst_send_byte(unsigned char value);
unsigned char i2c_usi_mst_read_byte(void);
void i2c_usi_mst_send_n_ack(unsigned char ack);
unsigned char i2c_usi_mst_send_address(unsigned char addr, unsigned char r_w);
