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
        delay_ms(1000);    //Delay 1s 
        LATBbits.LATB8 = 0; //Led turned off
        delay_ms(1000); 
    }else if(PORTAbits.RA4 == 0 && PORTBbits.RB4 == 1 && PORTAbits.RA2 == 1) { //IF PB2 is pressed 
        LATBbits.LATB8 = 1; //Led stays 
        delay_ms(2000);   //Delay 2s 
        LATBbits.LATB8 = 0; //Led turned off
        delay_ms(2000);   
    }else if(PORTBbits.RB4 == 0 && PORTAbits.RA4 == 1 && PORTAbits.RA2 == 1) {  //IF PB3 is pressed 
        LATBbits.LATB8 = 1; //Led stays 
        delay_ms(3000);   //Delay 3s 
        LATBbits.LATB8 = 0; //Led turned off
        delay_ms(3000);  
    }else if(PORTAbits.RA2 == 1 && PORTBbits.RB4 == 1 && PORTAbits.RA4 == 1) {  //If no button pressed 
        LATBbits.LATB8 = 0;  //Led turned off 
    }else{ //If any combination of buttons pressed
        LATBbits.LATB8 = 1; //Led stays on without blinking
    }
}

