#ifndef TIMEDELAY_H
#define	TIMEDELAY_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

void delay_ms(uint16_t time_ms);
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);

#endif	/* CHANGECLK_H */

