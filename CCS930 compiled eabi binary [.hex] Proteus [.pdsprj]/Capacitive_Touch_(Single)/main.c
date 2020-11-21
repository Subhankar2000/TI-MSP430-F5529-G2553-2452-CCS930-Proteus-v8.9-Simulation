#include <msp430.h> 
#include "CTS_Layer.h"
#include "CTS_HAL.h"
#include "structure.h"


#define DELAY 4000


void GPIO_graceInit(void);
void BCSplus_graceInit(void);
void sleep(unsigned int time);


#pragma vector = TIMER0_A0_VECTOR
__interrupt void ISR_Timer0_A0(void)
{
  TA0CTL &= ~MC_1;
  TA0CCTL0 &= ~(CCIE);
  __bic_SR_register_on_exit(LPM3_bits + GIE);
}


#pragma vector = PORT2_VECTOR,             \
  PORT1_VECTOR,                          \
  TIMER0_A1_VECTOR,                      \
  USI_VECTOR,                            \
  NMI_VECTOR,COMPARATORA_VECTOR,         \
  ADC10_VECTOR
__interrupt void ISR_trap(void)
{
  // the following will cause an access violation which results in a PUC reset
  WDTCTL = 0;
}



// Main Function
void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer

  GPIO_graceInit();
  BCSplus_graceInit();

  TI_CAPT_Init_Baseline(&one_button);
  TI_CAPT_Update_Baseline(&one_button, 9);

  while (1)
  {
    if(TI_CAPT_Button(&one_button))
    {
        P1OUT ^= BIT0;
        P1OUT |= BIT6;
    }

    else
    {
        P1OUT &= ~BIT6;
    }

    sleep(DELAY);

  }
} // End Main


void GPIO_graceInit(void)
{
    /* USER CODE START (section: GPIO_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: GPIO_graceInit_prologue) */

    /* Port 1 Output Register */
    P1OUT = 0;

    /* Port 1 Direction Register */
    P1DIR = BIT0 | BIT6;

    /* Port 1 Interrupt Edge Select Register */
    P1IES = 0;

    /* Port 1 Interrupt Flag Register */
    P1IFG = 0;

    /* Port 2 Output Register */
    P2OUT = 0;

    /* Port 2 Port Select Register */
    P2SEL &= ~(BIT6 | BIT7);

    /* Port 2 Direction Register */
    P2DIR = 0;

    /* Port 2 Interrupt Edge Select Register */
    P2IES = 0;

    /* Port 2 Interrupt Flag Register */
    P2IFG = 0;

    /* USER CODE START (section: GPIO_graceInit_epilogue) */
    /* User code */
    /* USER CODE END (section: GPIO_graceInit_epilogue) */

}


void BCSplus_graceInit(void)
{
    /* USER CODE START (section: BCSplus_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: BCSplus_graceInit_prologue) */

    /*
     * Basic Clock System Control 2
     *
     * SELM_0 -- DCOCLK
     * DIVM_0 -- Divide by 1
     * ~SELS -- DCOCLK
     * DIVS_3 -- Divide by 8
     * ~DCOR -- DCO uses internal resistor
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    BCSCTL2 = SELM_0 | DIVM_0 | DIVS_3;

    if (CALBC1_12MHZ != 0xFF) {
        /* Adjust this accordingly to your VCC rise time */
        __delay_cycles(100000);

        /* Follow recommended flow. First, clear all DCOx and MODx bits. Then
         * apply new RSELx values. Finally, apply new DCOx and MODx bit values.
         */
        DCOCTL = 0x00;
        BCSCTL1 = CALBC1_12MHZ;     /* Set DCO to 12MHz */
        DCOCTL = CALDCO_12MHZ;
    }

    /*
     * Basic Clock System Control 1
     *
     * XT2OFF -- Disable XT2CLK
     * ~XTS -- Low Frequency
     * DIVA_0 -- Divide by 1
     *
     * Note: ~XTS indicates that XTS has value zero
     */
    BCSCTL1 |= XT2OFF | DIVA_0;

    /*
     * Basic Clock System Control 3
     *
     * XT2S_0 -- 0.4 - 1 MHz
     * LFXT1S_2 -- If XTS = 0, XT1 = VLOCLK ; If XTS = 1, XT1 = 3 - 16-MHz crystal or resonator
     * XCAP_1 -- ~6 pF
     */
    BCSCTL3 = XT2S_0 | LFXT1S_2 | XCAP_1;

    /* USER CODE START (section: BCSplus_graceInit_epilogue) */
    /* User code */
    /* USER CODE END (section: BCSplus_graceInit_epilogue) */
}


void sleep(unsigned int time)
{
    TA0CCR0 = time;
    TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;
    TA0CCTL0 &= ~CCIFG;
    TA0CCTL0 |= CCIE;
    __bis_SR_register(LPM3_bits+GIE);
    __no_operation();
}
