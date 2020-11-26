/*
 *  This file is automatically generated and does not require a license
 *
 *  ==== WARNING: CHANGES TO THIS GENERATED FILE WILL BE OVERWRITTEN ====
 *
 *  To make changes to the generated code, use the space between existing
 *      "USER CODE START (section: <name>)"
 *  and
 *      "USER CODE END (section: <name>)"
 *  comments, where <name> is a single word identifying the section.
 *  Only these sections will be preserved.
 *
 *  Do not move these sections within this file or change the START and
 *  END comments in any way.
 *  ==== ALL OTHER CHANGES WILL BE OVERWRITTEN WHEN IT IS REGENERATED ====
 *
 *  This file was generated from
 *      C:/ti/grace/grace_2_20_02_32/packages/ti/mcu/msp430/csl/timer/ITimerx_A_init.xdt
 */

#include <msp430.h>

/* USER CODE START (section: Timer0_A3_init_c_prologue) */
/* User defined includes, defines, global variables and functions */
/* USER CODE END (section: Timer0_A3_init_c_prologue) */

/*
 *  ======== Timer0_A3_graceInit ========
 *  Initialize MSP430 Timer0_A3 timer
 */
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

    /* 
     * TA0CCTL1, Capture/Compare Control Register 1
     * 
     * CM_0 -- No Capture
     * CCIS_0 -- CCIxA
     * ~SCS -- Asynchronous Capture
     * ~SCCI -- Latched capture signal (read)
     * ~CAP -- Compare mode
     * OUTMOD_3 -- PWM output mode: 3 - PWM set/reset
     * 
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    TA0CCTL1 = CM_0 | CCIS_0 | OUTMOD_3;

    /* 
     * TA0CCTL2, Capture/Compare Control Register 2
     * 
     * CM_0 -- No Capture
     * CCIS_0 -- CCIxA
     * ~SCS -- Asynchronous Capture
     * ~SCCI -- Latched capture signal (read)
     * ~CAP -- Compare mode
     * OUTMOD_7 -- PWM output mode: 7 - PWM reset/set
     * 
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    TA0CCTL2 = CM_0 | CCIS_0 | OUTMOD_7;

    /* TA0CCR0, Timer_A Capture/Compare Register 0 */
    TA0CCR0 = 999;

    /* TA0CCR1, Timer_A Capture/Compare Register 1 */
    TA0CCR1 = 10;

    /* TA0CCR2, Timer_A Capture/Compare Register 2 */
    TA0CCR2 = 10;

    /* 
     * TA0CTL, Timer_A3 Control Register
     * 
     * TASSEL_2 -- SMCLK
     * ID_0 -- Divider - /1
     * MC_1 -- Up Mode
     */
    TA0CTL = TASSEL_2 | ID_0 | MC_1;

    /* USER CODE START (section: Timer0_A3_graceInit_epilogue) */
    /* User code */
    /* USER CODE END (section: Timer0_A3_graceInit_epilogue) */
}