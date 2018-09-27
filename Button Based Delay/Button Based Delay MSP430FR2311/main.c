#include <msp430.h>
#include <msp430FR2311.h>

/*
 * @author Nick Setaro
 * @version 9.27.2018
 *
 * Aclk is used to blink an LED at some initial rate. The user can then change the rate by
 * pressing a button. The LED will blink at a rate equal to how long the button was pressed.
 */
unsigned long time;
int reset = 0;
unsigned int start = 0;
unsigned int end =0;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   //STOP WATCHDOG TIMER
    PM5CTL0 &= ~LOCKLPM5;       //Stop Default High-Impedance Mode
    P1DIR |= BIT0;          //set LED to output
    P2DIR |= BIT0;          //set LED to output
    P2OUT |= BIT0;          //turn green LED on
    time = 8;               //increment value for CCR0


    P1DIR &= ~(BIT1);       //set button to input
    P1IE |= BIT1;           //enable button interrupt
    P1IES |= BIT1;          //set edge to high
    P1REN |= BIT1;          //enable pull up resistor on button
    P1IFG &= ~(BIT1);       // clear interrupt flags
    P1OUT |= BIT1;          //pull up resistor on button

    TB0CCTL0 |= CCIE;
    TB0CTL = TBSSEL_1 + MC_1 + ID_3;     //ACLK on up mode, clock divider (8)
    TB0CCR0 = 8;                        //20 Hz clock

    _BIS_SR(GIE);                   //enable interrupts

    while(1){
            if(reset)  {            //if a reset is needed
                reset = 0;
                if(end <= start){
                    TB0CCR0 = 65535-start+end+TB0R;     //change timer value
                    time=65535-start+end;                 //update increment value
                }else{
                    TB0CCR0=end-start+TB0R;
                    time=end-start;
                }
            }
        }
}

//Timer B interrupt
#pragma vector = TIMER0_B0_VECTOR
__interrupt void TB0_ISR(void)
{
        P1OUT ^= BIT0;                          //toggle red LED
        TB0CCR0 += time;                        //increment CCR0
}

//Button interrupt
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void)
{

    if(P1IES & BIT1)  {                //button pressed
        start = TB0R;                   //time button was pressed
        P2OUT |= BIT0;                  //turn green LED on
    }else {
        end = TB0R;                     //time button was released
        reset = 1;                      //flag to recalculate time
        P2OUT &= ~BIT0;                 //turn green LED off
 }
    P1IES ^= BIT1;                      //change edge
    P1IFG &= ~BIT1;                     //clear flags
}
