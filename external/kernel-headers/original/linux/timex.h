/*****************************************************************************
 *                                                                           *
 * Copyright (c) David L. Mills 1993                                         *
 *                                                                           *
 * Permission to use, copy, modify, and distribute this software and its     *
 * documentation for any purpose and without fee is hereby granted, provided *
 * that the above copyright notice appears in all copies and that both the   *
 * copyright notice and this permission notice appear in supporting          *
 * documentation, and that the name University of Delaware not be used in    *
 * advertising or publicity pertaining to distribution of the software       *
 * without specific, written prior permission.  The University of Delaware   *
 * makes no representations about the suitability this software for any      *
 * purpose.  It is provided "as is" without express or implied warranty.     *
 *                                                                           *
 *****************************************************************************/

#ifndef _LINUX_TIMEX_H
#define _LINUX_TIMEX_H

#include <linux/compiler.h>
#include <linux/time.h>

#include <asm/param.h>

#define SHIFT_KG 6		
#define SHIFT_KF 16		
#define SHIFT_KH 2		
#define MAXTC 6			

#define SHIFT_SCALE 22		
#define SHIFT_UPDATE (SHIFT_KG + MAXTC) 
#define SHIFT_USEC 16		
#define FINENSEC (1L << (SHIFT_SCALE - 10)) 

#define MAXPHASE 512000L        
#define MAXFREQ (512L << SHIFT_USEC)  
#define MINSEC 16L              
#define MAXSEC 1200L            
#define	NTP_PHASE_LIMIT	(MAXPHASE << 5)	

struct timex {
	unsigned int modes;	
	long offset;		
	long freq;		
	long maxerror;		
	long esterror;		
	int status;		
	long constant;		
	long precision;		
	long tolerance;		
	struct timeval time;	
	long tick;		

	long ppsfreq;           
	long jitter;            
	int shift;              
	long stabil;            
	long jitcnt;            
	long calcnt;            
	long errcnt;            
	long stbcnt;            

	int  :32; int  :32; int  :32; int  :32;
	int  :32; int  :32; int  :32; int  :32;
	int  :32; int  :32; int  :32; int  :32;
};

#define ADJ_OFFSET		0x0001	
#define ADJ_FREQUENCY		0x0002	
#define ADJ_MAXERROR		0x0004	
#define ADJ_ESTERROR		0x0008	
#define ADJ_STATUS		0x0010	
#define ADJ_TIMECONST		0x0020	
#define ADJ_TICK		0x4000	
#define ADJ_OFFSET_SINGLESHOT	0x8001	

#define MOD_OFFSET	ADJ_OFFSET
#define MOD_FREQUENCY	ADJ_FREQUENCY
#define MOD_MAXERROR	ADJ_MAXERROR
#define MOD_ESTERROR	ADJ_ESTERROR
#define MOD_STATUS	ADJ_STATUS
#define MOD_TIMECONST	ADJ_TIMECONST
#define MOD_CLKB	ADJ_TICK
#define MOD_CLKA	ADJ_OFFSET_SINGLESHOT 


#define STA_PLL		0x0001	
#define STA_PPSFREQ	0x0002	
#define STA_PPSTIME	0x0004	
#define STA_FLL		0x0008	

#define STA_INS		0x0010	
#define STA_DEL		0x0020	
#define STA_UNSYNC	0x0040	
#define STA_FREQHOLD	0x0080	

#define STA_PPSSIGNAL	0x0100	
#define STA_PPSJITTER	0x0200	
#define STA_PPSWANDER	0x0400	
#define STA_PPSERROR	0x0800	

#define STA_CLOCKERR	0x1000	

#define STA_RONLY (STA_PPSSIGNAL | STA_PPSJITTER | STA_PPSWANDER | \
    STA_PPSERROR | STA_CLOCKERR) 

#define TIME_OK		0	
#define TIME_INS	1	
#define TIME_DEL	2	
#define TIME_OOP	3	
#define TIME_WAIT	4	
#define TIME_ERROR	5	
#define TIME_BAD	TIME_ERROR 

#ifdef __KERNEL__
#include <asm/timex.h>

extern unsigned long tick_usec;		
extern unsigned long tick_nsec;		
extern int tickadj;			

extern int time_state;		
extern int time_status;		
extern long time_offset;	
extern long time_constant;	
extern long time_tolerance;	
extern long time_precision;	
extern long time_maxerror;	
extern long time_esterror;	

extern long time_freq;		
extern long time_reftime;	

extern long time_adjust;	
extern long time_next_adjust;	

static inline void ntp_clear(void)
{
	time_adjust = 0;		
	time_status |= STA_UNSYNC;
	time_maxerror = NTP_PHASE_LIMIT;
	time_esterror = NTP_PHASE_LIMIT;
}

static inline int ntp_synced(void)
{
	return !(time_status & STA_UNSYNC);
}

#define shift_right(x, s) ({	\
	__typeof__(x) __x = (x);	\
	__typeof__(s) __s = (s);	\
	__x < 0 ? -(-__x >> __s) : __x >> __s;	\
})


#ifdef CONFIG_TIME_INTERPOLATION

#define TIME_SOURCE_CPU 0
#define TIME_SOURCE_MMIO64 1
#define TIME_SOURCE_MMIO32 2
#define TIME_SOURCE_FUNCTION 3


struct time_interpolator {
	u16 source;			
	u8 shift;			
				
	u8 jitter;			
	u32 nsec_per_cyc;		
	void *addr;			
	u64 mask;			
	unsigned long offset;		
	u64 last_counter;		
	u64 last_cycle;			
	u64 frequency;			
	long drift;			
	unsigned long skips;		
	unsigned long ns_skipped;	
	struct time_interpolator *next;
};

extern void register_time_interpolator(struct time_interpolator *);
extern void unregister_time_interpolator(struct time_interpolator *);
extern void time_interpolator_reset(void);
extern unsigned long time_interpolator_get_offset(void);

#else 

static inline void
time_interpolator_reset(void)
{
}

#endif 

#define TICK_LENGTH_SHIFT	32

extern u64 current_tick_length(void);

extern int do_adjtimex(struct timex *);

#endif 

#endif 
