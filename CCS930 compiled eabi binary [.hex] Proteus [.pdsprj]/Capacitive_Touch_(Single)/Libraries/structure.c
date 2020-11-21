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
const struct Element sense_element =
{
    .inputPxselRegister = (uint8_t *)&P2SEL,
    .inputPxsel2Register = (uint8_t *)&P2SEL2,
    .inputBits = BIT4,
    // When using an abstracted function to measure the element
    // the 100*(maxResponse - threshold) < 0xFFFF
    // ie maxResponse - threshold < 655
    .maxResponse = (450 + 655),
    .threshold = 450
};

// One Button Sensor
const struct Sensor one_button =
{
    .halDefinition = RO_PINOSC_TA0_WDTp,  // Sensing Method
    .numElements = 1,                     // # of Elements
    .baseOffset = 0,                      // First element index = 0
    // Pointer to elements
    .arrayPtr[0] = &sense_element,       // point to middle element
    // Timer Information
    .measGateSource= GATE_WDT_ACLK,     //  0->SMCLK, 1-> ACLK
    .accumulationCycles= WDTp_GATE_64   //64 - Default
};
