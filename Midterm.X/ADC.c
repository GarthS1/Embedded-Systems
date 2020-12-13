/*
 * File:   ADC.c
 * Author: blaob
 *
 * Created on November 21, 2020, 12:29 PM
 */


#include "xc.h"
#include "ADC.h"
#include "Ios.h"

//Initialize the ADC 
void initADC() {
    //AD1CON1bits.ADON=0;			//Turn off ADC, turn on before sampling in do_ADC()
    AD1CON1bits.ADSIDL = 0;     //Continue ADC operation in idle mode
    AD1CON1bits.FORM = 0b00;    //Save data output as unsigned integer type
    AD1CON1bits.SSRC = 0b111;   //Internal counter ends sampling and starts conversion
    AD1CON1bits.ASAM = 0;       //Next Sampling begins when SAMP bit is set 
    AD1CON1bits.SAMP = 0;       //Set this bit in ADC routine to start sampling
    
    // Sampling time and ADC Clk select
    AD1CON2bits.VCFG = 0b000;   //Voltage reference configuration bits VR+ = AVDD, VR- = AVSS
    AD1CON2bits.CSCNA = 0;      //Don't scan input, use channel selected by CH0SA bit ...
    AD1CON2bits.SMPI = 0b0000;  //Enable ADC interrupt
    AD1CON2bits.BUFM = 0;       //Enable 16-word buffer
    AD1CON2bits.ALTS = 0;       //Always uses MUX A input multiplexer settings
    AD1CON3bits.ADRC = 0;       //Use system clock
    AD1CON3bits.SAMC = 0b11111; //Sampling time 10 * 2 /fclk
    AD1CON3bits.ADCS=0b00001; //ADCS conversion clock selet bits. Ignored if using internal AD RC clock.
    
    //Interrupts 
    IPC3bits.AD1IP = 6;         //Priority level for interrupt
    IEC0bits.AD1IE = 1;         //Enable adc interrupt request
    
    AD1CHSbits.CH0NA = 0;       //Set negative input to VR-
    AD1CHSbits.CH0NB=0;         //Ch0 MUXB -ve Input is VR-
    AD1PCFG = 0xFFFF;           //Set all bits as digital 
    AD1CSSL = 0;                //Analog channel omitted from input scan 
}

//Start sampling
//ANNUMBER - the pin that the voltage should be read from 
void doADC(int ANNUMBER) {
    initADC();
    
    if(ANNUMBER == 5) {
        AD1CHSbits.CH0SB=0b0101;	//Positive input is AN5/RA3/pin8 for MUXB
        AD1CHSbits.CH0SA=0b0101;	//Positive input is AN5/RA3/pin8 for MUXA
        AD1PCFGbits.PCFG5 = 0;  //Disable digital IO on pin 8.
        TRISAbits.TRISA3 = 1;   //Enable RA3 as ADC input 
    }
    else if(ANNUMBER == 11) {
        AD1PCFGbits.PCFG11 = 0; //Disable digital IO on pin 16.
        TRISBbits.TRISB13 = 1;  //Enable RB13 as ADC input
        AD1CHSbits.CH0SA = 0b1011;  //Enable AN11 or pin number 16 to ADC input
        AD1CHSbits.CH0SB=0b1011;	//Positive input is AN11/pin16 for MUXB
    }
    else if(ANNUMBER == 12) {
        AD1PCFGbits.PCFG12 = 0; //Disable digital IO on pin 15.
        TRISBbits.TRISB12 = 1;  //Enable RB12 as ADC input
        AD1CHSbits.CH0SA = 0b1100;  //Enable AN12 or pin number 15 to ADC input
        AD1CHSbits.CH0SB = 0b1100;	//Positive input is AN12/pin15 for MUXB
    }
    
    AD1CON1bits.ADON = 1; //Enable ADC module
    AD1CON1bits.SAMP = 1; //Start Sampling
}

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void) { 
    if(PORTAbits.RA2 == 0 && PORTBbits.RB4 == 1 && PORTAbits.RA4 == 1) {
        displayVoltage(ADC1BUF0);
    }else if(PORTAbits.RA4 == 0 && PORTBbits.RB4 == 1 && PORTAbits.RA2 == 1) {
        displayResistance(ADC1BUF0);
    }else if(PORTBbits.RB4 == 0 && PORTAbits.RA4 == 1 && PORTAbits.RA2 == 1) {
        displayPulse(ADC1BUF0);
    }
    AD1CON1bits.ADON = 0;   //Turn off ADC, turn on before sampling in do_ADC()
    IFS0bits.AD1IF = 0;     //Clear interrupt flag
}   
