#include <msp430.h>

/*
 * @author Nick Setaro
 * @version 9.24.2018
 *
 * This code uses the msp430F5529LP microcontroller to toggle an LED from a button press.
 * It uses an interrupt from the button to wake the cpu and enable the LED to be toggled.
 * Button debouncing is not included so the LED will not toggle perfectly.
 */
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;       //stop watchdog timer
    P1DIR |= BIT0;                   //set pin 1.0 to output
    P2REN |= BIT1;                   //enable resistor for pin 2.1
    P2OUT |= BIT1;                   //set resistor for pin 2.1 to pull up
    P2IE |= BIT1;                    //sets pin 2.1 as an interrupt pin
    P2IFG &= ~BIT1;                 //clears interrupt flags
    __enable_interrupt();           //enable interrupt
    while(1);
}
 #pragma vector=PORT2_VECTOR         //indicate interrupt vector being used
__interrupt void Port_2(void)
{
    P1OUT ^= BIT0;                  //toggle LED
    P2IFG = ~BIT1;                  //clear interrupt flag
}
