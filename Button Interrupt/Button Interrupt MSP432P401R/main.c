#include <msp432.h>

/*
 * @author Nick Setaro
 * @version 9.28.2018
 *
 * This code uses the msp432P401R microcontroller to toggle an LED when a button is pressed.
 * A button interrupt is used to wake the cpu to toggle the LED.
 * Debouncing is included to keep the cpu from registering multiple incorrect interrupts.
 *
 */
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
