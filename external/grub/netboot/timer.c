
 /*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2, or (at
 * your option) any later version.
 */

#include	"etherboot.h"
#include	"timer.h"

void load_timer2(unsigned int ticks)
{
	
	outb((inb(PPC_PORTB) & ~PPCB_SPKR) | PPCB_T2GATE, PPC_PORTB);
	outb(TIMER2_SEL|WORD_ACCESS|MODE0|BINARY_COUNT, TIMER_MODE_PORT);
	outb(ticks & 0xFF, TIMER2_PORT);
	outb(ticks >> 8, TIMER2_PORT);
}

#if defined(CONFIG_TSC_CURRTICKS)
#define rdtsc(low,high) \
     __asm__ __volatile__("rdtsc" : "=a" (low), "=d" (high))

#define rdtscll(val) \
     __asm__ __volatile__ ("rdtsc" : "=A" (val))


#define HZ TICKS_PER_SEC
#define CLOCK_TICK_RATE	1193180U 
#define LATCH  ((CLOCK_TICK_RATE + HZ/2) / HZ)	



#define CALIBRATE_LATCH	(5 * LATCH)

static unsigned long long calibrate_tsc(void)
{
	
	outb((inb(0x61) & ~0x02) | 0x01, 0x61);

	outb(0xb0, 0x43);			
	outb(CALIBRATE_LATCH & 0xff, 0x42);	
	outb(CALIBRATE_LATCH >> 8, 0x42);	

	{
		unsigned long startlow, starthigh;
		unsigned long endlow, endhigh;
		unsigned long count;

		rdtsc(startlow,starthigh);
		count = 0;
		do {
			count++;
		} while ((inb(0x61) & 0x20) == 0);
		rdtsc(endlow,endhigh);

		
		if (count <= 1)
			goto bad_ctc;

		
		__asm__("subl %2,%0\n\t"
			"sbbl %3,%1"
			:"=a" (endlow), "=d" (endhigh)
			:"g" (startlow), "g" (starthigh),
			 "0" (endlow), "1" (endhigh));

		
		if (endhigh)
			goto bad_ctc;

		endlow /= 5;
		return endlow;
	}

bad_ctc:
	printf("bad_ctc\n");
	return 0;
}


unsigned long currticks(void)
{
	static unsigned long clocks_per_tick;
	unsigned long clocks_high, clocks_low;
	unsigned long currticks;
	if (!clocks_per_tick) {
		clocks_per_tick = calibrate_tsc();
		printf("clocks_per_tick = %d\n", clocks_per_tick);
	}

	
	rdtsc(clocks_low, clocks_high);

	
	__asm__("divl %1"
		:"=a" (currticks)
		:"r" (clocks_per_tick), "0" (clocks_low), "d" (clocks_high));


	return currticks;
}

#endif 
