#include <msp430.h>

/*
 * @author Nick Setaro
 * @version 9.25.2018
 *
 * This code uses the msp430G2553 to blink two different LEDs at different rates.
 * It uses a timer interrupt to blink the LEDs when the timer has overflowed.
 * It also uses different counters to change the rates on each LED.
 */

volatile unsigned int timerCount = 0;       //counter for first LED
volatile unsigned int timerCount1 = 0;      //counter for second LED

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    BCSCTL3 = LFXT1S_2;         //THIS LINE IS REQUIRED IF USING A CLOCK
    P1OUT |= (BIT0);            //sets the LED at p1.0 to on
    P1OUT &= ~BIT6;             //sets the LED at p1.6 to be off
    P1DIR |= (BIT0 + BIT6);     //sets pin 1.0 and 1.6 to output
    CCTL0 = CCIE;
    CCR0 = 999;                 //sets overlow limit for timer
    TACTL = TASSEL_1 + MC_1;    //sets the timer to up mode
    __enable_interrupt();       //enables the interrupt
    while(1);
}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)         //timer interrupt
    {
    timerCount = (timerCount + 1);      //increment counter
    if(timerCount ==8){                 //if interrupt has happened specified amount of times
      P1OUT ^= BIT0;                    //toggle first LED
      timerCount = 0;                   //reset counter
    }
    timerCount1 = (timerCount1 + 1);    //increment counter
    if(timerCount1 ==32){               //if interrupt has happened specified amount of times
      P1OUT ^= BIT6;                    //toggle second LED
      timerCount1 = 0;                  //reset counter
    }
    }
