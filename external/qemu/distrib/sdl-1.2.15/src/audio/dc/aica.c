#include "aica.h"

#include <arch/irq.h>
#include <dc/spu.h>

 
#define dc_snd_base ((volatile unsigned char *)0xa0700000) 

#define	SNDREGADDR(x)	(0xa0700000 + (x))
#define	CHNREGADDR(ch,x)	SNDREGADDR(0x80*(ch)+(x))


#define SNDREG32(x)	(*(volatile unsigned long *)SNDREGADDR(x))
#define SNDREG8(x)	(*(volatile unsigned char *)SNDREGADDR(x))
#define CHNREG32(ch, x) (*(volatile unsigned long *)CHNREGADDR(ch,x))
#define CHNREG8(ch, x)	(*(volatile unsigned long *)CHNREGADDR(ch,x))

#define G2_LOCK(OLD) \
	do { \
		if (!irq_inside_int()) \
			OLD = irq_disable(); \
		 \
		while((*(volatile unsigned int *)0xa05f688c) & 0x20) \
			; \
	} while(0)

#define G2_UNLOCK(OLD) \
	do { \
		 \
		if (!irq_inside_int()) \
			irq_restore(OLD); \
	} while(0)


void aica_init() {
	int i, j, old = 0;
	
		
	G2_LOCK(old);
	SNDREG32(0x2800) = 0x0000;
	
	for (i=0; i<64; i++) {
		for (j=0; j<0x80; j+=4) {
			if ((j&31)==0) g2_fifo_wait();
			CHNREG32(i, j) = 0;
		}
		g2_fifo_wait();
		CHNREG32(i,0) = 0x8000;
		CHNREG32(i,20) = 0x1f;
	}

	SNDREG32(0x2800) = 0x000f;
	g2_fifo_wait();
	G2_UNLOCK(old);
}


const static unsigned char logs[] = {
	0, 15, 22, 27, 31, 35, 39, 42, 45, 47, 50, 52, 55, 57, 59, 61,
	63, 65, 67, 69, 71, 73, 74, 76, 78, 79, 81, 82, 84, 85, 87, 88,
	90, 91, 92, 94, 95, 96, 98, 99, 100, 102, 103, 104, 105, 106,
	108, 109, 110, 111, 112, 113, 114, 116, 117, 118, 119, 120, 121,
	122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134,
	135, 136, 137, 138, 138, 139, 140, 141, 142, 143, 144, 145, 146,
	146, 147, 148, 149, 150, 151, 152, 152, 153, 154, 155, 156, 156,
	157, 158, 159, 160, 160, 161, 162, 163, 164, 164, 165, 166, 167,
	167, 168, 169, 170, 170, 171, 172, 173, 173, 174, 175, 176, 176,
	177, 178, 178, 179, 180, 181, 181, 182, 183, 183, 184, 185, 185,
	186, 187, 187, 188, 189, 189, 190, 191, 191, 192, 193, 193, 194,
	195, 195, 196, 197, 197, 198, 199, 199, 200, 200, 201, 202, 202,
	203, 204, 204, 205, 205, 206, 207, 207, 208, 209, 209, 210, 210,
	211, 212, 212, 213, 213, 214, 215, 215, 216, 216, 217, 217, 218,
	219, 219, 220, 220, 221, 221, 222, 223, 223, 224, 224, 225, 225,
	226, 227, 227, 228, 228, 229, 229, 230, 230, 231, 232, 232, 233,
	233, 234, 234, 235, 235, 236, 236, 237, 237, 238, 239, 239, 240,
	240, 241, 241, 242, 242, 243, 243, 244, 244, 245, 245, 246, 246,
	247, 247, 248, 248, 249, 249, 250, 250, 251, 251, 252, 252, 253, 254, 255
};

#define AICA_PAN(x) ((x)==0x80?(0):((x)<0x80?(0x1f):(0x0f)))
#define AICA_VOL(x) (0xff - logs[128 + (((x) & 0xff) / 2)])

static inline unsigned  AICA_FREQ(unsigned freq)	{
	unsigned long freq_lo, freq_base = 5644800;
	int freq_hi = 7;

	while (freq < freq_base && freq_hi > -8) {
		freq_base >>= 1;
		--freq_hi;
	}
	while (freq < freq_base && freq_hi > -8) {
		freq_base >>= 1;
		freq_hi--;
	}
	freq_lo = (freq<<10) / freq_base;
	return (freq_hi << 11) | (freq_lo & 1023);
}

void aica_play(int ch,int mode,unsigned long smpptr,int loopst,int loopend,int freq,int vol,int pan,int loopflag) {
	int val;
	int old = 0;

	
	aica_stop(ch);
	
	G2_LOCK(old);
	CHNREG32(ch, 8) = loopst & 0xffff;
	CHNREG32(ch, 12) = loopend & 0xffff;
	
	
	CHNREG32(ch, 24) = AICA_FREQ(freq);
	
	CHNREG32(ch, 36) = AICA_PAN(pan) | (0xf<<8);
	
	
	vol = AICA_VOL(vol);
	CHNREG32(ch, 40) = 0x24 | (vol<<8);
	
	

	CHNREG32(ch, 16) = 0x1f;	
	
	
	CHNREG32(ch, 4) = smpptr & 0xffff;
	val = 0xc000 | 0x0000 | (mode<<7) | (smpptr >> 16);
	if (loopflag) val|=0x200;
	
	CHNREG32(ch, 0) = val;
	
	G2_UNLOCK(old);

	
	
	g2_fifo_wait();

#if 0
	for (i=0xff; i>=vol; i--) {
		if ((i&7)==0) g2_fifo_wait();
		CHNREG32(ch, 40) =  0x24 | (i<<8);;
	}

	g2_fifo_wait();
#endif
}

void aica_stop(int ch) {
	g2_write_32(CHNREGADDR(ch, 0),(g2_read_32(CHNREGADDR(ch, 0)) & ~0x4000) | 0x8000);
	g2_fifo_wait();
}


   
void aica_vol(int ch,int vol) {
	g2_write_32(CHNREGADDR(ch, 40),(g2_read_32(CHNREGADDR(ch, 40))&0xffff00ff)|(AICA_VOL(vol)<<8) );
	g2_fifo_wait();
}

void aica_pan(int ch,int pan) {
	g2_write_32(CHNREGADDR(ch, 36),(g2_read_32(CHNREGADDR(ch, 36))&0xffffff00)|(AICA_PAN(pan)) );
	g2_fifo_wait();
}

void aica_freq(int ch,int freq) {
	g2_write_32(CHNREGADDR(ch, 24),AICA_FREQ(freq));
	g2_fifo_wait();
}

int aica_get_pos(int ch) {
#if 1
	
	g2_write_32(SNDREGADDR(0x280c),(g2_read_32(SNDREGADDR(0x280c))&0xffff00ff) | (ch<<8));
	g2_fifo_wait();
	
	return g2_read_32(SNDREGADDR(0x2814)) & 0xffff;
#else
	
	g2_write_8(SNDREGADDR(0x280d),ch);
	
	return g2_read_32(SNDREGADDR(0x2814)) & 0xffff;
#endif
}
