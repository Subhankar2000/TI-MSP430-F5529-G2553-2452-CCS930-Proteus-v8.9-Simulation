#include <msp430.h> 
#include "delay.h"
#include "lcd.h"


#define T_offset        -18


unsigned int ADC_Value = 0;


void GPIO_graceInit(void);
void BCSplus_graceInit(void);
void ADC10_graceInit(void);
void System_graceInit(void);
void WDTplus_graceInit(void);
unsigned int get_volt(unsigned int value);
unsigned int get_temp(unsigned int value);
void lcd_print(unsigned char x_pos, unsigned char y_pos, unsigned int value);


#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR_HOOK(void)
{
    P1OUT ^= BIT0;
    ADC_Value = ADC10MEM;
    ADC10CTL0 &= ~ADC10IFG;
}


void main(void)
{
    signed int t = 0;

    /* Stop watchdog timer from timing out during initial start-up. */
    WDTCTL = WDTPW | WDTHOLD;

    /* initialize Config for the MSP430 GPIO */
    GPIO_graceInit();

    /* initialize Config for the MSP430 2xx family clock systems (BCS) */
    BCSplus_graceInit();

    /* initialize Config for the MSP430 10-bit Analog to Digital Converter (ADC) */
    ADC10_graceInit();

    /* initialize Config for the MSP430 System Registers */
    System_graceInit();

    /* initialize Config for the MSP430 WDT+ */
    WDTplus_graceInit();

    LCD_init();

    LCD_clear_home();

    LCD_goto(0, 0);
    LCD_putstr("ADC Value:");
    LCD_goto(0, 1);
    LCD_putstr("Tmp/Deg.C:");

    while(1)
    {
        // ADC Start Conversion - Software trigger
        ADC10CTL0 |= ADC10SC;

        P1OUT ^= BIT6;

        t = get_volt(ADC_Value);
        t = get_temp(t);

        lcd_print(12, 0, ADC_Value);
        lcd_print(12, 1, t);
        delay_ms(200);
    };
}


void GPIO_graceInit(void)
{
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
    P2DIR = BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5;

    /* Port 2 Interrupt Edge Select Register */
    P2IES = 0;

    /* Port 2 Interrupt Flag Register */
    P2IFG = 0;

    /* Port 3 Output Register */
    P3OUT = 0;

    /* Port 3 Direction Register */
    P3DIR = 0;
}


void BCSplus_graceInit(void)
{
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

    if (CALBC1_12MHZ != 0xFF)
    {
        /* Adjust this accordingly to your VCC rise time */
        __delay_cycles(1000);

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
}


void ADC10_graceInit(void)
{
    /* disable ADC10 during initialization */
    ADC10CTL0 &= ~ENC;

    /*
     * Control Register 0
     *
     * ~ADC10SC -- No conversion
     * ~ENC -- Disable ADC
     * ~ADC10IFG -- Clear ADC interrupt flag
     * ADC10IE -- Enable ADC interrupt
     * ADC10ON -- Switch On ADC10
     * ~REFON -- Disable ADC reference generator
     * ~REF2_5V -- Set reference voltage generator = 1.5V
     * MSC -- Enable multiple sample and conversion
     * ~REFBURST -- Reference buffer on continuously
     * ~REFOUT -- Reference output off
     * ADC10SR -- Reference buffer supports up to ~50 ksps
     * ADC10SHT_3 -- 64 x ADC10CLKs
     * SREF_0 -- VR+ = VCC and VR- = VSS
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    ADC10CTL0 = ADC10IE | ADC10ON | MSC | ADC10SR | ADC10SHT_3 | SREF_0;

    /*
     * Control Register 1
     *
     * ~ADC10BUSY -- No operation is active
     * CONSEQ_2 -- Repeat single channel
     * ADC10SSEL_3 -- SMCLK
     * ADC10DIV_7 -- Divide by 8
     * ~ISSH -- Input signal not inverted
     * ~ADC10DF -- ADC10 Data Format as binary
     * SHS_0 -- ADC10SC
     * INCH_10 -- Temperature Sensor
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    ADC10CTL1 = CONSEQ_2 | ADC10SSEL_3 | ADC10DIV_7 | SHS_0 | INCH_10;


    /* enable ADC10 */
    ADC10CTL0 |= ENC;
}


void System_graceInit(void)
{
    /* Clear oscillator fault flag with software delay */
    do
    {
        // Clear OSC fault flag
        IFG1 &= ~OFIFG;

        // 50us delay
        __delay_cycles(600);
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
}


void WDTplus_graceInit(void)
{
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
}


unsigned int get_volt(unsigned int value)
{
    return (unsigned int)((value * 3600.0) / 1023.0);
}


unsigned int get_temp(unsigned int value)
{
    return (unsigned int)((((value / 1000.0) - 0.986) / 0.00355) + T_offset);
}


void lcd_print(unsigned char x_pos, unsigned char y_pos, unsigned int value)
{
    char chr = 0x00;

    chr = ((value / 1000) + 0x30);
    LCD_goto(x_pos, y_pos);
    LCD_putchar(chr);

    chr = (((value / 100) % 10) + 0x30);
    LCD_goto((x_pos + 1), y_pos);
    LCD_putchar(chr);

    chr = (((value / 10) % 10) + 0x30);
    LCD_goto((x_pos + 2), y_pos);
    LCD_putchar(chr);

    chr = ((value % 10) + 0x30);
    LCD_goto((x_pos + 3), y_pos);
    LCD_putchar(chr);
}
