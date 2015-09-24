
#include <linux/config.h>

#include "IA64entry.h"

#define rARPR		r31
#define rCRIFS		r30
#define rCRIPSR		r29
#define rCRIIP		r28
#define rARRSC		r27
#define rARPFS		r26
#define rARUNAT		r25
#define rARRNAT		r24
#define rARBSPSTORE	r23
#define rKRBS		r22
#define rB6		r21
#define rR1		r20


#define MINSTATE_START_SAVE_MIN_VIRT					\
						\
	dep r1=-1, r1, 61, 3;						\
		\
(pUser)	mov ar.rsc=0;							\
	;;								\
						\
(pUser)	addl rKRBS=IA64_RBS_OFFSET, r1;					\
(pUser)	mov rARRNAT=ar.rnat;						\
								\
(pKern) mov r1=sp;							\
	;;								\
					\
(pUser)	addl r1=IA64_STK_OFFSET-IA64_PT_REGS_SIZE, r1;			\
							\
(pUser)	mov rARBSPSTORE=ar.bspstore;					\
	;;								\
					\
(pKern) addl r1=-IA64_PT_REGS_SIZE, r1;					\
						\
(pUser)	mov ar.bspstore=rKRBS;						\
	;;								\
(pUser)	mov r18=ar.bsp;							\
			\
(pUser)	mov ar.rsc=0x3;

#define MINSTATE_END_SAVE_MIN_VIRT					\
				\
	or r13=r13, r14;						\
		\
	bsw.1;								\
	;;

#define MINSTATE_START_SAVE_MIN_PHYS					\
(pKern) movl sp=ia64_init_stack+IA64_STK_OFFSET-IA64_PT_REGS_SIZE;	\
		\
(pUser)	mov ar.rsc=0;							\
				\
(pUser)	addl rKRBS=IA64_RBS_OFFSET, r1;					\
	;;								\
(pUser)	mov rARRNAT=ar.rnat;						\
					\
(pKern) dep r1=0, sp, 61, 3;						\
					\
(pUser)	addl r1=IA64_STK_OFFSET-IA64_PT_REGS_SIZE, r1;			\
							\
(pUser)	mov rARBSPSTORE=ar.bspstore;					\
				\
(pUser)	dep rKRBS=-1, rKRBS, 61, 3;					\
	;;								\
					\
(pKern) addl r1=-IA64_PT_REGS_SIZE, r1;					\
						\
(pUser)	mov ar.bspstore=rKRBS;						\
	;;								\
(pUser)	mov r18=ar.bsp;							\
			\
(pUser)	mov ar.rsc=0x3;

#define MINSTATE_END_SAVE_MIN_PHYS					\
					\
	or r12=r12, r14;						\
				\
	or r13=r13, r14;						\
	;;

#ifdef MINSTATE_VIRT
# define MINSTATE_START_SAVE_MIN	MINSTATE_START_SAVE_MIN_VIRT
# define MINSTATE_END_SAVE_MIN		MINSTATE_END_SAVE_MIN_VIRT
#endif

#ifdef MINSTATE_PHYS
# define MINSTATE_START_SAVE_MIN	MINSTATE_START_SAVE_MIN_PHYS
# define MINSTATE_END_SAVE_MIN		MINSTATE_END_SAVE_MIN_PHYS
#endif

#define DO_SAVE_MIN(COVER, SAVE_IFS, EXTRA)				\
	mov rARRSC=ar.rsc;						\
	mov rARPFS=ar.pfs;						\
	mov rR1=r1;							\
	mov rARUNAT=ar.unat;						\
	mov rCRIPSR=cr.ipsr;						\
				  		\
	mov rB6=b6;							\
	mov rCRIIP=cr.iip;						\
				  		\
	mov r1=IA64_KR(CURRENT);					\
	COVER;								\
	;;								\
	invala;								\
					  		\
	extr.u r16=rCRIPSR, 32, 2;					\
	;;								\
	 		\
	cmp.eq pKern, pUser=r0, r16;					\
					\
	;;								\
	SAVE_IFS;							\
	MINSTATE_START_SAVE_MIN						\
	;;								\
		  			\
	mov r16=r1;							\
		  			\
	adds r17=8, r1;							\
	;;								\
	st8 [r16]=rCRIPSR, 16;				\
	st8 [r17]=rCRIIP, 16;				\
(pKern)	mov r18=r0;				\
	;;								\
	st8 [r16]=rCRIFS, 16;				\
	st8 [r17]=rARUNAT, 16;				\
