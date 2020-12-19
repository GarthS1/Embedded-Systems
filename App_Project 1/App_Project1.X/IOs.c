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
#include "ChangeClk.h"

int seconds = 0;    //seconds on timer
int minutes = 0;    //minutes on timer
int countdown = 0;  //flag for countdown 


//This function initializes IO ports.
void IOinit() {
    AD1PCFG = 0xFFFF; // Turn all analog pins as digital
    
    TRISBbits.TRISB8 = 0; //RB8 is output.
    LATBbits.LATB8 = 0; // Make GPIO RB8 as a digital output
    
    CNPU1bits.CN0PUE = 1; //Enables pull up resistor on RA4/CN0
    CNPU2bits.CN30PUE = 1; //Enables pull up resistor on RA2/CN3
    CNPU1bits.CN1PUE = 1; //Enables pull up resistor on RB4/CN1
    
    TRISAbits.TRISA2 = 1; //RA2 is input, PB1 is connected to this port.
    TRISAbits.TRISA4 = 1; //RA4 is input, PB2 is connected to this port.
    TRISBbits.TRISB4 = 1; //RB4 is input, PB3 is connected to this port
    
    CNinit();
}

//This function initializes Cn interrupts 
void CNinit() {
    CNEN1bits.CN1IE = 1;    //Configure change of notification input of RB4
    CNEN1bits.CN0IE = 1;    //Configure change of notification input of RA4
    CNEN2bits.CN30IE = 1;   //Configure change of notification input of RA2
    
    IPC4bits.CNIP0 = 6;     //Change in IO Notifcation priority 6.
    IEC1bits.CNIE = 1;      //Set interrupt request to occur
    IFS1bits.CNIF = 0;      // Clear interrupt flag
}

//CN interrupt routine 
void IOcheck()
{
    IEC1bits.CNIE = 0; //disable CN interrupts to avoid debounces
    delay_ms(400, 1);   // 400 msec delay to filter out debounces 
    IEC1bits.CNIE = 1; //Enable CN interrupts to detect pb release
    
    while(PORTAbits.RA2 == 0 && PORTBbits.RB4 == 1 && PORTAbits.RA4 == 1)   //If PB1 is pressed 
    {
        countdown = 0;    //disable countdown
        if(minutes < 59)
        {
            minutes++;       //increment the minute count by 1
            displayTime();  //call displaytime function
        }
        delay_ms(1000, 1);   // 1 sec delay
        
    }
    
    while(PORTAbits.RA4 == 0 && PORTBbits.RB4 == 1 && PORTAbits.RA2 == 1)  //IF PB2 is pressed 
    {   
        countdown = 0;    //disable countdown
        if(minutes<59)
        {
            seconds++;        //increment the second count by 1
            displayTime();    //call displaytime function
        }  
        delay_ms(1000, 1);   // 1 sec delay
    }
    
    int timePB3 = 0;  //keeps track of how long PB3 has been pushed down
    int PB3flag = 0;  //flag for button 3
    while(PORTBbits.RB4 == 0 && PORTAbits.RA4 == 1 && PORTAbits.RA2 == 1)   //IF PB3 is pressed 
    {   
        PB3flag = 1;         //Enable flag for PB3
        delay_ms(1000, 1);   // 1 sec delay
        timePB3++;           //Increment timer
    }
    
    if(PB3flag)
    {
        if(timePB3 >= 3) //If button has been pushed down for greater then 3 seconds 
        {
            //Reset timer and display
            countdown = 0; 
            seconds = 0;
            minutes = 0;
            displayTime();
        }
        else  //Button pushed for less then 3 seconds 
        {
          countdown = 1 - countdown;  //Start or pause countdown depending on previous state 
        }
    }
}

//Interrupt routine for _CNInterrupt
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{
    IFS1bits.CNIF = 0;		// clear IF flag
    T2CONbits.TON = 0;    // Disable timer
    IEC0bits.T2IE = 0;    //Disable timer interrupt
    IOcheck();
    Nop();	 
}

//Function to show timer if set 
void counterdownTimer() {
  if(countdown){
    if(seconds == 0 && minutes == 0){ //Timer done
        alarm();
        countdown = 0;
        LATBbits.LATB8 = 1; //Turn LED on 
    }
    else //Timer on
    {   
        //Calculate time left 
        if(seconds == 0){
          minutes--;
          seconds = 59;
        }
        else{
          seconds--;
        }
        
        delay_ms(1000, 1);   // 1 sec delay
        displayTime();
        LATBbits.LATB8 = 1  - LATBbits.LATB8; //Blink LED
    }
  } 
}

//Show alarm message
void alarm() {
    NewClk(8); 
    Disp2String("\r");
    Disp2Dec(minutes);
    Disp2String("m : ");
    Disp2Dec(seconds);
    Disp2String("s");
    Disp2String(" --ALARM");
    NewClk(32);
}

//Display how much time is left 
void displayTime() {
    NewClk(8); 
    Disp2String("\r");
    Disp2Dec(minutes);
    Disp2String("m : ");
    Disp2Dec(seconds);
    Disp2String("s              ");
    NewClk(32);
}
/*
int CN30Flag = 0;   //RA2 push button flag
int CN0Flag = 0;    //RA4 push button flag
int CN1Flag = 0;    //RB4 push button flag
int seconds = 0;    //seconds on timer
int minutes = 0;    //minutes on timer
int timer_flag = 0;
void pushButton1()
{
    if(minutes==0)
    {
        minutes++; //increment the minute count by 1
        displayTime(); //call displaytime function
    }
    delay_ms(1000); //delay 1s
    if(minutes<59)
    {
        minutes++; //increment the minute count by 1
        displayTime();//call displaytime function
    }
}

void pushButton2()
{
    if(seconds==0)
    {
        seconds++; //increment the second count by 1
        displayTime(); //call displaytime function
    }
    delay_ms(1000); //delay 1s
    if(seconds<59)
    {
        seconds++; //increment the second count by 1
        displayTime(); //call displaytime function
    }
}



void pushButton3() {
    int time = countTimer();
    if(time < 3)
        shortPresses(timer_flag);
    else
        resetTimer();
}


void displayTime(){
  Disp2String("\r")
  Disp2Dec(minutes);
  Disp2String("m : ");
  Disp2Dec(seconds);
  Disp2String("s");
}
void counterdownTimer() {
  if(timer){
    displayTime();
    delay_ms(1000);  
    minutes --;
    seconds --;
  } 
}

void resetTimer() {
    Disp2String("\r00m : 00s/t/t/t");  //Reset to 0.
}

void shortPresses() {
    timer_flag = 1 - timer_flag;
    if(timer_flag)
        startTimer();
}

void startTimer() {
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
*/


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