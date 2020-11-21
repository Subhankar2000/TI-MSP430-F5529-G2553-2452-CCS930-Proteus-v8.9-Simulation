#include <msp430.h> 
#include <string.h>
#include "delay.h"
#include "SPI.h"
#include "MAX72xx.h"


void GPIO_graceInit(void);
void BCSplus_graceInit(void);
void USI_graceInit(void);
void System_graceInit(void);
void WDTplus_graceInit(void);


void main(void)
{
    unsigned char i = 0;
    unsigned char j = 0;

    unsigned char temp[8];

    const unsigned char text[80] =
    {
        0x00, 0x7E, 0x04, 0x08, 0x08, 0x04, 0x7E, 0x00,       //M
        0x00, 0x42, 0x42, 0x7E, 0x7E, 0x42, 0x42, 0x00,       //I
        0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x24, 0x00,       //C
        0x00, 0x7E, 0x1A, 0x1A, 0x1A, 0x2A, 0x44, 0x00,       //R
        0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00,       //O
        0x00, 0x7C, 0x12, 0x12, 0x12, 0x12, 0x7C, 0x00,       //A
        0x00, 0x7E, 0x1A, 0x1A, 0x1A, 0x2A, 0x44, 0x00,       //R
        0x00, 0x7E, 0x7E, 0x4A, 0x4A, 0x4A, 0x42, 0x00,       //E
        0x00, 0x7E, 0x04, 0x08, 0x10, 0x20, 0x7E, 0x00,       //N
        0x00, 0x7C, 0x12, 0x12, 0x12, 0x12, 0x7C, 0x00        //A
    };

    const unsigned char symbols[56] =
    {
       0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA,
       0x3C, 0x42, 0x95, 0xA1, 0xA1, 0x95, 0x42, 0x3C,
       0xFF, 0xC3, 0xBD, 0xA5, 0xA5, 0xBD, 0xC3, 0xFF,
       0x99, 0x5A, 0x3C, 0xFF, 0xFF, 0x3C, 0x5A, 0x99,
       0x1C, 0x22, 0x41, 0x86, 0x86, 0x41, 0x22, 0x1C,
       0xDF, 0xDF, 0xD8, 0xFF, 0xFF, 0x1B, 0xFB, 0xFB,
       0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55
    };

    memset(temp, 0x00, sizeof(temp));

    /* Stop watchdog timer from timing out during initial start-up. */
    WDTCTL = WDTPW | WDTHOLD;

    /* initialize Config for the MSP430 GPIO */
    GPIO_graceInit();

    /* initialize Config for the MSP430 2xx family clock systems (BCS) */
    BCSplus_graceInit();

    /* initialize Config for the MSP430 USI */
    USI_graceInit();

    /* initialize Config for the MSP430 System Registers */
    System_graceInit();

    /* initialize Config for the MSP430 WDT+ */
    WDTplus_graceInit();

    MAX72xx_init();

    while(1)
    {
        for(i = 0; i < 80; i++)
        {
           for(j = 0; j < 8; j++)
           {
                 temp[j] = text[(i + j)];
                 MAX72xx_write((j + 1), temp[j]);
                 delay_ms(6);
           }
        }

        for(j = 0; j < 56; j = (j + 8))
        {
            for(i = 0; i < 8; i++)
            {
               MAX72xx_write((i + 1), symbols[(i + j)]);
            }

            delay_ms(2000);
        }
    };
}


void GPIO_graceInit(void)
{
    /* USER CODE START (section: GPIO_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: GPIO_graceInit_prologue) */

    /* Port 1 Output Register */
    P1OUT = 0;

    /* Port 1 Port Select Register */
    P1SEL = BIT5 | BIT6;

    /* Port 1 Direction Register */
    P1DIR = BIT4;

    /* Port 1 Interrupt Edge Select Register */
    P1IES = 0;

    /* Port 1 Interrupt Flag Register */
    P1IFG = 0;

    /* Port 2 Output Register */
    P2OUT = 0;

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
     * DIVS_0 -- Divide by 1
     * ~DCOR -- DCO uses internal resistor
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    BCSCTL2 = SELM_0 | DIVM_0 | DIVS_0;

    if (CALBC1_1MHZ != 0xFF) {
        /* Follow recommended flow. First, clear all DCOx and MODx bits. Then
         * apply new RSELx values. Finally, apply new DCOx and MODx bit values.
         */
        DCOCTL = 0x00;
        BCSCTL1 = CALBC1_1MHZ;      /* Set DCO to 1MHz */
        DCOCTL = CALDCO_1MHZ;
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


void USI_graceInit(void)
{
    /* USER CODE START (section: USI_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: USI_graceInit_prologue) */

    /* Disable USI */
    USICTL0 |= USISWRST;

    /*
     * USI Control Register 0
     *
     * ~USIPE7 -- USI function disabled
     * USIPE6 -- USI function enabled
     * USIPE5 -- USI function enabled
     * ~USILSB -- MSB first
     * USIMST -- Master mode
     * ~USIGE -- Output latch enable depends on shift clock
     * USIOE -- Output enabled
     * USISWRST -- USI logic held in reset state
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    USICTL0 = USIPE6 | USIPE5 | USIMST | USIOE | USISWRST;

    /*
     * USI Clock Control Register
     *
     * USIDIV_3 -- Divide by 8
     * USISSEL_2 -- SMCLK
     * USICKPL -- Inactive state is high
     * ~USISWCLK -- Input clock is low
     *
     * Note: ~USISWCLK indicates that USISWCLK has value zero
     */
    USICKCTL = USIDIV_3 | USISSEL_2 | USICKPL;

    /* Enable USI */
    USICTL0 &= ~USISWRST;

    /* USER CODE START (section: USI_graceInit_epilogue) */
    /* User code */
    /* USER CODE END (section: USI_graceInit_epilogue) */
}


void System_graceInit(void)
{
    /* USER CODE START (section: System_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: System_graceInit_prologue) */

    /* Clear oscillator fault flag with software delay */
    do
    {
        // Clear OSC fault flag
        IFG1 &= ~OFIFG;

        // 50us delay
        __delay_cycles(50);
    } while (IFG1 & OFIFG);

    /*
     * SR, Status Register
     *
     * ~SCG1 -- Disable System clock generator 1
     * ~SCG0 -- Disable System clock generator 0
     * ~OSCOFF -- Oscillator On
     * ~CPUOFF -- CPU On
     * GIE -- General interrupt enable
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    __bis_SR_register(GIE);

    /* USER CODE START (section: System_graceInit_epilogue) */
    /* User code */
    /* USER CODE END (section: System_graceInit_epilogue) */
}


void WDTplus_graceInit(void)
{
    /* USER CODE START (section: RTC_B_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: RTC_B_graceInit_prologue) */

    /*
     * WDTCTL, Watchdog Timer+ Register
     *
     * WDTPW -- Watchdog password
     * WDTHOLD -- Watchdog timer+ is stopped
     * ~WDTNMIES -- NMI on rising edge
     * ~WDTNMI -- Reset function
     * ~WDTTMSEL -- Watchdog mode
     * ~WDTCNTCL -- No action
     * ~WDTSSEL -- SMCLK
     * ~WDTIS0 -- Watchdog clock source bit0 disabled
     * ~WDTIS1 -- Watchdog clock source bit1 disabled
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    WDTCTL = WDTPW | WDTHOLD;

    /* USER CODE START (section: RTC_B_graceInit_epilogue) */
    /* User code */
    /* USER CODE END (section: RTC_B_graceInit_epilogue) */
}
