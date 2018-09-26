#include <msp430.h>

/*
 * @author Nick Setaro
 * @version 9.24.2018
 *
 * This code uses the the msp430G2553 to toggle an LED on a button press.
 * It uses a button interrupt to toggle the LED.
 * Button debouncing has not been added so the LED will not toggle perfectly.
 */
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;       //stop watchdog timer
    P1DIR |= BIT6;                   //set pin 1.6 to output
    P1REN |= BIT3;                   //enable resistor for pin 1.3
    P1OUT |= BIT3;                   //set resistor for pin 1.3 to pull up
    P1IE |= BIT3;                    //sets pin 1.3 as an interrupt pin
    P1IFG &= ~BIT3;                 //clears interrupt flags
    __enable_interrupt();           //enable interrupt
    while(1);

}
 #pragma vector=PORT1_VECTOR         //indicate interrupt vector being used
__interrupt void Port_1(void)
{
    P1OUT ^= BIT6;                  //toggle LED
    P1IFG = ~BIT3;                  //clear interrupt flag
}
