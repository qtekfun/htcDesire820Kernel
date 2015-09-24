/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1994 Waldorf GMBH
 * Copyright (C) 1995, 1996, 1997, 1998, 1999, 2001, 2002, 2003 Ralf Baechle
 * Copyright (C) 1996 Paul M. Antoine
 * Copyright (C) 1999, 2000 Silicon Graphics, Inc.
 */
#ifndef _ASM_PROCESSOR_H
#define _ASM_PROCESSOR_H

#include <linux/cpumask.h>
#include <linux/threads.h>

#include <asm/cachectl.h>
#include <asm/cpu.h>
#include <asm/cpu-info.h>
#include <asm/mipsregs.h>
#include <asm/prefetch.h>
#include <asm/system.h>

#define current_text_addr() ({ __label__ _l; _l: &&_l;})

extern void (*cpu_wait)(void);

extern unsigned int vced_count, vcei_count;

#ifdef CONFIG_32BIT
#define TASK_SIZE	0x7fff8000UL
#define STACK_TOP	TASK_SIZE

#define TASK_UNMAPPED_BASE	((TASK_SIZE / 3) & ~(PAGE_SIZE))
#endif

#ifdef CONFIG_64BIT
#define TASK_SIZE32	0x7fff8000UL
#define TASK_SIZE	0x10000000000UL
#define STACK_TOP	\
      (test_thread_flag(TIF_32BIT_ADDR) ? TASK_SIZE32 : TASK_SIZE)

#define TASK_UNMAPPED_BASE						\
	(test_thread_flag(TIF_32BIT_ADDR) ?				\
		PAGE_ALIGN(TASK_SIZE32 / 3) : PAGE_ALIGN(TASK_SIZE / 3))
#define TASK_SIZE_OF(tsk)						\
	(test_tsk_thread_flag(tsk, TIF_32BIT_ADDR) ? TASK_SIZE32 : TASK_SIZE)
#endif

#ifdef __KERNEL__
#define STACK_TOP_MAX	TASK_SIZE
#endif

#define NUM_FPU_REGS	32

typedef __u64 fpureg_t;


struct mips_fpu_struct {
	fpureg_t	fpr[NUM_FPU_REGS];
	unsigned int	fcr31;
};

#define NUM_DSP_REGS   6

typedef __u32 dspreg_t;

struct mips_dsp_state {
	dspreg_t        dspr[NUM_DSP_REGS];
	unsigned int    dspcontrol;
};

#define INIT_CPUMASK { \
	{0,} \
}

typedef struct {
	unsigned long seg;
} mm_segment_t;

#define ARCH_MIN_TASKALIGN	8

struct mips_abi;

struct thread_struct {
	
	unsigned long reg16;
	unsigned long reg17, reg18, reg19, reg20, reg21, reg22, reg23;
	unsigned long reg29, reg30, reg31;

	
	unsigned long cp0_status;

	
	struct mips_fpu_struct fpu;
#ifdef CONFIG_MIPS_MT_FPAFF
	
	unsigned long emulated_fp;
	
	cpumask_t user_cpus_allowed;
#endif 

	
	struct mips_dsp_state dsp;

	
	unsigned long cp0_badvaddr;	
	unsigned long cp0_baduaddr;	
	unsigned long error_code;
	unsigned long trap_no;
	unsigned long irix_trampoline;  
	unsigned long irix_oldctx;
	struct mips_abi *abi;
};

#ifdef CONFIG_MIPS_MT_FPAFF
#define FPAFF_INIT						\
	.emulated_fp			= 0,			\
	.user_cpus_allowed		= INIT_CPUMASK,
#else
#define FPAFF_INIT
#endif 

#define INIT_THREAD  {						\
							\
	.reg16			= 0,				\
	.reg17			= 0,				\
	.reg18			= 0,				\
	.reg19			= 0,				\
	.reg20			= 0,				\
	.reg21			= 0,				\
	.reg22			= 0,				\
	.reg23			= 0,				\
	.reg29			= 0,				\
	.reg30			= 0,				\
	.reg31			= 0,				\
							\
	.cp0_status		= 0,				\
							\
	.fpu			= {				\
		.fpr		= {0,},				\
		.fcr31		= 0,				\
	},							\
							\
	FPAFF_INIT						\
							\
	.dsp			= {				\
		.dspr		= {0, },			\
		.dspcontrol	= 0,				\
	},							\
							\
	.cp0_badvaddr		= 0,				\
	.cp0_baduaddr		= 0,				\
	.error_code		= 0,				\
	.trap_no		= 0,				\
	.irix_trampoline	= 0,				\
	.irix_oldctx		= 0,				\
}

struct task_struct;

#define release_thread(thread) do { } while(0)

#define prepare_to_copy(tsk)	do { } while (0)

extern long kernel_thread(int (*fn)(void *), void * arg, unsigned long flags);

extern unsigned long thread_saved_pc(struct task_struct *tsk);

extern void start_thread(struct pt_regs * regs, unsigned long pc, unsigned long sp);

unsigned long get_wchan(struct task_struct *p);

#define __KSTK_TOS(tsk) ((unsigned long)task_stack_page(tsk) + THREAD_SIZE - 32)
#define task_pt_regs(tsk) ((struct pt_regs *)__KSTK_TOS(tsk) - 1)
#define KSTK_EIP(tsk) (task_pt_regs(tsk)->cp0_epc)
#define KSTK_ESP(tsk) (task_pt_regs(tsk)->regs[29])
#define KSTK_STATUS(tsk) (task_pt_regs(tsk)->cp0_status)

#define cpu_relax()	barrier()

#define return_address() ({__asm__ __volatile__("":::"$31");__builtin_return_address(0);})

#ifdef CONFIG_CPU_HAS_PREFETCH

#define ARCH_HAS_PREFETCH

static inline void prefetch(const void *addr)
{
	__asm__ __volatile__(
	"	.set	mips4		\n"
	"	pref	%0, (%1)	\n"
	"	.set	mips0		\n"
	:
	: "i" (Pref_Load), "r" (addr));
}

#endif

#endif 
