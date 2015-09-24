#ifndef _LINUX_SECCOMP_H
#define _LINUX_SECCOMP_H

#include <linux/compiler.h>
#include <linux/types.h>


#define SECCOMP_MODE_DISABLED	0 
#define SECCOMP_MODE_STRICT	1 
#define SECCOMP_MODE_FILTER	2 

#define SECCOMP_RET_KILL	0x00000000U 
#define SECCOMP_RET_TRAP	0x00030000U 
#define SECCOMP_RET_ERRNO	0x00050000U 
#define SECCOMP_RET_TRACE	0x7ff00000U 
#define SECCOMP_RET_ALLOW	0x7fff0000U 

#define SECCOMP_RET_ACTION	0x7fff0000U
#define SECCOMP_RET_DATA	0x0000ffffU

struct seccomp_data {
	int nr;
	__u32 arch;
	__u64 instruction_pointer;
	__u64 args[6];
};

#ifdef __KERNEL__
#ifdef CONFIG_SECCOMP

#include <linux/thread_info.h>
#include <asm/seccomp.h>

struct seccomp_filter;
struct seccomp {
	int mode;
	struct seccomp_filter *filter;
};

extern int __secure_computing(int);
static inline int secure_computing(int this_syscall)
{
	if (unlikely(test_thread_flag(TIF_SECCOMP)))
		return  __secure_computing(this_syscall);
	return 0;
}

static inline void secure_computing_strict(int this_syscall)
{
	BUG_ON(secure_computing(this_syscall) != 0);
}

extern long prctl_get_seccomp(void);
extern long prctl_set_seccomp(unsigned long, char __user *);

static inline int seccomp_mode(struct seccomp *s)
{
	return s->mode;
}

#else 

#include <linux/errno.h>

struct seccomp { };
struct seccomp_filter { };

static inline int secure_computing(int this_syscall) { return 0; }
static inline void secure_computing_strict(int this_syscall) { return; }

static inline long prctl_get_seccomp(void)
{
	return -EINVAL;
}

static inline long prctl_set_seccomp(unsigned long arg2, char __user *arg3)
{
	return -EINVAL;
}

static inline int seccomp_mode(struct seccomp *s)
{
	return 0;
}
#endif 

#ifdef CONFIG_SECCOMP_FILTER
extern void put_seccomp_filter(struct task_struct *tsk);
extern void get_seccomp_filter(struct task_struct *tsk);
extern u32 seccomp_bpf_load(int off);
#else  
static inline void put_seccomp_filter(struct task_struct *tsk)
{
	return;
}
static inline void get_seccomp_filter(struct task_struct *tsk)
{
	return;
}
#endif 
#endif 
#endif 
