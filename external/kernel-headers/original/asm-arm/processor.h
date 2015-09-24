/*
 *  linux/include/asm-arm/processor.h
 *
 *  Copyright (C) 1995-1999 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ASM_ARM_PROCESSOR_H
#define __ASM_ARM_PROCESSOR_H

#define current_text_addr() ({ __label__ _l; _l: &&_l;})

#ifdef __KERNEL__

#include <asm/ptrace.h>
#include <asm/procinfo.h>
#include <asm/types.h>

union debug_insn {
	u32	arm;
	u16	thumb;
};

struct debug_entry {
	u32			address;
	union debug_insn	insn;
};

struct debug_info {
	int			nsaved;
	struct debug_entry	bp[2];
};

struct thread_struct {
							
	unsigned long		address;
	unsigned long		trap_no;
	unsigned long		error_code;
							
	struct debug_info	debug;
};

#define INIT_THREAD  {	}

#ifdef CONFIG_MMU
#define nommu_start_thread(regs) do { } while (0)
#else
#define nommu_start_thread(regs) regs->ARM_r10 = current->mm->start_data
#endif

#define start_thread(regs,pc,sp)					\
({									\
	unsigned long *stack = (unsigned long *)sp;			\
	set_fs(USER_DS);						\
	memzero(regs->uregs, sizeof(regs->uregs));			\
	if (current->personality & ADDR_LIMIT_32BIT)			\
		regs->ARM_cpsr = USR_MODE;				\
	else								\
		regs->ARM_cpsr = USR26_MODE;				\
	if (elf_hwcap & HWCAP_THUMB && pc & 1)				\
		regs->ARM_cpsr |= PSR_T_BIT;				\
	regs->ARM_pc = pc & ~1;					\
	regs->ARM_sp = sp;					\
	regs->ARM_r2 = stack[2];				\
	regs->ARM_r1 = stack[1];				\
	regs->ARM_r0 = stack[0];				\
	nommu_start_thread(regs);					\
})

struct task_struct;

extern void release_thread(struct task_struct *);

#define prepare_to_copy(tsk)	do { } while (0)

unsigned long get_wchan(struct task_struct *p);

#define cpu_relax()			barrier()

extern int kernel_thread(int (*fn)(void *), void *arg, unsigned long flags);

#define task_pt_regs(p) \
	((struct pt_regs *)(THREAD_START_SP + task_stack_page(p)) - 1)

#define KSTK_EIP(tsk)	task_pt_regs(tsk)->ARM_pc
#define KSTK_ESP(tsk)	task_pt_regs(tsk)->ARM_sp

#if __LINUX_ARM_ARCH__ >= 5

#define ARCH_HAS_PREFETCH
#define prefetch(ptr)				\
	({					\
		__asm__ __volatile__(		\
		"pld\t%0"			\
		:				\
		: "o" (*(char *)(ptr))		\
		: "cc");			\
	})

#define ARCH_HAS_PREFETCHW
#define prefetchw(ptr)	prefetch(ptr)

#define ARCH_HAS_SPINLOCK_PREFETCH
#define spin_lock_prefetch(x) do { } while (0)

#endif

#endif

#endif 