/*
 * File:   newmainXC16.c
 * Author: blaob
 *
 * Created on December 2, 2020, 5:48 PM
 */

// MPLAB header libraries
#include <xc.h>
#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>

// User header files
#include "ChangeClk.h"
#include "Ios.h"
#include "UART2.h"
#include "ADC.h"


// CLOCK CONTROL 
#pragma config IESO = OFF    // 2 Speed Startup disabled
#pragma config FNOSC = FRC  // Start up CLK = 8 MHz
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor disabled
#pragma config SOSCSEL = SOSCLP // Secondary oscillator for Low Power Operation
#pragma config POSCFREQ = MS  //Primary Oscillator/External clk freq betwn 100kHz and 8 MHz. Options: LS, MS, HS
#pragma config OSCIOFNC = ON  //CLKO output disabled on pin 8, use as IO. 
#pragma config POSCMOD = NONE  // Primary oscillator mode is disabled


// Global variables
unsigned int clock_val = 8;


// MACROS for Idle, Sleep modes
#define Nop() {__asm__ volatile ("nop");}
#define ClrWdt() {__asm__ volatile ("clrwdt");}
#define Sleep() {__asm__ volatile ("pwrsav #0");}   //Sleep() - put MCU in sleep mode - CPU and some peripherals off
#define Idle() {__asm__ volatile ("pwrsav #1");}    //Idle() - put MCU in idle mode - only CPU off
#define dsen() {__asm__ volatile ("BSET DSCON, #15");} //

int main(void) {
    IOinit();   //Initialize IO ports
    CNinit();   //Initialize change notification
    InitUART2();   //Initialize UART2
    
    //PULSE GENERATION ON REFO/RB15/PIN18
    TRISBbits.TRISB15 = 0; // Set RB15/pin18 as output for REFO
    REFOCONbits.ROEN = 1; // Ref oscillator is enabled
    REFOCONbits.ROSSLP = 1; // Ref oscillator is disabled in sleep
    REFOCONbits.ROSEL = 0; // Output base clk showing clock switching
    REFOCONbits.RODIV =  0b1101; //changes frequency of pulse gen
    
    // Change Clock
    NewClk(clock_val); // 8 for 8 MHz; 500 for 500 kHz; 32 for 32 kHz 
    
    while(1) {}

    return 0;
}

