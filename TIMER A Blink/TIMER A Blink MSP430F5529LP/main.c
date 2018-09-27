#include <msp430.h>
#include <msp430F5529.h>

/*
 * @author Nick Setaro
 * @version 9.27.2018
 *
 * This code uses the timer connected to ACLK to blink two different LEDs at different rates.
 * An interrupt is used when the timer overflows to toggle the LEDs.
 */

volatile unsigned int timerCount = 0;
volatile unsigned int timerCount1 = 0;

int main(void)
{


    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P1OUT |= (BIT0);            //sets the LED at p1.0 to on
    P4OUT &= ~BIT7;             //sets the LED at p4.7 to be off
    P1DIR |= BIT0;              //sets pin 1.0 to output
    P4DIR |= BIT7;              //sets pin 4.7 to output
    TA0CCTL0 = CCIE;            //enable ccr interrupt
    TA0CCR0 = 1000;              //set overflow
    TA0CTL = TASSEL_2 + MC_1 + ID_3;    //set Timer A to ACLK on continuous mode with a divider of 8
    __enable_interrupt();       //enables the interrupt
    __bis_SR_register(LPM0_bits + GIE); // LPM0 with general interrupts enabled

}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A (void)
    {
    timerCount = (timerCount + 1);      //increment counter
    if(timerCount > 8){                 //if timer count reaches overflow
      P1OUT ^= BIT0;            //toggle pin 1.0 LED
      timerCount = 0;           //reset counter
    }
    timerCount1 = (timerCount1 + 1);   //increment counter
    if(timerCount1 > 32){              //if timer count reaches overflow
      P4OUT ^= BIT7;            //toggle pin 4.7 LED
      timerCount1 = 0;          //reset counter
    }
}
