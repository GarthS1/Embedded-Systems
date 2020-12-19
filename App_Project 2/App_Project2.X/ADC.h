// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ADC_H
#define	ADC_H

#include <xc.h> // include processor files - each processor file is guarded.  

void doADC(int);
void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */



#ifdef	__cplusplus
}
#endif 

#endif	
