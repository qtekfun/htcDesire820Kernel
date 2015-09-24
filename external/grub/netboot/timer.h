
 /*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2, or (at
 * your option) any later version.
 */

#ifndef	TIMER_H
#define TIMER_H

#define	TIMER2_PORT	0x42
#define	TIMER_MODE_PORT	0x43

#define	TIMER0_SEL	0x00
#define	TIMER1_SEL	0x40
#define	TIMER2_SEL	0x80
#define	READBACK_SEL	0xC0

#define	LATCH_COUNT	0x00
#define	LOBYTE_ACCESS	0x10
#define	HIBYTE_ACCESS	0x20
#define	WORD_ACCESS	0x30

#define	MODE0		0x00
#define	MODE1		0x02
#define	MODE2		0x04
#define	MODE3		0x06
#define	MODE4		0x08
#define	MODE5		0x0A

#define	BINARY_COUNT	0x00
#define	BCD_COUNT	0x01

#define	TICKS_PER_MS	1193

#define	PPC_PORTB	0x61

#define	PPCB_T2OUT	0x20	
#define	PPCB_SPKR	0x02	
#define	PPCB_T2GATE	0x01	

extern void load_timer2(unsigned int ticks);
extern inline int timer2_running(void)
{
	return ((inb(PPC_PORTB) & PPCB_T2OUT) == 0);
}

extern inline void waiton_timer2(unsigned int ticks)
{
	load_timer2(ticks);
	while ((inb(PPC_PORTB) & PPCB_T2OUT) == 0)
		;
}

#endif	
