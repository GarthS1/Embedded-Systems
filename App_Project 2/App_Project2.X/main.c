/*
 * File:   newmainXC16.c
 * Author: blaob
 *
 * Created on December 2, 2020, 5:48 PM
 */

#include <xc.h>
#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include "ADC.h"
#include "UART2.h"
#include "ChangeClk.h"
#include "Ios.h"
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor disabled
#pragma config OSCIOFNC = ON //CLKO output disabled on pin 8, use as IO.

#define Nop() {__asm__ volatile ("nop");}
#define ClrWdt() {__asm__ volatile ("clrwdt");}
#define Sleep() {__asm__ volatile ("pwrsav #0");}   //Sleep() - put MCU in sleep mode - CPU and some peripherals off
#define Idle() {__asm__ volatile ("pwrsav #1");}    //Idle() - put MCU in idle mode - only CPU off
#define dsen() {__asm__ volatile ("BSET DSCON, #15");} //



int main(void) {
    NewClk(32);     //Use system clock 32kHz
    IOinit();   //Initialize IO ports
    CNinit();   //Initialize change notification
    InitUART2();   //Initialize UART2
    


    while(1) {}
    
    return 0;
}

