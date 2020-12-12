/*
 * File:   IOS.c
 * Author: Garth Slaney, Long Ta, Weitao Wu
 *
 * Created on October 13, 2020, 11:13 PM
 */

#include <xc.h>
#include "Ios.h"
#include "UART2.h"
#include "ADC.h"
#include <math.h>
#include "TimeDelay.h"

#define VREF 3.2

//This function initializes IO ports.
void IOinit() {
    AD1PCFG = 0xFFFF; // Turn all analog pins as digital
    CNPU1bits.CN0PUE = 1; //Enables pull up resistor on RA4/CN0
    CNPU2bits.CN30PUE = 1; //Enables pull up resistor on RA2/CN3
    CNPU1bits.CN1PUE = 1; //Enables pull up resistor on RB4/CN1
    TRISAbits.TRISA2 = 1; //RA2 is input, PB1 is connected to this port.
    TRISAbits.TRISA4 = 1; //RA4 is input, PB2 is connected to this port.
    TRISBbits.TRISB4 = 1; //RB4 is input, PB3 is connected to this port
}


void CNinit() {
    CNEN1bits.CN1IE = 1;    //Configure change of notification input of RB4
    CNEN1bits.CN0IE = 1;    //Configure change of notification input of RA4
    CNEN2bits.CN30IE = 1;   //Configure change of notification input of RA2
    IEC1bits.CNIE = 1;      //Set interrupt request to occur
    IPC4bits.CNIP0 = 0;         
    IPC4bits.CNIP1 = 1;
    IPC4bits.CNIP2 = 1;     //Change in IO Notifcation priority 6.
}

//This function implements the IO checks and changing the modes
void IOcheck() {
    IEC1bits.CNIE = 0; //disable CN interrupts to avoid debounces
    delay_ms(400,1);   // 400 msec delay to filter out debounces 
    IEC1bits.CNIE = 1; //Enable CN interrupts to detect pb release
    while(PORTAbits.RA2 == 0 && PORTBbits.RB4 == 1 && PORTAbits.RA4 == 1) {  //If PB1 is pressed 
        doADC(5);
    }
    
    while(PORTAbits.RA4 == 0 && PORTBbits.RB4 == 1 && PORTAbits.RA2 == 1) { //IF PB2 is pressed 
        doADC(11);
    }
    
    while(PORTBbits.RB4 == 0 && PORTAbits.RA4 == 1 && PORTAbits.RA2 == 1) {  //IF PB3 is pressed 
        doADC(12);
    }
}

void displayVoltage(uint16_t adc_value) {
    uint16_t vol = adc_value*(VREF/(pow(2,10)-1));  //Vin = Vref * ADCBUF/(2^10 - 1) 
     //display voltage
    Disp2String("\rVOLTMETER Voltage = ");
    Disp2Dec(vol);
    Disp2String("V");
    Disp2String("                                   ");
}

//Display the resistance
void displayResistance(uint16_t adc_value) {
    //Vin = Vref * (R-DUT/(1000 + R-DUT))
    //Vin/Vref = ADCBUF/1023 
    //R-DUT = 1000*(ADCBUF/1023)/(1-ADCBUF/1023)
    float vol = adc_value*(VREF/(pow(2,10)-1));
    uint16_t R = 1000*vol/(VREF - vol); 
    //display resistance
    Disp2String(" \r OHMMETER Resistance="); 
    Disp2Dec(R);
    Disp2String("OHM");
    Disp2String("                                   ");
}

void displayPulse(uint16_t value) {
    uint16_t freq = 0;
    uint16_t amplitude;
    Disp2String(" \r PULSEMETER Freq="); 
    Disp2Dec(freq);
    Disp2String("kHz, Amplitude="); 
    Disp2Dec(amplitude);
    Disp2String("V");
}

//Interrupt routine for _CNInterrupt
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void) {
    IFS1bits.CNIF = 0;		// clear IF flag
    T2CONbits.TON = 0;    // Disable timer
    IEC0bits.T2IE = 0;    //Disable timer interrupt
    IOcheck();
    Nop();	 
}
