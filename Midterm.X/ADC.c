/*
 * File:   ADC.c
 * Author: blaob
 *
 * Created on November 21, 2020, 12:29 PM
 */


#include "xc.h"
#include "ADC.h"
#include "UART2.h"
#include "ChangeClk.h"
#include "Ios.h"


void initADC() {
    AD1CON1bits.ADSIDL = 0;     //Continue ADC operation in idle mode
    AD1CON1bits.FORM = 0b00;    //Save data output as unsigned integer type
    AD1CON1bits.SSRC = 0b111;   //Internal counter ends sampling and starts conversion
    AD1CON1bits.ASAM = 0;       //Next Sampling begins when SAMP bit is set ...
    AD1CON1bits.SAMP = 0;       //...
    AD1CON2bits.VCFG = 0b000;   //Voltage reference configuration bits VR+ = AVDD, VR- = AVSS
    AD1CON2bits.CSCNA = 0;      //Don't scan input, use channel selected by CH0SA bit ...
    AD1CON2bits.SMPI = 0b0000;  //Enable ADC interrupt
    IPC3bits.AD1IP = 7;         //Priority level for interrupt
    IEC0bits.AD1IE = 1;         //Enable adc interrupt request
    AD1CON2bits.BUFM = 0;       //Enable 16-word buffer
    AD1CON2bits.ALTS = 0;       //Always uses MUX A input multiplexer settings
    AD1CON3bits.ADRC = 0;       //Use system clock
    AD1CON3bits.SAMC = 0b11111; //Sampling time 10 * 2 /fclk
    AD1CHSbits.CH0NA = 0;       //Set negative input to VR-
    AD1PCFG = 0xFFFF;           //Set all bits as digital 
    AD1CSSL = 0;                //Analog channel omitted from input scan 
}
/*
 * START SAMPLING FOR ONE TIME
 */
void doADC(int ANNUMBER) {
    initADC();
    
    if(ANNUMBER == 5) {
        AD1PCFGbits.PCFG5 = 0;  //Disable digital IO on pin 8.
        TRISAbits.TRISA3 = 1;   //Enable RA3 as ADC input 
        AD1CHSbits.CH0SA = 0b0101;  //Enable AN5 or pin number 8 to ADC input/
    }
    else if(ANNUMBER == 11) {
        AD1PCFGbits.PCFG11 = 0; //Disable digital IO on pin 16.
        TRISBbits.TRISB13 = 1;  //Enable RB13 as ADC input
        AD1CHSbits.CH0SA = 0b1011;  //Enable AN11 or pin number 16 to ADC input
    } else if(ANNUMBER == 12) {
        AD1PCFGbits.PCFG12 = 0; //Disable digital IO on pin 16.
        TRISBbits.TRISB12 = 1;  //Enable RB13 as ADC input
        AD1CHSbits.CH0SA = 0b1100;  //Enable AN11 or pin number 16 to ADC input
    }
    
    AD1CON1bits.ADON = 1; //Enable ADC module
    AD1CON1bits.SAMP = 1;   //Start sampling
}

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void) {
    IFS0bits.AD1IF = 0;     //Clear interrupt flag
    AD1CON1bits.ADON = 0;   //Turn off ADC
    if(PORTAbits.RA2 == 0 && PORTBbits.RB4 == 1 && PORTAbits.RA4 == 1) {
        displayVoltage(ADC1BUF0);
    }else if(PORTAbits.RA4 == 0 && PORTBbits.RB4 == 1 && PORTAbits.RA2 == 1) {
        displayResistance(ADC1BUF0);
    }else if(PORTBbits.RB4 == 0 && PORTAbits.RA4 == 1 && PORTAbits.RA2 == 1) {
    }
 
}   







