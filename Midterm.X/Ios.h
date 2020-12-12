// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef IOS_H
#define	IOS_H

#include <xc.h> // include processor files - each processor file is guarded.  
//This function initializes IO ports.
void IOinit(void);
//This function implements the IO checks and LED blinking functions
void IOcheck(void);

void CNinit(void);

void displayVoltage(uint16_t value);

void displayResistance(uint16_t value);
extern unsigned int clock_val;
#endif	/* XC_HEADER_TEMPLATE_H */

