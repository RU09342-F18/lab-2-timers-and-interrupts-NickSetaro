#include <msp432.h>

/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2013, Texas Instruments Incorporated
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
 *
 *******************************************************************************
 *
 *                       MSP432 CODE EXAMPLE DISCLAIMER
 *
 * MSP432 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see http://www.ti.com/tool/mspdriverlib for an API functional
 * library & https://dev.ti.com/pinmux/ for a GUI approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP432P401 Demo - Software Port Interrupt Service on P1.1 from LPM4
//
//  Description: MSP432 device is configured to enter LPM4 mode with GPIOs
//  properly terminated. P1.1 is configured as an input. Pressing the button
//  connect to P1.1 results in device waking up and servicing the Port1 ISR.
//  LPM3 current can be measured when P1.0 is output low (e.g. LED off).
//
//  ACLK = 32kHz, MCLK = SMCLK = default DCO
//
//
//               MSP432P401x
//            -----------------
//        /|\|                 |
//         | |                 |
//         --|RST              |
//     /|\   |                 |
//      --o--|P1.1         P1.0|-->LED
//     \|/
//
//   Dung Dang
//   Texas Instruments Inc.
//   Oct 2016 (updated) | November 2013 (created)
//   Built with CCSv6.1, IAR, Keil, GCC
//******************************************************************************

int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     //STOP WATCHDOG TIMER


    P1->DIR = ~(uint8_t) BIT1;
    P1->OUT = BIT1;                         //set resistor to pull up
    P1->REN = BIT1;                         // Enable pull-up resistor (P1.1 output high)
    P1->SEL0 = 0;                           //select button
    P1->SEL1 = 0;
    P1->IES = BIT1;                         // set edge to high
    P1->IFG = 0;                            // Clear all P1 interrupt flags
    P1->IE = BIT1;                          // Enable interrupt for P1.1

    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);   //enable port 1 interrupt

    PJ->DIR |= (BIT0| BIT1 | BIT2 | BIT3);      //set port J to output
    PJ->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3);    //set port J low


    PCM->CTL1 = PCM_CTL0_KEY_VAL | PCM_CTL1_FORCE_LPM_ENTRY;        //enable pcm rude mode


    __enable_irq();     //enable interrupts


    SCB->SCR |= (SCB_SCR_SLEEPDEEP_Msk);            // Setting the sleep deep bit


    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;        // Do not wake up on exit from ISR


    __DSB();        // Ensures SLEEPONEXIT takes effect immediately


    __sleep();      // Go to LPM4
}

//interrupt routine
void PORT1_IRQHandler(void)
{
    volatile uint32_t i;

    if(P1->IFG & BIT1)
        P1->OUT ^= BIT0;        //toggle LED
        P1->IES ^= BIT1;            //toggle edge
    for(i = 0; i < 10000; i++)      //delay for debounce

    P1->IFG &= ~BIT1;       //clear interrupt flags
}
