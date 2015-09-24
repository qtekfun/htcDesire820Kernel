/* thread_info.h: i386 low-level thread information
 *
 * Copyright (C) 2002  David Howells (dhowells@redhat.com)
 * - Incorporating suggestions made by Linus Torvalds and Dave Miller
 */

#ifndef _ASM_THREAD_INFO_H
#define _ASM_THREAD_INFO_H

#ifdef __KERNEL__

#include <linux/compiler.h>
#include <asm/page.h>

#ifndef __ASSEMBLY__
#include <asm/processor.h>
#endif

#ifndef __ASSEMBLY__

struct thread_info {
	struct task_struct	*task;		
	struct exec_domain	*exec_domain;	
	unsigned long		flags;		
	unsigned long		status;		
	__u32			cpu;		
	int			preempt_count;	


	mm_segment_t		addr_limit;	
	void			*sysenter_return;
	struct restart_block    restart_block;

	unsigned long           previous_esp;   
	__u8			supervisor_stack[0];
};

#else 

#include <asm/asm-offsets.h>

#endif

#define PREEMPT_ACTIVE		0x10000000
#ifdef CONFIG_4KSTACKS
#define THREAD_SIZE            (4096)
#else
#define THREAD_SIZE		(8192)
#endif

#define STACK_WARN             (THREAD_SIZE/8)
#ifndef __ASSEMBLY__

#define INIT_THREAD_INFO(tsk)			\
{						\
	.task		= &tsk,			\
	.exec_domain	= &default_exec_domain,	\
	.flags		= 0,			\
	.cpu		= 0,			\
	.preempt_count	= 1,			\
	.addr_limit	= KERNEL_DS,		\
	.restart_block = {			\
		.fn = do_no_restart_syscall,	\
	},					\
}

#define init_thread_info	(init_thread_union.thread_info)
#define init_stack		(init_thread_union.stack)


register unsigned long current_stack_pointer asm("esp") __attribute_used__;

static inline struct thread_info *current_thread_info(void)
{
	return (struct thread_info *)(current_stack_pointer & ~(THREAD_SIZE - 1));
}

#ifdef CONFIG_DEBUG_STACK_USAGE
#define alloc_thread_info(tsk) ((struct thread_info *) \
	__get_free_pages(GFP_KERNEL| __GFP_ZERO, get_order(THREAD_SIZE)))
#else
#define alloc_thread_info(tsk) ((struct thread_info *) \
	__get_free_pages(GFP_KERNEL, get_order(THREAD_SIZE)))
#endif

#define free_thread_info(info)	free_pages((unsigned long)(info), get_order(THREAD_SIZE))

#else 

#define GET_THREAD_INFO(reg) \
	movl $-THREAD_SIZE, reg; \
	andl %esp, reg

#define GET_THREAD_INFO_WITH_ESP(reg) \
	andl $-THREAD_SIZE, reg

#endif

#define TIF_SYSCALL_TRACE	0	
#define TIF_SIGPENDING		1	
#define TIF_NEED_RESCHED	2	
#define TIF_SINGLESTEP		3	
#define TIF_IRET		4	
#define TIF_SYSCALL_EMU		5	
#define TIF_SYSCALL_AUDIT	6	
#define TIF_SECCOMP		7	
#define TIF_RESTORE_SIGMASK	8	
#define TIF_MEMDIE		16
#define TIF_DEBUG		17	
#define TIF_IO_BITMAP		18	
#define TIF_FREEZE		19	
#define TIF_NOTSC		20	

#define _TIF_SYSCALL_TRACE	(1<<TIF_SYSCALL_TRACE)
#define _TIF_SIGPENDING		(1<<TIF_SIGPENDING)
#define _TIF_NEED_RESCHED	(1<<TIF_NEED_RESCHED)
#define _TIF_SINGLESTEP		(1<<TIF_SINGLESTEP)
#define _TIF_IRET		(1<<TIF_IRET)
#define _TIF_SYSCALL_EMU	(1<<TIF_SYSCALL_EMU)
#define _TIF_SYSCALL_AUDIT	(1<<TIF_SYSCALL_AUDIT)
#define _TIF_SECCOMP		(1<<TIF_SECCOMP)
#define _TIF_RESTORE_SIGMASK	(1<<TIF_RESTORE_SIGMASK)
#define _TIF_DEBUG		(1<<TIF_DEBUG)
#define _TIF_IO_BITMAP		(1<<TIF_IO_BITMAP)
#define _TIF_FREEZE		(1<<TIF_FREEZE)
#define _TIF_NOTSC		(1<<TIF_NOTSC)

#define _TIF_WORK_MASK \
  (0x0000FFFF & ~(_TIF_SYSCALL_TRACE | _TIF_SYSCALL_AUDIT | \
		  _TIF_SECCOMP | _TIF_SYSCALL_EMU))
#define _TIF_ALLWORK_MASK	(0x0000FFFF & ~_TIF_SECCOMP)

#define _TIF_WORK_CTXSW_NEXT (_TIF_IO_BITMAP | _TIF_NOTSC | _TIF_DEBUG)
#define _TIF_WORK_CTXSW_PREV (_TIF_IO_BITMAP | _TIF_NOTSC)

#define TS_USEDFPU		0x0001	
#define TS_POLLING		0x0002	

#define tsk_is_polling(t) (task_thread_info(t)->status & TS_POLLING)

#endif 

#endif 
