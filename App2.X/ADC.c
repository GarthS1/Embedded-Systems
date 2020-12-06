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

int RA2PB_FLAG = 0;
int RA4PB_FLAG = 0;

void buttonPushed() {
    if(PORTAbits.RA2 == 0)
        RA2PB_FLAG = 1;
    else if(PORTAbits.RA4 == 0)
        RA4PB_FLAG = 1;
}
/*
 * START SAMPLING FOR ONE TIME
 */
void init_do_ADC(void) {
    buttonPushed(); //Determine if RA2 or RA4 was pushed
    
    AD1CON1bits.ADON = 1; //Enable ADC module
    
    AD1CON1bits.FORM0 = 0;
    AD1CON1bits.FORM1 = 0; //Save data output as unsigned integer type.
    
    AD1CON1bits.SSRC0 = 1;
    AD1CON1bits.SSRC1 = 1;
    AD1CON1bits.SSRC2 = 1; //Internal counter ends sampling and starts conversion
   
    AD1CON2bits.VCFG = 0b000;   //Voltage reference configuration bits VR+ = AVDD, VR- = AVSS
    AD1CON2bits.CSCNA = 0;      //Do not scan input
    
    AD1CON2bits.SMPI = 0b0000; //Only if you want to use interrupt
    IPC3bits.AD1IP = 5;         //Priority level for interrupt
    IEC0bits.AD1IE = 1;         //Enable adc interrupt
    
    
    AD1CON2bits.BUFM = 0;   //Enable 16-word buffer
    AD1CON2bits.ALTS = 0;   //Always uses MUX A input multiplexer settings
    
    AD1CON3bits.ADRC = 0;   //Use system clock
    AD1CON3bits.SAMC = 0b01010;     //Sampling time 10 * 2 /fclk
    
    AD1CHSbits.CH0NA = 0;   //Set negative input to VR-
    AD1CHSbits.CH0SA = 0b0101;  //Enable AN5 or pin number 8 to ADC input/
    
    AD1PCFGbits.PCFG5 = 0;   //Disable digital IO on pin 8.
    AD1PCFGbits.PCFG11 = 0;   //Disable digital IO on pin 16.
    
    AD1CSSLbits.CSSL0 = 0;
    AD1CSSLbits.CSSL1 = 0;
    AD1CSSLbits.CSSL2 = 0;
    AD1CSSLbits.CSSL3 = 0;
    AD1CSSLbits.CSSL4 = 0;
    AD1CSSLbits.CSSL5 = 0;
    AD1CSSLbits.CSSL10 = 0;
    AD1CSSLbits.CSSL11 = 0;
    AD1CSSLbits.CSSL12 = 0; //Analog channel omitted from input scan
    
    AD1CON1bits.SAMP = 1;
}

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void) {
//    displayADC(ADC1BUF0);   //Display ADC1BUF0 value
    IFS0bits.AD1IF = 0; //Clear interrupt flag
    
    if(RA2PB_FLAG) {
        displayVoltage(ADC1BUF0);
        RA2PB_FLAG = 0; //Reset the flag
    }else if(RA4PB_FLAG) {
        displayResistance(ADC1BUF0);
        RA4PB_FLAG = 0; //Reset the flag
    }
}   






