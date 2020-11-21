#include <msp430.h> 
#include "delay.h"
#include "lcd.h"


unsigned int overflow_count = 0;
unsigned int pulse_ticks = 0;
unsigned int start_time = 0;
unsigned int end_time = 0;


void GPIO_graceInit(void);
void BCSplus_graceInit(void);
void Timer0_A3_graceInit(void);
void Timer1_A3_graceInit(void);
void System_graceInit(void);
void WDTplus_graceInit(void);
void lcd_print(unsigned char x_pos, unsigned char y_pos, unsigned long value);


#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR_HOOK(void)
{
    if(TA1IV == TA1IV_TACCR1)
    {
        end_time = TA1CCR1;
        pulse_ticks = (end_time - start_time);
        start_time = end_time;
        TA1CCTL1 &= ~CCIFG;
    }
}


void main(void)
{
    unsigned char i = 0;
    unsigned long time_period = 0;

    /* Stop watchdog timer from timing out during initial start-up. */
    WDTCTL = WDTPW | WDTHOLD;

    /* initialize Config for the MSP430 GPIO */
    GPIO_graceInit();

    /* initialize Config for the MSP430 2xx family clock systems (BCS) */
    BCSplus_graceInit();

    /* initialize Config for the MSP430 A3 Timer0 */
    Timer0_A3_graceInit();

    /* initialize Config for the MSP430 A3 Timer0 */
    Timer1_A3_graceInit();

    /* initialize Config for the MSP430 System Registers */
    System_graceInit();

    /* initialize Config for the MSP430 WDT+ */
    WDTplus_graceInit();

    LCD_init();
    LCD_clear_home();

    LCD_goto(0, 0);
    LCD_putstr("Capt./us:");
    delay_ms(10);

    while(1)
    {
        if((P1IN & BIT3) == 0)
        {
            P1OUT |= BIT0;
            while((P1IN & BIT3) == 0);

            i++;
            if(i > 9)
            {
                i = 0;
            }
            P1OUT &= ~BIT0;
        }

        switch(i)
        {
            case 1:
            {
                TA0CCR0 = 9999;
                LCD_goto(0, 1);
                LCD_putstr("Period/ms:    20");
                break;
            }
            case 2:
            {
                TA0CCR0 = 4999;
                LCD_goto(0, 1);
                LCD_putstr("Period/ms:    10");
                break;
            }
            case 3:
            {
                TA0CCR0 = 1999;
                LCD_goto(0, 1);
                LCD_putstr("Period/ms:     4");
                break;
            }
            case 4:
            {
                TA0CCR0 = 999;
                LCD_goto(0, 1);
                LCD_putstr("Period/ms:     2");
                break;
            }
            case 5:
            {
                TA0CCR0 = 166;
                LCD_goto(0, 1);
                LCD_putstr("Period/us:   334");
                break;
            }
            case 6:
            {
                TA0CCR0 = 1230;
                LCD_goto(0, 1);
                LCD_putstr("Period/ms:   2.5");
                break;
            }
            case 7:
            {
                TA0CCR0 = 2626;
                LCD_goto(0, 1);
                LCD_putstr("Period/ms:   5.3");
                break;
            }
            case 8:
            {
                TA0CCR0 = 4579;
                LCD_goto(0, 1);
                LCD_putstr("Period/ms:   9.2");
                break;
            }
            case 9:
            {
                TA0CCR0 = 499;
                LCD_goto(0, 1);
                LCD_putstr("Period/ms:     1");
                break;
            }
            default:
            {
                TA0CCR0 = 6964;
                LCD_goto(0, 1);
                LCD_putstr("Period/ms:  13.9");
                break;
            }
        }

        time_period = (pulse_ticks >> 1);
        lcd_print(10, 0, time_period);
        delay_ms(400);
    };
}


