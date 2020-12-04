/*
 * File:   IOS.c
 * Author: Garth Slaney, Long Ta, Weitao Wu
 *
 * Created on October 13, 2020, 11:13 PM
 */

#include "Ios.h"
#include <xc.h>
#include <stdio.h>
//This function initializes IO ports.
void IOinit() {
    AD1PCFG = 0xFFFF; // Turn all analog pins as digital
    TRISBbits.TRISB8 = 0; //RB8 is output.
    CNPU1bits.CN0PUE = 1; //Enables pull up resistor on RA4/CN0
    CNPU2bits.CN30PUE = 1; //Enables pull up resistor on RA2/CN3
    CNPU1bits.CN1PUE = 1; //Enables pull up resistor on RB4/CN1
    TRISAbits.TRISA2 = 1; //RA2 is input, PB1 is connected to this port.
    TRISAbits.TRISA4 = 1; //RA4 is input, PB2 is connected to this port.
    TRISBbits.TRISB4 = 1; //RB4 is input, PB3 is connected to this port
}

//This function implements the IO checks and LED blinking functions
void IOcheck() {
    if(PORTAbits.RA2 == 0 && PORTBbits.RB4 == 1 && PORTAbits.RA4 == 1) {  //If PB1 is pressed 
        LATBbits.LATB8 = 1; //Led stays on without blinking
        delay_ms(1000);    //Delay 1s PR2 = 4
        LATBbits.LATB8 = 0; //Led turned off
        delay_ms(1000); 
    }else if(PORTAbits.RA4 == 0 && PORTBbits.RB4 == 1 && PORTAbits.RA2 == 1) { //IF PB2 is pressed 
        LATBbits.LATB8 = 1; //Led stays 
        delay_ms(2000);   //Delay 2s PR2 = 8
        LATBbits.LATB8 = 0; //Led turned off
        delay_ms(2000);   
    }else if(PORTBbits.RB4 == 0 && PORTAbits.RA4 == 1 && PORTAbits.RA2 == 1) {  //IF PB3 is pressed 
        LATBbits.LATB8 = 1; //Led stays 
        delay_ms(3000);   //Delay 3s PR2 = 12
        LATBbits.LATB8 = 0; //Led turned off
        delay_ms(3000);  
    }else if(PORTAbits.RA2 == 1 && PORTBbits.RB4 == 1 && PORTAbits.RA4 == 1) {  //If no button pressed 
        LATBbits.LATB8 = 0;  //Led turned off 
    }else{ //If any combination of buttons pressed
        LATBbits.LATB8 = 1; //Led stays on without blinking
    }
}

void delay_ms(uint16_t time_ms){ 
    T2CON = 0;          //CLEAR T2CON.
    TMR2 = 0;           //CLEAR THE TIMER
    
    T2CONbits.TCS = 0;  //Use FOSC INTERNAL CLOCK
    T2CONbits.T32 = 0;  //Use 16 bit timer
    T2CONbits.TSIDL = 0; //Continue module operation in idle mode
    
    T2CONbits.TCKPS0 = 0;   //Prescaler to 8
    T2CONbits.TCKPS1 = 1;
    
    IPC1bits.T2IP2 = 1;
    IPC1bits.T2IP1 = 1;
    IPC1bits.T2IP0 = 1;  //Set interrupt priority to 7.
    
    PR2 = 2 * time_ms;     
    
    /*
     * 2 multiply time_ms because:
     * Frequency for the clock is 32000 Hz
     * Prescaler as set above is 8
     * We have formula:
     * PR2 * Prescaler * 2/(clock_frequency) = desired time in second
     * In our case:
     * PR2 * time_ms * Prescaler * 2/clock_frequency * 1000 = desired time in ms
     * For each second we have:
     * (PR2 * 1000 ms) * 8 * 2 / 32000 * 1000 = 1000 ms
     * -> PR2 = 2;
     */
    
    IEC0bits.T2IE = 1;   //Enable timer interrupt.
    T2CONbits.TON = 1;  //Enable timer.
    Idle();             //Stay here until interrupt trigger.
}

void __attribute__((interrupt, auto_psv)) _T2Interrupt(void)
  {
    IFS0bits.T2IF = 0;
    T2CONbits.TON = 0; //Disable the timer
 }
