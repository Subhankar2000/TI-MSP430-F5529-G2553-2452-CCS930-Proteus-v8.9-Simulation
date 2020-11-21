#include <msp430.h> 
#include "delay.h"
#include "lcd.h"


unsigned char rx = 0;


void GPIO_graceInit(void);
void BCSplus_graceInit(void);
void USCI_A0_graceInit(void);
void System_graceInit(void);
void WDTplus_graceInit(void);
void UART_putc(char ch);
void UART_puts(char *str);


#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR_HOOK(void)
{
    rx = UCA0RXBUF;
}


void main(void)
{
    unsigned char tx = 0x20;

    /* Stop watchdog timer from timing out during initial start-up. */
    WDTCTL = WDTPW | WDTHOLD;

    /* initialize Config for the MSP430 GPIO */
    GPIO_graceInit();

    /* initialize Config for the MSP430 2xx family clock systems (BCS) */
    BCSplus_graceInit();

    /* initialize Config for the MSP430 USCI_A0 */
    USCI_A0_graceInit();

    /* initialize Config for the MSP430 System Registers */
    System_graceInit();

    /* initialize Config for the MSP430 WDT+ */
    WDTplus_graceInit();

    UART_puts("\f");
    UART_puts("MSP430G2553 UART Demo\n");
    UART_puts("Shawon Shahryiar\n");
    UART_puts("https://www.facebook.com/MicroArena\n");

    LCD_init();

    LCD_goto(0, 0);
    LCD_putstr("TXD:");
    LCD_goto(0, 1);
    LCD_putstr("RXD:");

    while(1)
    {
        LCD_goto(15, 0);
        LCD_putchar(tx);
        UART_putc(tx);
        tx++;

        if(tx > 0x7F)
        {
            tx = 0x20;
        }

        LCD_goto(15, 1);
        LCD_putchar(rx);

        delay_ms(400);
    };
}


void GPIO_graceInit(void)
{
    /* USER CODE START (section: GPIO_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: GPIO_graceInit_prologue) */

    /* Port 1 Port Select 2 Register */
    P1SEL2 = BIT1 | BIT2;

    /* Port 1 Output Register */
    P1OUT = 0;

    /* Port 1 Port Select Register */
    P1SEL = BIT1 | BIT2;

    /* Port 1 Direction Register */
    P1DIR = 0;

    /* Port 1 Interrupt Edge Select Register */
    P1IES = 0;

    /* Port 1 Interrupt Flag Register */
    P1IFG = 0;

    /* Port 2 Output Register */
    P2OUT = 0;

    /* Port 2 Direction Register */
    P2DIR = BIT0 | BIT1 | BIT2;

    /* Port 2 Interrupt Edge Select Register */
    P2IES = 0;

    /* Port 2 Interrupt Flag Register */
    P2IFG = 0;

    /* Port 3 Output Register */
    P3OUT = 0;

    /* Port 3 Direction Register */
    P3DIR = 0;

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
     * LFXT1S_0 -- If XTS = 0, XT1 = 32768kHz Crystal ; If XTS = 1, XT1 = 0.4 - 1-MHz crystal or resonator
     * XCAP_1 -- ~6 pF
     */
    BCSCTL3 = XT2S_0 | LFXT1S_0 | XCAP_1;

    /* USER CODE START (section: BCSplus_graceInit_epilogue) */
    /* User code */
    /* USER CODE END (section: BCSplus_graceInit_epilogue) */
}


void USCI_A0_graceInit(void)
{
    /* USER CODE START (section: USCI_A0_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: USCI_A0_graceInit_prologue) */

    /* Disable USCI */
    UCA0CTL1 |= UCSWRST;

    /*
     * Control Register 0
     *
     * ~UCPEN -- Parity Disabled
     * UCPAR -- Even parity
     * ~UCMSB -- LSB first
     * ~UC7BIT -- 8-bit
     * ~UCSPB -- One stop bit
     * UCMODE_0 -- UART Mode
     * ~UCSYNC -- Asynchronous mode
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    UCA0CTL0 = UCPAR | UCMODE_0;

    /*
     * Control Register 1
     *
     * UCSSEL_2 -- SMCLK
     * ~UCRXEIE -- Erroneous characters rejected and UCAxRXIFG is not set
     * ~UCBRKIE -- Received break characters do not set UCAxRXIFG
     * ~UCDORM -- Not dormant. All received characters will set UCAxRXIFG
     * ~UCTXADDR -- Next frame transmitted is data
     * ~UCTXBRK -- Next frame transmitted is not a break
     * UCSWRST -- Enabled. USCI logic held in reset state
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    UCA0CTL1 = UCSSEL_2 | UCSWRST;

    /*
     * Modulation Control Register
     *
     * UCBRF_0 -- First stage 0
     * UCBRS_1 -- Second stage 1
     * ~UCOS16 -- Disabled
     *
     * Note: ~UCOS16 indicates that UCOS16 has value zero
     */
    UCA0MCTL = UCBRF_0 | UCBRS_1;

    /* Baud rate control register 0 */
    UCA0BR0 = 104;

    /* Enable USCI */
    UCA0CTL1 &= ~UCSWRST;

    /* USER CODE START (section: USCI_A0_graceInit_epilogue) */
    /* User code */
    /* USER CODE END (section: USCI_A0_graceInit_epilogue) */
}


void System_graceInit(void)
{
    /* USER CODE START (section: System_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: System_graceInit_prologue) */

    /*
     * IFG2, Interrupt Flag Register 2
     *
     * ~UCB0TXIFG -- No interrupt pending
     * ~UCB0RXIFG -- No interrupt pending
     * ~UCA0TXIFG -- No interrupt pending
     * UCA0RXIFG -- Interrupt pending
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    IFG2 &= ~(UCA0RXIFG);

    /*
     * IE2, Interrupt Enable Register 2
     *
     * ~UCB0TXIE -- Interrupt disabled
     * ~UCB0RXIE -- Interrupt disabled
     * ~UCA0TXIE -- Interrupt disabled
     * UCA0RXIE -- Interrupt enabled
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    IE2 |= UCA0RXIE;

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


void UART_putc(char ch)
{
    while(!(IFG2 & UCA0TXIFG));
    UCA0TXBUF = ch;
}


void UART_puts(char *str)
{
    while(*str != 0)
    {
        while(!(IFG2 & UCA0TXIFG));
        UCA0TXBUF = *str++;
    };
}
