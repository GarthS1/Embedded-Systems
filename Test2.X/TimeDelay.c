/*
 * File:   TimeDelay.c
 * Author: blaob
 *
 * Created on December 6, 2020, 4:08 PM
 */


#include "xc.h"
#include "TimeDelay.h"


void delay_ms(uint16_t time_ms, uint8_t idle_on)
{
    //T2CON config
    T2CONbits.TSIDL = 0; //operate in idle mode
    T2CONbits.T32 = 0; // operate timer 2 as 16 bit timer
    T2CONbits.TCS = 0; // use internal clock
    // T2CONbits.TGATE = 0;
    
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
    T2CONbits.TON=0; // Stop timer
    return;
}

void configureTimer1() {
    T1CONbits.TSIDL = 0;    //Continue operation in idle mode
    //T1CONbits.TCKPS = 0b11;   //Highest clock resolution
    T1CONbits.TCS = 0;      //Clock source is internal clock
    //T1CONbits.TGATE = 1;    //Enable gated time accumulation  Only count pulse when high, when low, throw interrupt
    //Do I need to trigger gated time accumilation?
    IEC0bits.T1IE = 1; //enable timer interrupt
    IPC0bits.T1IP = 7;  //Interrupt priority 2
    IFS0bits.T1IF = 0;  //Clear interrupt flag
    TMR1 = 0;
    PR1 = 0xFFFF;  //For the sake of simplicity, might need to change, ideally this should be free running but we set it at 1000 for now
    
    T1CONbits.TON = 1;  //Start timer 1
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    T1CONbits.TON = 0; // Stop timer  
    IFS0bits.T1IF = 0;  //Clear timer 1 interrupt flag
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void)
{
    IFS0bits.T2IF=0; //Clear timer 2 interrupt flag
    return;
}