void GPIO_graceInit(void)
{
    /* USER CODE START (section: GPIO_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: GPIO_graceInit_prologue) */

    /* Port 1 Output Register */
    P1OUT = BIT3;

    /* Port 1 Port Select Register */
    P1SEL = BIT1;

    /* Port 1 Direction Register */
    P1DIR = BIT0 | BIT1 | BIT6 | BIT7;

    /* Port 1 Resistor Enable Register */
    P1REN = BIT3;

    /* Port 1 Interrupt Edge Select Register */
    P1IES = 0;

    /* Port 1 Interrupt Flag Register */
    P1IFG = 0;

    /* Port 2 Output Register */
    P2OUT = 0;

    /* Port 2 Port Select Register */
    P2SEL = BIT1;

    /* Port 2 Port Select Register */
    P2SEL &= ~(BIT6 | BIT7);

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


void Timer0_A3_graceInit(void)
{
    /* USER CODE START (section: Timer0_A3_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: Timer0_A3_graceInit_prologue) */

    /*
     * TA0CCTL0, Capture/Compare Control Register 0
     *
     * CM_0 -- No Capture
     * CCIS_0 -- CCIxA
     * ~SCS -- Asynchronous Capture
     * ~SCCI -- Latched capture signal (read)
     * ~CAP -- Compare mode
     * OUTMOD_4 -- PWM output mode: 4 - Toggle
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    TA0CCTL0 = CM_0 | CCIS_0 | OUTMOD_4;

    /* TA0CCR0, Timer_A Capture/Compare Register 0 */
    TA0CCR0 = 9999;

    /*
     * TA0CTL, Timer_A3 Control Register
     *
     * TASSEL_2 -- SMCLK
     * ID_1 -- Divider - /2
     * MC_1 -- Up Mode
     */
    TA0CTL = TASSEL_2 | ID_1 | MC_1;

    /* USER CODE START (section: Timer0_A3_graceInit_epilogue) */
    /* User code */
    /* USER CODE END (section: Timer0_A3_graceInit_epilogue) */
}


void Timer1_A3_graceInit(void)
{
    /* USER CODE START (section: Timer1_A3_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: Timer1_A3_graceInit_prologue) */

    /*
     * TA1CCTL0, Capture/Compare Control Register 0
     *
     * CM_1 -- Rising Edge
     * CCIS_0 -- CCIxA
     * ~SCS -- Asynchronous Capture
     * ~SCCI -- Latched capture signal (read)
     * ~CAP -- Compare mode
     * OUTMOD_1 -- PWM output mode: 1 - Set
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    TA1CCTL0 = CM_1 | CCIS_0 | OUTMOD_1;

    /*
     * TA1CCTL1, Capture/Compare Control Register 1
     *
     * CM_1 -- Rising Edge
     * CCIS_0 -- CCIxA
     * SCS -- Sychronous Capture
     * ~SCCI -- Latched capture signal (read)
     * CAP -- Capture mode
     * OUTMOD_0 -- PWM output mode: 0 - OUT bit value
     *
     * Note: ~SCCI indicates that SCCI has value zero
     */
    TA1CCTL1 = CM_1 | CCIS_0 | SCS | CAP | OUTMOD_0 | CCIE;

    /*
     * TA1CTL, Timer_A3 Control Register
     *
     * TASSEL_2 -- SMCLK
     * ID_0 -- Divider - /1
     * MC_2 -- Continuous Mode
     */
    TA1CTL = TASSEL_2 | ID_0 | MC_2;

    /* USER CODE START (section: Timer1_A3_graceInit_epilogue) */
    /* User code */
    /* USER CODE END (section: Timer1_A3_graceInit_epilogue) */
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


void lcd_print(unsigned char x_pos, unsigned char y_pos, unsigned long value)
{
    char tmp[6] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20} ;

    tmp[0] = (((value / 100000) % 10) + 0x30);
    tmp[1] = (((value / 10000) % 10) + 0x30);
    tmp[2] = (((value / 1000) % 10) + 0x30);
    tmp[3] = (((value / 100) % 10) + 0x30);
    tmp[4] = (((value / 10) % 10) + 0x30);
    tmp[5] = ((value % 10) + 0x30);

    LCD_goto(x_pos, y_pos);
    LCD_putstr(tmp);
}
