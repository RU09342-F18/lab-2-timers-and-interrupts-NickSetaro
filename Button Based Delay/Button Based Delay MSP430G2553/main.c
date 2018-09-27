#include <msp430.h> 
/*
 * @author Nick Setaro
 * @version 9.26.2018
 *
 * The Aclk is used to blink the green LED at a certain rate. The user can determine this rate by pushing and releasing a button.
 * The rate that the green LED blinks at will be the time that the button was pressed for. While the user is pressing the
 * button the red LED will be on to signify the new rate that the green LED will blink at.
 */
unsigned long time;
int reset = 0;
unsigned int start = 0;
unsigned int end =0;

void main(void) {

    WDTCTL = WDTPW | WDTHOLD;   //STOP WATCHDOG TIMER

    CCTL0 |= CCIE;                              //enable interrupt
    CCTL0 &= ~TAIFG;                            //clear flag
    TA0CTL = TASSEL_1 + MC_2 + ID_3;           // ACLK on continuous mode, clock divider (8)
    TA0CCR0 = 8;                               //20 Hz clock
    time = 8;                                    //increment value for CCR0

    P1IE |=  BIT3;                            // P1.3 interrupt enabled
    P1IES |= BIT3;                            // set p1.3 to falling edge
    P1REN |= BIT3;                            // Enable resistor on P1.3
    P1OUT |= BIT3;                             //Pull up resistor on P1.3
    P1IFG &= ~BIT3;                           // P1.3 Interrupt Flag cleared

    P1DIR |= BIT0;                              //P1.0 pin output
    P1DIR |= BIT6;                              //P1.6 pin output
    __bis_SR_register(GIE);

    while(1){
        if(reset)  {                        //if a reset is needed
            reset = 0;
            if(end <= start){
                TA0CCR0 = 65535-start+end+TA0R;     //change timer value
                time=65535-start+end;                 //update increment value
            }else{
                TA0CCR0=end-start+TA0R;
                time=end-start;
            }
        }
    }
}


//Timer A interrupt
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void) {
        P1OUT ^= BIT0;                          //toggle green LED
        TA0CCR0 += time;                          //increment CCR0
}


//Button interrupt
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){

    if(P1IES & BIT3){                   //if button was pressed
        start = TA0R;                       //time button was pressed
        P1OUT |= BIT6;                      //turn red LED on
    }else{
        end = TA0R;                     //time button was released
        reset = 1;                      //flag to recalculate time of button blink
        P1OUT &= ~BIT6;                 //turn red LED off
    }

    P1IES ^= BIT3;  //change edge to low to catch release or high to catch next press
    P1IFG &= ~BIT3;  //clear flag

}

