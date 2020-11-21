/*
 * ======== Standard MSP430 includes ========
 */
#include <msp430.h>

/*
 * ======== Grace related declaration ========
 */
extern void Grace_init(void);

/*
 *  ======== main ========
 */
int main( void )
{
    Grace_init();                     // Activate Grace-generated configuration
    
    // >>>>> Fill-in user code here <<<<<
    
    return (0);
}
