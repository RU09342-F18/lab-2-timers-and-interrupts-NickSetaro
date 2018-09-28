# Button Interrupt
This code uses a button interrupt to determine if an LED should be on. When the button is pressed the LED will toggle on and when it is released it will toggle off again. Debouncing has not been added to the msp430G2553 and msp430FR2311 boards so rapid button presses can change the direction that the LED toggles. The msp432P401R board uses a different kind of interrupt
