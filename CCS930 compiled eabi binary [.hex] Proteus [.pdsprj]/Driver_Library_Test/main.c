#include "driverlib.h"
#include "delay.h"


void main(void)
{
    unsigned char s = 0x00;

    WDT_A_hold(WDT_A_BASE);

    GPIO_setAsOutputPin (GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setDriveStrength(GPIO_PORT_P1, GPIO_PIN0, GPIO_FULL_OUTPUT_DRIVE_STRENGTH);
    GPIO_setAsOutputPin (GPIO_PORT_P4, GPIO_PIN7);
    GPIO_setDriveStrength(GPIO_PORT_P4, GPIO_PIN7, GPIO_FULL_OUTPUT_DRIVE_STRENGTH);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN1);

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, (GPIO_PIN2 | GPIO_PIN4));
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, (GPIO_PIN2 | GPIO_PIN4));
    UCS_setExternalClockSource(32768, 4000000);
    UCS_turnOnXT2(UCS_XT2_DRIVE_4MHZ_8MHZ);
    UCS_turnOnLFXT1(UCS_XT1_DRIVE_0, UCS_XCAP_3);
    UCS_initClockSignal(UCS_MCLK, UCS_XT2CLK_SELECT, UCS_CLOCK_DIVIDER_1);
    UCS_initClockSignal(UCS_SMCLK, UCS_XT2CLK_SELECT, UCS_CLOCK_DIVIDER_1);
    UCS_initClockSignal(UCS_ACLK, UCS_XT1CLK_SELECT, UCS_CLOCK_DIVIDER_1);

    while(1)
    {
        if(GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN1) == false)
        {
            while(GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN1) == false);
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
            delay_ms(100);
            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
            s = ~s;
        }

        P4OUT ^= BIT7;

        switch(s)
        {
            case 0:
            {
                delay_ms(100);
                break;
            }

            default:
            {
               delay_ms(600);
               break;
            }
        }

    };
}
