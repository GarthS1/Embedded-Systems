/*
 * File:   ADC.c
 * Author: blaob
 *
 * Created on November 21, 2020, 12:29 PM
 */


#include "xc.h"
#include "ADC.h"
#include "UART2.h"

//Display bar graph and value in hex
void displayADC(uint16_t value) {
    //If value too small only show hex 
    if(value >= 30) {
        uint16_t size = value / 30; //Since the adc value can be big, we divide the value by 30 to display in one line
        char result[size];  //number of needed character to write
        char clear[size];   //number of needed character to clear the write

        int i = 0;
         while(i < size - 1) { //Populate two arrays
            result[i] = '*';    //Populate first array with *
            clear[i] = ' ';     //Populate clear array with empty character
            i++;
        }
        result[size - 1] = '\0';    //Null terminator at the end of the string
        clear[size - 1] = '\0';  
        Disp2String("\r");  //Start writing the value  
        Disp2String(result);    //Display the bar graph
        Disp2Hex(value);    //Display the value in hex from unint16_t 
        Disp2String("\r");  //Start clearing written value
        Disp2String(clear); //Clear character
        Disp2String("                ");    //Clear hex character at the end
    } else {
        Disp2String("\r");  //Start writing the value  
        Disp2Hex(value);    //Display the value in hex from unint16_t
        Disp2String("\r");  //Start clearing written value
        Disp2String("                ");    //Clear hex character at the end
    }
}

//Return the value of the ADC's output 
uint16_t do_ADC(void) {
    AD1CON1bits.ADON = 1; //Enable ADC module
    
    //Save data output as unsigned integer type.
    AD1CON1bits.FORM0 = 0;
    AD1CON1bits.FORM1 = 0; 
    
    //Internal counter ends sampling and starts conversion
    AD1CON1bits.SSRC0 = 1;
    AD1CON1bits.SSRC1 = 1;
    AD1CON1bits.SSRC2 = 1; 
   
    AD1CON1bits.ASAM = 0; //Sampling begins when SAMP bit is set 
    
    AD1CON2bits.VCFG = 0b000;   //Voltage reference configuration bits VR+ = AVDD, VR- = AVSS
    
    AD1CON2bits.CSCNA = 0;      //Do not scan input 
    AD1CON2bits.BUFM = 0;   //Enable 16-word buffer
    AD1CON2bits.ALTS = 0;   //Always uses MUX A input multiplexer settings
    
    AD1CON3bits.ADRC = 0;   //Use system clock
    AD1CON3bits.SAMC = 0b01010;     //Sampling time 10 * 2 /fclk
    
    AD1CHSbits.CH0NA = 0;   //Set negative input to VR-
    AD1CHSbits.CH0SA = 0b0101;  //Enable AN5 or pin number 8 to ADC input/
    
    AD1PCFGbits.PCFG5 = 0;   //Disable digital IO on pin 8.
    
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
    while(!AD1CON1bits.DONE) {}
    
    uint16_t value = ADC1BUF0;  //SAVE ADC1BUF0 output to value
    AD1CON1bits.SAMP = 0;       //Stop sampling
    AD1CON1bits.ADON = 0;       //Turn off ADC
    
    return value;
}