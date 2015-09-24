/**
 * @file compat22.h
 * Compatability functions for 2.2 kernels
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef COMPAT22_H
#define COMPAT22_H

#include <linux/smp_lock.h>

#define local_irq_disable()	__cli()
#define local_irq_enable()	__sti()
#define pte_page_address(a) pte_page(a)

#define GET_VM_OFFSET(v) ((v)->vm_offset)
#define MODULE_LICENSE(l)
#define NEED_2_2_DENTRIES
#define INC_USE_COUNT_MAYBE MOD_INC_USE_COUNT
#define DEC_USE_COUNT_MAYBE MOD_DEC_USE_COUNT
#define lock_execve lock_kernel
#define unlock_execve unlock_kernel

#define lock_mmap(mm) do {} while (0)
#define unlock_mmap(mm) do {} while (0)

static inline pid_t op_get_tgid(void)
{
	return current->pid;
}

static inline void oprof_wake_up(struct wait_queue **q)
{
	unsigned long flags;
	save_flags(flags);
	cli();
	wake_up(q);
	restore_flags(flags);
}

extern int wind_dentries_2_2(struct dentry * dentry);
extern uint do_path_hash_2_2(struct dentry * dentry);
#define wind_dentries(d, v, r, m) wind_dentries_2_2(d)
#define hash_path(f) do_path_hash_2_2((f)->f_dentry)

static inline void lock_out_mmap(void)
{
	lock_kernel();
	down(&current->mm->mmap_sem);
}

static inline void unlock_out_mmap(void)
{
	unlock_kernel();
	up(&current->mm->mmap_sem);
}

#define request_region_check compat_request_region
void * compat_request_region (unsigned long start, unsigned long n, char const * name);

#define __exit

#define virt_to_page(va) MAP_NR(va)

#ifdef CONFIG_SMP
	#define op_cpu_id() cpu_number_map[smp_processor_id()]
#else
	#define op_cpu_id() smp_processor_id()
#endif 

#if !defined(CONFIG_SMP)

	#undef smp_call_function
	static int inline smp_call_function (void (*func) (void * info), void * info,
					     int retry, int wait)
	{
		return 0;
	}

#endif 

#if V_BEFORE(2, 2, 18)

	
	
	#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 91)
		#define module_init(x)	int init_module(void) __attribute__((alias(#x)));
		#define module_exit(x)	void cleanup_module(void) __attribute__((alias(#x)));
	#else
		#define module_init(x)	int init_module(void) { return x(); }
		#define module_exit(x)	void cleanup_module(void) { x(); }
	#endif

	
	#define vmalloc_32 vmalloc

	
	#define DECLARE_WAIT_QUEUE_HEAD(q) struct wait_queue * q = NULL
	#define DECLARE_MUTEX(foo)	struct semaphore foo = MUTEX

	
	#define THIS_MODULE (&__this_module)

	#define BUG() __asm__ __volatile__("ud2\n");

#endif 

#ifdef RTC_LOCK
	#define lock_rtc(f) spin_lock_irqsave(&rtc_lock, f)
	#define unlock_rtc(f) spin_unlock_irqrestore(&rtc_lock, f)
#else
	#define lock_rtc(f) do { save_flags(f); cli(); } while (0)
	#define unlock_rtc(f) restore_flags(f)
#endif 

#if V_AT_LEAST(2, 2, 20)
	#define PTRACE_OFF(t) ((t)->ptrace &= ~PT_DTRACE)
#else
	#define PTRACE_OFF(t) ((t)->flags &= ~PF_DTRACE)
#endif

#if V_BEFORE(2, 2, 21)
static inline unsigned int cpuid_edx(unsigned int op)
{
	unsigned int eax, edx;

	__asm__("cpuid"
		: "=a" (eax), "=d" (edx)
		: "0" (op)
		: "bx", "cx");
	return edx;
}
#endif

#endif 
