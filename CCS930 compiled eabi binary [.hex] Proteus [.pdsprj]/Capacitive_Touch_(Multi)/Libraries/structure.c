/* --COPYRIGHT--,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/*
 * structure.c
 * 
 * RO_CSIO_TA0_RTC_One_Button example for MSP430FR4xx
 * Touch the middle element to turn on/off the center button LED
 * RO method capactiance measurement using Cap sense IO, TimerA0, and RTC
 *
 *
 * Revised by Eric Fu on May 12th
 */

#include "structure.h"

// Middle Element (P2.5)
const struct Element middle_element = {

              .inputPxselRegister = (unsigned char *)&P2SEL,
              .inputPxsel2Register = (unsigned char *)&P2SEL2,
              .inputBits = BIT5,
              // When using an abstracted function to measure the element
              // the 100*(maxResponse - threshold) < 0xFFFF
              // ie maxResponse - threshold < 655
              .maxResponse = (200 + 655),
              .threshold = 200
};

// Up Element (P2.4)
const struct Element up_element = {

              .inputPxselRegister = (unsigned char *)&P2SEL,
              .inputPxsel2Register = (unsigned char *)&P2SEL2,
              .inputBits = BIT4,
              // When using an abstracted function to measure the element
              // the 100*(maxResponse - threshold) < 0xFFFF
              // ie maxResponse - threshold < 655
              .maxResponse = (200 + 655),
              .threshold = 200
};


// Down Element (P2.3)
const struct Element down_element =
{

    .inputPxselRegister = (unsigned char *)&P2SEL,
    .inputPxsel2Register = (unsigned char *)&P2SEL2,
    .inputBits = BIT3,
    // When using an abstracted function to measure the element
    // the 100*(maxResponse - threshold) < 0xFFFF
    // ie maxResponse - threshold < 655
    .maxResponse = (200 + 655),
    .threshold = 200
};

//*** CAP TOUCH HANDLER *******************************************************/
// This defines the grouping of sensors, the method to measure change in
// capacitance, and the function of the group

//PinOsc Wheel
const struct Sensor multi_buttons =
{
    .halDefinition = RO_PINOSC_TA0_WDTp,
    .numElements = 3,
    .baseOffset = 0,
    // Pointer to elements
    .arrayPtr[0] = &up_element,           // point to up element
    .arrayPtr[1] = &down_element,         // point to down element
    .arrayPtr[2] = &middle_element,       // point to middle element
    // Timer Information
    .measGateSource= GATE_WDT_ACLK,     //  0->SMCLK, 1-> ACLK
    .accumulationCycles= WDTp_GATE_64   //64 - Default
};
