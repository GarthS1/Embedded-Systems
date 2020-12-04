/* 
 * File:   main.c
 * Author: Garth-Work
 *
 * Created on November 24, 2020, 2:52 PM
 */
#include <xc.h>
#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include "ChangeClk.h"
#include "ADC.h"
#include "UART2.h"

//Preprocessor directives - Configuration bits for MCU start up
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor disabled


// MACROS for Idle, Sleep modes
#define Nop() {__asm__ volatile ("nop");}
#define ClrWdt() {__asm__ volatile ("clrwdt");}
#define Sleep() {__asm__ volatile ("pwrsav #0");}   //Sleep() - put MCU in sleep mode - CPU and some peripherals off
#define Idle() {__asm__ volatile ("pwrsav #1");}    //Idle() - put MCU in idle mode - only CPU off
#define dsen() {__asm__ volatile ("BSET DSCON, #15");} //


void displayADC(uint16_t value) {
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
        Disp2String("\r                ");  //Clearing written value
    }
}

int main(void) {
    NewClk(32);     //Use system clock 32kHz
    TRISAbits.TRISA3 = 1; //Enable RA3 as ADC input 
    InitUART2();   //Initialize UART2
         
    while(1) {
        displayADC(do_ADC());
    }
    
    return 0;
}

