/*
 * File:   TimeDelay.c
 * Author: Rushi V
 *
 * Created on October 5, 2020, 4:05 PM
 */


#include "xc.h"
#include "TimeDelay.h"
#include "Ios.h"
#include <stdio.h>

void delay_ms(uint16_t time_ms, uint8_t idle_on)
{
    //T2CON config
    T2CONbits.TSIDL = 0; //operate in idle mode
    T2CONbits.T32 = 0; // operate timer 2 as 16 bit timer
    T2CONbits.TCS = 0; // use internal clock
    
    // Timer 2 interrupt config
    IPC1bits.T2IP = 2; //7 is highest and 1 is lowest priority
    IEC0bits.T2IE = 1; //enable timer interrupt
    IFS0bits.T2IF = 0; // Clear timer 2 flaf
    
    PR2 = time_ms << 4; //After PR2 simplification
    TMR2 = 0;
    T2CONbits.TON = 1; //start timer
    
    if(idle_on == 1)
    {
        Idle(); 
    }
    T2CONbits.TON = 0; // Stop timer
    return;
}

//Start the timer
void startTimer() {
    T3CONbits.TSIDL = 0;      //operate in idle mode
    T3CONbits.TON = 0;        //Disable Timer
    T3CONbits.TCS = 0;        //Clock source is internal clock
    T3CONbits.TGATE = 1;      //Enable gated time accumulation  Only count pulse when high, when low, throw interrupt
    T3CONbits.TCKPS = 0b00;   //1:1 prescaler 
    T2CONbits.T32 = 0;        // operate timer 2 as 16 bit timer
    TMR3 = 0;                 //Clear Timer Register
    PR3 = 0xFFFF;             //Set to max to make sure timer only ends on a falling edge 
    
    IPC2bits.T3IP = 7;        //Timer 3 interrupt priority is 7
    IEC0bits.T3IE = 1;        //enable timer interrupt 
    IFS0bits.T3IF = 0;        //Clear interrupt flag

    T3CONbits.TON = 1;        // Start timer  
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {
    T3CONbits.TON = 0;    // Stop timer  
    IFS0bits.T3IF = 0;    //Clear timer 3 interrupt flag  
    calculateFrequency();
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {
    IFS0bits.T2IF = 0; //Clear timer 2 interrupt flag
    return;
}