#include <msp430.h> 
#include "delay.h"
#include "SW_I2C.h"
#include "PCF8591.h"
#include "lcd.h"


void GPIO_graceInit(void);
void BCSplus_graceInit(void);
void System_graceInit(void);
void WDTplus_graceInit(void);
void lcd_print(unsigned char x_pos, unsigned char y_pos, unsigned char value);


void main(void)
{
    unsigned char adc0 = 0;
    unsigned char adc1 = 0;
    unsigned char adc2 = 0;
    unsigned char adc3 = 0;

    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    /* initialize Config for the MSP430 GPIO */
    GPIO_graceInit();

    /* initialize Config for the MSP430 2xx family clock systems (BCS) */
    BCSplus_graceInit();

    /* initialize Config for the MSP430 System Registers */
    System_graceInit();

    /* initialize Config for the MSP430 WDT+ */
    WDTplus_graceInit();

    SW_I2C_init();
    LCD_init();

    LCD_goto(0, 0);
    LCD_putstr("A0:");

    LCD_goto(9, 0);
    LCD_putstr("A1:");

    LCD_goto(0, 1);
    LCD_putstr("A2:");

    LCD_goto(9, 1);
    LCD_putstr("A3:");

    while(1)
    {
        adc0 = PCF8591_read(AOut_enable | Four_Channel_ADC | Auto_Increment_Disable | AIN0);
        lcd_print(4, 0, adc0);

        adc1 = PCF8591_read(AOut_enable | Four_Channel_ADC | Auto_Increment_Disable | AIN1);
        lcd_print(13, 0, adc1);

        adc2 = PCF8591_read(AOut_enable | Four_Channel_ADC | Auto_Increment_Disable | AIN2);
        lcd_print(4, 1, adc2);

        adc3 = PCF8591_read(AOut_enable | Four_Channel_ADC | Auto_Increment_Disable | AIN3);
        lcd_print(13, 1, adc3);

        PCF8591_write(AOut_enable, adc0);
        delay_ms(400);
    };
}


void GPIO_graceInit(void)
{
    /* USER CODE START (section: GPIO_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: GPIO_graceInit_prologue) */

    /* Port 1 Output Register */
    P1OUT = 0;

    /* Port 1 Direction Register */
    P1DIR = BIT0 | BIT6 | BIT7;

    /* Port 1 Interrupt Edge Select Register */
    P1IES = 0;

    /* Port 1 Interrupt Flag Register */
    P1IFG = 0;

    /* Port 2 Output Register */
    P2OUT = 0;

    /* Port 2 Port Select Register */
    P2SEL &= ~(BIT6 | BIT7);

    /* Port 2 Direction Register */
    P2DIR = BIT0 | BIT1 | BIT2;

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
     * LFXT1S_0 -- If XTS = 0, XT1 = 32768kHz Crystal ; If XTS = 1, XT1 = 0.4 - 1-MHz crystal or resonator
     * XCAP_1 -- ~6 pF
     */
    BCSCTL3 = XT2S_0 | LFXT1S_0 | XCAP_1;

    /* USER CODE START (section: BCSplus_graceInit_epilogue) */
    /* User code */
    /* USER CODE END (section: BCSplus_graceInit_epilogue) */
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


void lcd_print(unsigned char x_pos, unsigned char y_pos, unsigned char value)
{
   unsigned char ch = 0;

   ch = ((value / 100) + 0x30);
   LCD_goto(x_pos, y_pos);
   LCD_putchar(ch);

   ch = (((value / 10) % 10) + 0x30);
   LCD_goto((x_pos + 1), y_pos);
   LCD_putchar(ch);

   ch = ((value % 10) + 0x30);
   LCD_goto((x_pos + 2), y_pos);
   LCD_putchar(ch);
}
