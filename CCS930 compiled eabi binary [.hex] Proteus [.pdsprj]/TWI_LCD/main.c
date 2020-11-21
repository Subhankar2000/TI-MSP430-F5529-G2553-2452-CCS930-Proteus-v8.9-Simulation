#include <msp430.h>
#include "delay.h"
#include "I2C.h"
#include "PCF8574.h"
#include "lcd.h"


void GPIO_graceInit(void);
void BCSplus_graceInit(void);
void USI_graceInit(void);
void System_graceInit(void);
void WDTplus_graceInit(void);
void show_value(unsigned char value);


void main(void)
{
    unsigned char s = 0;

    char txt1[] = {"MICROARENA"};
    char txt2[] = {"SShahryiar"};
    char txt3[] = {"MSP-EXP430G2"};
    char txt4[] = {"Launchpad!"};

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

    LCD_init();
    LCD_clear_home();

    LCD_goto(3, 0);
    LCD_putstr(txt1);
    LCD_goto(3, 1);
    LCD_putstr(txt2);
    delay_ms(2600);

    LCD_clear_home();

    for(s = 0; s < 12; s++)
    {
        LCD_goto((2 + s), 0);
        LCD_putchar(txt3[s]);
        delay_ms(60);
    }
    for(s = 0; s < 10; s++)
    {
        LCD_goto((3 + s), 1);
        LCD_putchar(txt4[s]);
        delay_ms(60);
    }

    delay_ms(2600);

    s = 0;
    LCD_clear_home();

    LCD_goto(3, 0);
    LCD_putstr(txt1);

    while(1)
    {
        show_value(s);
        s++;
        delay_ms(200);
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
     * USIPE7 -- USI function enabled
     * USIPE6 -- USI function enabled
     * ~USIPE5 -- USI function disabled
     * ~USILSB -- MSB first
     * USIMST -- Master mode
     * ~USIGE -- Output latch enable depends on shift clock
     * ~USIOE -- Output disabled
     * USISWRST -- USI logic held in reset state
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    USICTL0 = USIPE7 | USIPE6 | USIMST | USISWRST;

    /*
     * USI Control Register 1
     *
     * ~USICKPH -- Data is changed on the first SCLK edge and captured on the following edge
     * USII2C -- I2C mode enabled
     * ~USISTTIE -- Interrupt on START condition disabled
     * USIIE -- Interrupt enabled
     * ~USIAL -- No arbitration lost condition
     * ~USISTP -- No STOP condition received
     * ~USISTTIFG -- No START condition received. No interrupt pending
     * USIIFG -- Interrupt pending
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    USICTL1 = USII2C | USIIE | USIIFG;

    /*
     * USI Clock Control Register
     *
     * USIDIV_4 -- Divide by 16
     * USISSEL_2 -- SMCLK
     * USICKPL -- Inactive state is high
     * ~USISWCLK -- Input clock is low
     *
     * Note: ~USISWCLK indicates that USISWCLK has value zero
     */
    USICKCTL = USIDIV_7 | USISSEL_2 | USICKPL;

    /*
     * USI Bit Counter Register
     *
     * ~USISCLREL -- SCL line is held low if USIIFG is set
     * ~USI16B -- 8-bit shift register mode. Low byte register USISRL is used
     * USIIFGCC -- USIIFG is not cleared automatically
     * ~USICNT4 -- USI bit count
     * ~USICNT3 -- USI bit count
     * ~USICNT2 -- USI bit count
     * ~USICNT1 -- USI bit count
     * ~USICNT0 -- USI bit count
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    USICNT = USIIFGCC;

    /* Enable USI */
    USICTL0 &= ~USISWRST;

    /* Clear pending flag */
    USICTL1 &= ~(USIIFG + USISTTIFG);

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


void show_value(unsigned char value)
{
   unsigned char ch = 0x00;

   ch = ((value / 100) + 0x30);
   LCD_goto(6, 1);
   LCD_putchar(ch);

   ch = (((value / 10) % 10) + 0x30);
   LCD_goto(7, 1);
   LCD_putchar(ch);

   ch = ((value % 10) + 0x30);
   LCD_goto(8, 1);
   LCD_putchar(ch);
}
