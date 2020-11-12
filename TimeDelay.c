#include "Ios.h"
#include <xc.h>
#include <stdio.h>

// User delay function for LED's 
void delay_ms(uint16_t time_ms)
{ 
    NewClk(32);         //Configure clock 
    T2CON = 0;          //CLEAR T2CON.
    TMR2 = 0;           //CLEAR THE TIMER
    
    IFS0bits.T2IF = 0;   //Clear timer 2 interrupt flag
    T2CONbits.TCS = 0;   //Use FOSC INTERNAL CLOCK
    T2CONbits.T32 = 0;   //Use 16 bit timer
    T2CONbits.TSIDL = 0; //Continue module operation in idle mode
    
    //Prescaler to 8
    T2CONbits.TCKPS0 = 1;  
    T2CONbits.TCKPS1 = 0;
    
    //Set interrupt priority to 1
    IPC1bits.T2IP2 = 1;
    IPC1bits.T2IP1 = 1;
    IPC1bits.T2IP0 = 1;  
     
    PR2 = 2 * time_ms ;     
    
    /*
     * 2 multiply time_ms because:
     * Frequency for the clock is 32000 Hz
     * Prescaler as set above is 8
     * We have formula:
     * PR2 * Prescaler * 2/(clock_frequency) = desired time in second
     * In our case:
     * PR2 * time_ms * Prescaler * 2/clock_frequency * 1000 = desired time in ms
     * For each second we have:
     * (PR2 * 1000 ms) * 8 * 2 / 32000 * 1000 = 1000ms
     * -> PR2 = 2;
     */
    
    IEC0bits.T2IE = 1;   //Enable timer interrupt.
    T2CONbits.TON = 1;  //Enable timer.
    Idle();             //Stay here until interrupt trigger.
}

// Interrupt function for timer 2
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void)
  {
    IFS0bits.T2IF = 0; //Clear timer 2 interrupt flag 
    T2CONbits.TON = 0; //Disable the timer
 }
