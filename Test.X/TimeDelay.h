#ifndef TIMEDELAY_H
#define	TIMEDELAY_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

void delay_ms(uint16_t, uint8_t);
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);
void startTimer();
void configureTimer3();
void configureTimer1();
#endif	/* CHANGECLK_H */

