/*
 * File:   IOS.c
 * Author: Garth Slaney, Long Ta, Weitao Wu
 *
 * Created on October 13, 2020, 11:13 PM
 */

#include "Ios.h"
#include <xc.h>
#include <stdio.h>
#include "TimeDelay.h"
#include "UART2.h"

int CN30Flag = 0;   //RA2 push button flag
int CN0Flag = 0;    //RA4 push button flag
int CN1Flag = 0;    //RB4 push button flag
int seconds = 0;    //seconds on timer
int minutes = 0;    //minutes on timer
int timer_flag = 0;
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
    CNinit();
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


//Interrupt routine for _CNInterrupt
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void) {
  NewClk(32); //switch to lower clock 
  if(IFS1bits.CNIF == 1) {
        if(PORTBbits.RB4 == 0) {
            CN1Flag = 1 - CN1Flag;  //Switch between 0 and 1. First time user presses button, change to 1, when un -presses the button flag change to 0.
        }
        if(PORTAbits.RA4 == 0) {
            CN0Flag = 1 - CN0Flag;
        }
        if(PORTAbits.RA2 == 0) {
            CN30Flag = 1 - CN30Flag;
        }   
    }
    // Button 1 pressed call pushButton1()
    if(CN30Flag)
        pushButton1();
    // Button 2 pressed call pushButton2()
    if(CN30Flag)
        pushButton2();
    // Button 3 pressed call pushButton3()
    if(CN30Flag)
        pushButton3();
    NewClk(8); //switch to higher clock    
    IFS1bits.CNIF = 0; //Clear CNIF Flag
    Nop();
}

void pushButton1() {
    
}

void pushButton2() {
    
}

void pushButton3() {
    int time = countTimer();
    if(time < 3)
        shortPresses(timer_flag);
    else
        resetTimer();
}

int countTimer() {
    return 0;
}

void resetTimer() {
    Disp2String("\r00m : 00s/t/t/t");  //Reset to 0.
}

void shortPresses(int& timer_flag) {
    timer_flag = 1 - timer_flag;
    if(timer_flag)
        startTimer();
}

void startTimer(int& timer_flag) {
    while(seconds && minutes && timer_flag) {
        Disp2String("\r"); 
        Disp2Dec(minutes);
        Disp2String(" : "); 
        Disp2Dec(seconds);
        Disp2String("\t\t\t"); 
        seconds--;
       if(seconds == -1 && minutes > 0) {
           seconds = 59;
           minutes--;
       }
       delay_ms(1000);
    }
    
    if(seconds == 0 && minutes == 0)
        timer_flag = 0;             //Reset timer_flag after counting down.
}



/*void blinkingLed() {
  // run loop unless not buttons are pressed
  while(!(PORTAbits.RA2 == 1 && PORTBbits.RB4 == 1 && PORTAbits.RA4 == 1)){
    if(PORTAbits.RA2 == 0 && PORTBbits.RB4 == 1 && PORTAbits.RA4 == 1) {  //If PB1 is pressed 
        Disp2String("\rPB1 is pressed           ");
        LATBbits.LATB8 = 1; //Led stays on without blinking
        delay_ms(500);    //Delay 500ms 
        LATBbits.LATB8 = 0; //Led turned off
        delay_ms(500); 
    }else if(PORTAbits.RA4 == 0 && PORTBbits.RB4 == 1 && PORTAbits.RA2 == 1) { //IF PB2 is pressed 
        Disp2String("\rPB2 is pressed        ");
        LATBbits.LATB8 = 1; //Led stays 
        delay_ms(2000);   //Delay 2s 
        LATBbits.LATB8 = 0; //Led turned off
        delay_ms(2000);   
    }else if(PORTBbits.RB4 == 0 && PORTAbits.RA4 == 1 && PORTAbits.RA2 == 1) {  //IF PB3 is pressed 
        Disp2String("\rPB3 is pressed        ");
        LATBbits.LATB8 = 1; //Led stays 
        delay_ms(3000);   //Delay 3s 
        LATBbits.LATB8 = 0; //Led turned off
        delay_ms(3000);  
    }else{ //If any combination of buttons pressed
        if(!PORTAbits.RA2 && !PORTAbits.RA4 && !PORTBbits.RB4 ) {
            Disp2String("\rAll PBs pressed       ");
        } else if(!PORTAbits.RA2) {
            Disp2String("\rPB1 and PB3 are pressed          ");
        }else if(!PORTAbits.RA4) {
            Disp2String("\rPB2 and PB3 are pressed          ");
        } else if(!PORTBbits.RB4) {
            Disp2String("\rPB1 and PB2 are pressed           ");
        }
        LATBbits.LATB8 = 1; //Led stays on without blinking
    }
  }
  Disp2String("\rNothing pressed             ");
  LATBbits.LATB8 = 0;  //Led turned off 
  CN1Flag = 0;  //reset flags
  CN0Flag = 0;
  CN30Flag = 0;
}*/

