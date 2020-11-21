#include <msp430.h> 
#include "delay.h"
#include "lcd.h"


#define T_offset        -18


void BCSplus_graceInit(void);
void GPIO_graceInit(void);
void ADC10_graceInit(void);
void System_graceInit(void);
void WDTplus_graceInit(void);
unsigned int get_ADC(unsigned int channel);
unsigned int get_volt(unsigned int value);
unsigned int get_temp(unsigned int value);
void lcd_print(unsigned char x_pos, unsigned char y_pos, unsigned int value);


void main(void)
{
    unsigned char n = 0;

    unsigned int res = 0;
    unsigned int ADC_Value = 0;

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

    while(1)
    {
        if((P1IN & BIT3) == 0)
        {
            P1OUT |= BIT6;
            while((P1IN & BIT3) == 0);

            n++;
            if(n > 2)
            {
                n = 0;
            }
            P1OUT &= ~BIT6;
        }

        switch(n)
        {
            case 1:
            {
                ADC_Value = get_ADC(INCH_1);
                res = get_volt(ADC_Value);

                LCD_goto(0, 0);
                LCD_putstr("ADC Ch01:");
                LCD_goto(0, 1);
                LCD_putstr("Volts/mV:");
                break;
            }
            case 2:
            {
                ADC_Value = get_ADC(INCH_2);
                res = get_volt(ADC_Value);

                LCD_goto(0, 0);
                LCD_putstr("ADC Ch02:");
                LCD_goto(0, 1);
                LCD_putstr("Volts/mV:");
                break;
            }
            default:
            {
                ADC_Value = get_ADC(INCH_10);
                res = get_temp(get_volt(ADC_Value));

                LCD_goto(0, 0);
                LCD_putstr("ADC Ch10:");
                LCD_goto(0, 1);
                LCD_putstr("TC/Deg.C:");
                break;
            }
        }

        lcd_print(12, 0, ADC_Value);
        lcd_print(12, 1, res);
        delay_ms(200);
    };
}


void GPIO_graceInit(void)
{
    /* USER CODE START (section: GPIO_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: GPIO_graceInit_prologue) */

    /* Port 1 Output Register */
    P1OUT = BIT3;

    /* Port 1 Direction Register */
    P1DIR = BIT0 | BIT6;

    /* Port 1 Resistor Enable Register */
    P1REN = BIT3;

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


void ADC10_graceInit(void)
{
    /* USER CODE START (section: ADC10_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: ADC10_graceInit_prologue) */

    /* disable ADC10 during initialization */
    ADC10CTL0 &= ~ENC;

    /*
     * Control Register 0
     *
     * ~ADC10SC -- No conversion
     * ~ENC -- Disable ADC
     * ~ADC10IFG -- Clear ADC interrupt flag
     * ~ADC10IE -- Disable ADC interrupt
     * ADC10ON -- Switch On ADC10
     * ~REFON -- Disable ADC reference generator
     * ~REF2_5V -- Set reference voltage generator = 1.5V
     * ~MSC -- Disable multiple sample and conversion
     * ~REFBURST -- Reference buffer on continuously
     * ~REFOUT -- Reference output off
     * ~ADC10SR -- Reference buffer supports up to ~200 ksps
     * ADC10SHT_3 -- 64 x ADC10CLKs
     * SREF_0 -- VR+ = VCC and VR- = VSS
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    ADC10CTL0 = ADC10ON | ADC10SHT_3 | SREF_0;

    /*
     * Control Register 1
     *
     * ~ADC10BUSY -- No operation is active
     * CONSEQ_2 -- Repeat single channel
     * ADC10SSEL_3 -- SMCLK
     * ADC10DIV_3 -- Divide by 4
     * ~ISSH -- Input signal not inverted
     * ~ADC10DF -- ADC10 Data Format as binary
     * SHS_0 -- ADC10SC
     * INCH_10 -- Temperature Sensor
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    ADC10CTL1 = CONSEQ_2 | ADC10SSEL_3 | ADC10DIV_3 | SHS_0;

    /* Analog (Input) Enable Control Register 0 */
    ADC10AE0 = 0x6;


    /* enable ADC10 */
    ADC10CTL0 |= ENC;

    /* USER CODE START (section: ADC10_graceInit_epilogue) */
    /* User code */
    /* USER CODE END (section: ADC10_graceInit_epilogue) */
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


unsigned int get_ADC(unsigned int channel)
{
    P1OUT ^= BIT0;

    ADC10CTL0 &= ~ENC;

    ADC10CTL1 &= ~(0xF000);
    ADC10CTL1 |= channel;

    ADC10CTL0 |= ENC;

    ADC10CTL0 |= ADC10SC;
    while ((ADC10CTL0 & ADC10IFG) == 0);

    return ADC10MEM;
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
