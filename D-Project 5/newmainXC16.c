/*
 * File:   newmainXC16.c
 * Author: 
 *
 * Created on November 21, 2020, 12:29 PM
 */


#include "xc.h"
#include "ChangeClk.h"
#include "ADC.h"
#include "UART2.h"

int main(void) {
    NewClk(32);     //Use system clock 32kHz
    TRISAbits.TRISA3 = 1; //Enable RA3 as ADC input 
    
    while(1) {
        Disp2String("\r");
        Disp2Dec(do_ADC());
        Disp2String("          ");
    }
    
    return 0;
}
