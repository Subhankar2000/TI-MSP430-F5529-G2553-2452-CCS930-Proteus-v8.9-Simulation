#include <msp430.h>
#include "delay.h"
#include "HW_I2C.h"
#include "DS1307.h"
#include "lcd.h"


struct
{
   unsigned char sec;
   unsigned char min;
   unsigned char hr;
   unsigned char day;
   unsigned char dt;
   unsigned char mt;
   unsigned char yr;
}rtc;


void GPIO_graceInit(void);
void BCSplus_graceInit(void);
void USCI_B0_graceInit(void);
void System_graceInit(void);
void WDTplus_graceInit(void);
void show_value(unsigned char x_pos, unsigned char y_pos, unsigned char value);
void display_time(void);


void main(void)
{
    rtc.sec = 30;
    rtc.min = 58;
    rtc.hr = 23;

    /* Stop watchdog timer from timing out during initial start-up. */
    WDTCTL = WDTPW | WDTHOLD;

    /* initialize Config for the MSP430 GPIO */
    GPIO_graceInit();

    /* initialize Config for the MSP430 2xx family clock systems (BCS) */
    BCSplus_graceInit();

    /* initialize Config for the MSP430 USCI_B0 */
    USCI_B0_graceInit();

    /* initialize Config for the MSP430 System Registers */
    System_graceInit();

    /* initialize Config for the MSP430 WDT+ */
    WDTplus_graceInit();

    LCD_init();
    LCD_clear_home();

    LCD_goto(0, 0);

    LCD_putstr("MSP430G USCI I2C");

    DS1307_init();
    set_time();

    while(1)
    {
        get_time();
        display_time();
    }
}


void GPIO_graceInit(void)
{
    /* USER CODE START (section: GPIO_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: GPIO_graceInit_prologue) */

    /* Port 1 Port Select 2 Register */
    P1SEL2 = BIT6 | BIT7;

    /* Port 1 Output Register */
    P1OUT = 0;

    /* Port 1 Port Select Register */
    P1SEL = BIT6 | BIT7;

    /* Port 1 Direction Register */
    P1DIR = 0;

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

    if (CALBC1_1MHZ != 0xFF)
    {
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


void USCI_B0_graceInit(void)
{
    /* USER CODE START (section: USCI_B0_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: USCI_B0_graceInit_prologue) */

    /* Disable USCI */
    UCB0CTL1 |= UCSWRST;

    /*
     * Control Register 0
     *
     * ~UCA10 -- Own address is a 7-bit address
     * ~UCSLA10 -- Address slave with 7-bit address
     * ~UCMM -- Single master environment. There is no other master in the system. The address compare unit is disabled
     * UCMST -- Master mode
     * UCMODE_3 -- I2C Mode
     * UCSYNC -- Synchronous Mode
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;

    /*
     * Control Register 1
     *
     * UCSSEL_2 -- SMCLK
     * ~UCTR -- Receiver
     * ~UCTXNACK -- Acknowledge normally
     * ~UCTXSTP -- No STOP generated
     * ~UCTXSTT -- Do not generate START condition
     * UCSWRST -- Enabled. USCI logic held in reset state
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    UCB0CTL1 = UCSSEL_2 | UCSWRST;

    /* I2C Slave Address Register */
    UCB0I2CSA = DS1307_address;

    /* Bit Rate Control Register 0 */
    UCB0BR0 = 20;

    /* Enable USCI */
    UCB0CTL1 &= ~UCSWRST;

    /* USER CODE START (section: USCI_B0_graceInit_epilogue) */
    /* User code */
    /* USER CODE END (section: USCI_B0_graceInit_epilogue) */
}


void System_graceInit(void)
{
    /* USER CODE START (section: System_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: System_graceInit_prologue) */

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


void show_value(unsigned char x_pos, unsigned char y_pos, unsigned char value)
{
   char chr = 0;

   chr = ((value / 10) + 0x30);
   LCD_goto(x_pos, y_pos);
   LCD_putchar(chr);

   chr = ((value % 10) + 0x30);
   LCD_goto((x_pos + 1), y_pos);
   LCD_putchar(chr);
}


void display_time(void)
{
    LCD_goto(6, 1);
    LCD_putchar(' ');
    LCD_goto(9, 1);
    LCD_putchar(' ');
    delay_ms(450);

    show_value(7, 1, rtc.hr);
    show_value(10, 1, rtc.min);
    show_value(4, 1, rtc.sec);

    LCD_goto(6, 1);
    LCD_putchar(':');
    LCD_goto(9, 1);
    LCD_putchar(':');
    delay_ms(450);
}