(pUser)	sub r18=r18, rKRBS;				\
	;;								\
	st8 [r16]=rARPFS, 16;				\
	st8 [r17]=rARRSC, 16;				\
	tbit.nz p15, p0=rCRIPSR, IA64_PSR_I_BIT				\
	;;					\
(pKern)	adds r16=16, r16;			\
(pKern)	adds r17=16, r17;		\
(pUser)	st8 [r16]=rARRNAT, 16;				\
(pUser)	st8 [r17]=rARBSPSTORE, 16;			\
	;;								\
	st8 [r16]=rARPR, 16;				\
	st8 [r17]=rB6, 16;					\
				\
	shl r18=r18, 16;						\
	;;								\
	st8 [r16]=r18, 16;		\
	st8.spill [r17]=rR1, 16;			\
	;;								\
.mem.offset 0, 0;	st8.spill [r16]=r2, 16;				\
.mem.offset 8, 0;	st8.spill [r17]=r3, 16;				\
	adds r2=IA64_PT_REGS_R16_OFFSET, r1;				\
	;;								\
.mem.offset 0, 0;		st8.spill [r16]=r12, 16;		\
.mem.offset 8, 0;		st8.spill [r17]=r13, 16;		\
				  	\
	cmp.eq pNonSys, pSys=r0, r0					\
	;;								\
.mem.offset 0, 0;		st8.spill [r16]=r14, 16;		\
.mem.offset 8, 0;		st8.spill [r17]=r15, 16;		\
	dep r14=-1, r0, 61, 3;						\
	;;								\
.mem.offset 0, 0;		st8.spill [r16]=r8, 16;			\
.mem.offset 8, 0;		st8.spill [r17]=r9, 16;			\
		\
	adds r12=-16, r1;						\
	;;								\
.mem.offset 0, 0;		st8.spill [r16]=r10, 16;		\
.mem.offset 8, 0;		st8.spill [r17]=r11, 16;		\
	mov r13=IA64_KR(CURRENT);		\
	;;								\
	EXTRA;								\
	movl r1=__gp;			\
	;;								\
	MINSTATE_END_SAVE_MIN

#define SAVE_REST				\
.mem.offset 0, 0;	st8.spill [r2]=r16, 16;	\
.mem.offset 8, 0;	st8.spill [r3]=r17, 16;	\
	;;					\
.mem.offset 0, 0;	st8.spill [r2]=r18, 16;	\
.mem.offset 8, 0;	st8.spill [r3]=r19, 16;	\
	;;					\
	mov r16=ar.ccv;			\
	movl r18=FPSR_DEFAULT		\
	;;					\
	mov r17=ar.fpsr;		\
	mov ar.fpsr=r18;		\
	;;					\
.mem.offset 0, 0;	st8.spill [r2]=r20, 16;	\
.mem.offset 8, 0;	st8.spill [r3]=r21, 16;	\
	mov r18=b0;				\
	;;					\
.mem.offset 0, 0;	st8.spill [r2]=r22, 16;	\
.mem.offset 8, 0;	st8.spill [r3]=r23, 16;	\
	mov r19=b7;				\
	;;					\
.mem.offset 0, 0;	st8.spill [r2]=r24, 16;	\
.mem.offset 8, 0;	st8.spill [r3]=r25, 16;	\
	;;					\
.mem.offset 0, 0;	st8.spill [r2]=r26, 16;	\
.mem.offset 8, 0;	st8.spill [r3]=r27, 16;	\
	;;					\
.mem.offset 0, 0;	st8.spill [r2]=r28, 16;	\
.mem.offset 8, 0;	st8.spill [r3]=r29, 16;	\
	;;					\
.mem.offset 0, 0;	st8.spill [r2]=r30, 16;	\
.mem.offset 8, 0;	st8.spill [r3]=r31, 16;	\
	;;					\
	st8 [r2]=r16, 16;		\
	st8 [r3]=r17, 16;		\
	;;					\
	st8 [r2]=r18, 16;		\
	st8 [r3]=r19, 16+8;		\
	;;					\
	stf.spill [r2]=f6, 32;			\
	stf.spill [r3]=f7, 32;			\
	;;					\
	stf.spill [r2]=f8, 32;			\
	stf.spill [r3]=f9, 32

#define SAVE_MIN_WITH_COVER	DO_SAVE_MIN(cover, mov rCRIFS=cr.ifs,)
#define SAVE_MIN_WITH_COVER_R19	\
	DO_SAVE_MIN(cover, mov rCRIFS=cr.ifs, mov r15=r19)
#define SAVE_MIN		DO_SAVE_MIN(, mov rCRIFS=r0,)
