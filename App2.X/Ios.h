// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef IOS_H
#define	IOS_H

#include <xc.h> // include processor files - each processor file is guarded.  
//This function initializes IO ports.
void IOinit(void);
//This function implements the IO checks and LED blinking functions
void IOcheck(value);

void CNinit(void);

#endif	/* XC_HEADER_TEMPLATE_H */

