#include <msp430.h> 
#include "delay.h"
#include "SW_I2C.h"
#include "PCF8574.h"
#include "lcd.h"


void Flash_graceInit(void);
void GPIO_graceInit(void);
void BCSplus_graceInit(void);
void System_graceInit(void);
void WDTplus_graceInit(void);
void lcd_print(unsigned char x_pos, unsigned char y_pos, unsigned char value);
void Flash_Erase(unsigned int address);
void Flash_Write_Char(unsigned int address, char value);
char Flash_Read_Char(unsigned int address);
void Flash_Write_Word(unsigned int address, unsigned int value);
unsigned int Flash_Read_Word(unsigned int address);


void main(void)
{
    unsigned char value = 0x00;

    /* Stop watchdog timer from timing out during initial start-up. */
    WDTCTL = WDTPW | WDTHOLD;

    /* initialize Config for the MSP430F2xx Flash Memory Controller */
    Flash_graceInit();

    /* initialize Config for the MSP430 GPIO */
    GPIO_graceInit();

    /* initialize Config for the MSP430 2xx family clock systems (BCS) */
    BCSplus_graceInit();

    /* initialize Config for the MSP430 System Registers */
    System_graceInit();

    /* initialize Config for the MSP430 WDT+ */
    WDTplus_graceInit();

    LCD_init();
    LCD_clear_home();

    LCD_goto(0, 0);
    LCD_putstr("MSP430 Flash Ex.");

    value = Flash_Read_Char(0x1000);

    LCD_goto(0, 1);
    LCD_putstr("WR: ---");
    LCD_goto(9, 1);
    LCD_putstr("RD:");
    lcd_print(13, 1, value);
    delay_ms(2000);

	while(1)
	{
	    if((P1IN & BIT3) == !BIT3)
        {
            while((P1IN & BIT3) == !BIT3);
            Flash_Erase(0x1000);
            Flash_Write_Char(0x1000, value);
            lcd_print(13, 1, value);
	        P1OUT |= BIT0;
            _delay_cycles(40000);
            P1OUT &= ~BIT0;
        }

	    delay_ms(20);
        lcd_print(4, 1, value);

        value++;
        delay_ms(200);
	};
}


void Flash_graceInit(void)
{
    /* USER CODE START (section: Flash_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: Flash_graceInit_prologue) */

    /*
     * Flash Memory Control Register 2
     *
     * FSSEL_1 -- MCLK
     * ~FN5 -- Flash controller clock divider bit 5
     * FN4 -- Flash controller clock divider bit 4
     * ~FN3 -- Flash controller clock divider bit 3
     * FN2 -- Flash controller clock divider bit 2
     * ~FN1 -- Flash controller clock divider bit 1
     * FN0 -- Flash controller clock divider bit 0
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    FCTL2 = FWKEY | FSSEL_1 | FN4 | FN2 | FN0;

    /* USER CODE START (section: Flash_graceInit_epilogue) */
    /* User code */
    /* USER CODE END (section: Flash_graceInit_epilogue) */
}


void GPIO_graceInit(void)
{
    /* USER CODE START (section: GPIO_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: GPIO_graceInit_prologue) */

    /* Port 1 Output Register */
    P1OUT = BIT3;

    /* Port 1 Direction Register */
    P1DIR = BIT0;

    /* Port 1 Resistor Enable Register */
    P1REN = BIT3;

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

    if (CALBC1_8MHZ != 0xFF) {
        /* Adjust this accordingly to your VCC rise time */
        __delay_cycles(100000);

        // Follow recommended flow. First, clear all DCOx and MODx bits. Then
        // apply new RSELx values. Finally, apply new DCOx and MODx bit values.
        DCOCTL = 0x00;
        BCSCTL1 = CALBC1_8MHZ;      /* Set DCO to 8MHz */
        DCOCTL = CALDCO_8MHZ;
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
    char chr = 0x00;

    chr = ((value / 100) + 0x30);
    LCD_goto(x_pos, y_pos);
    LCD_putchar(chr);

    chr = (((value / 10) % 10) + 0x30);
    LCD_goto((x_pos + 1), y_pos);
    LCD_putchar(chr);

    chr = ((value % 10) + 0x30);
    LCD_goto((x_pos + 2), y_pos);
    LCD_putchar(chr);
}


void Flash_Erase(unsigned int address)
{
    char *FlashPtr;

    FlashPtr = (char *)address;
    FCTL1 = FWKEY + ERASE;                      // Set Erase bit
    FCTL3 = FWKEY;                              // Clear Lock bit
    __bic_SR_register(GIE);                     // Disable Interrupts
    *FlashPtr = 0;                              // Dummy write to erase Flash segment B
    while((FCTL3 & BUSY) == BUSY);              // Busy
    __bis_SR_register(GIE);                     // Enable Interrupts
    FCTL1 = FWKEY;                              // Lock
    FCTL3 = FWKEY + LOCK;                       // Set Lock bit
}


void Flash_Write_Char(unsigned int address, char value)
{
    char *FlashPtr = (char *)address;

    FCTL1 = FWKEY + WRT;                        // Set WRT bit for write operation
    FCTL3 = FWKEY;                              // Clear Lock bit
    __bic_SR_register(GIE);                     // Disable Interrupts
    *FlashPtr = value;                          // Save Data
    while((FCTL3 & BUSY) == BUSY);              // Busy
    __bis_SR_register(GIE);                     // Enable Interrupts
    FCTL1 = FWKEY;                              // Clear WRT bit
    FCTL3 = FWKEY + LOCK;                       // Set LOCK bit
}


char Flash_Read_Char(unsigned int address)
{
    char value = 0x00;
    char *FlashPtr = (char *)address;

    value = *FlashPtr;

    return value;
}


void Flash_Write_Word(unsigned int address, unsigned int value)
{
    unsigned int *FlashPtr = (unsigned int *)address;

    FCTL1 = FWKEY + WRT;                        // Set WRT bit for write operation
    FCTL3 = FWKEY;                              // Clear Lock bit
    __bic_SR_register(GIE);                     // Disable Interrupts
    *FlashPtr = value;                          // Save Data
    while((FCTL3 & BUSY) == BUSY);              // Busy
    __bis_SR_register(GIE);                     // Enable Interrupts
    FCTL1 = FWKEY;                              // Clear WRT bit
    FCTL3 = FWKEY + LOCK;                       // Set LOCK bit
}


unsigned int Flash_Read_Word(unsigned int address)
{
    unsigned int value = 0x0000;
    unsigned int *FlashPtr = (unsigned int *)address;

    value = *FlashPtr;

    return value;
}
