#ifndef _LINUX_SCHED_H
#define _LINUX_SCHED_H

#include <linux/auxvec.h>	

#define CSIGNAL		0x000000ff	
#define CLONE_VM	0x00000100	
#define CLONE_FS	0x00000200	
#define CLONE_FILES	0x00000400	
#define CLONE_SIGHAND	0x00000800	
#define CLONE_PTRACE	0x00002000	
#define CLONE_VFORK	0x00004000	
#define CLONE_PARENT	0x00008000	
#define CLONE_THREAD	0x00010000	
#define CLONE_NEWNS	0x00020000	
#define CLONE_SYSVSEM	0x00040000	
#define CLONE_SETTLS	0x00080000	
#define CLONE_PARENT_SETTID	0x00100000	
#define CLONE_CHILD_CLEARTID	0x00200000	
#define CLONE_DETACHED		0x00400000	
#define CLONE_UNTRACED		0x00800000	
#define CLONE_CHILD_SETTID	0x01000000	
#define CLONE_STOPPED		0x02000000	

#define SCHED_NORMAL		0
#define SCHED_FIFO		1
#define SCHED_RR		2
#define SCHED_BATCH		3

#ifdef __KERNEL__

struct sched_param {
	int sched_priority;
};

#include <asm/param.h>	

#include <linux/capability.h>
#include <linux/threads.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/timex.h>
#include <linux/jiffies.h>
#include <linux/rbtree.h>
#include <linux/thread_info.h>
#include <linux/cpumask.h>
#include <linux/errno.h>
#include <linux/nodemask.h>

#include <asm/system.h>
#include <asm/semaphore.h>
#include <asm/page.h>
#include <asm/ptrace.h>
#include <asm/mmu.h>
#include <asm/cputime.h>

#include <linux/smp.h>
#include <linux/sem.h>
#include <linux/signal.h>
#include <linux/securebits.h>
#include <linux/fs_struct.h>
#include <linux/compiler.h>
#include <linux/completion.h>
#include <linux/pid.h>
#include <linux/percpu.h>
#include <linux/topology.h>
#include <linux/seccomp.h>
#include <linux/rcupdate.h>
#include <linux/futex.h>
#include <linux/rtmutex.h>

#include <linux/time.h>
#include <linux/param.h>
#include <linux/resource.h>
#include <linux/timer.h>
#include <linux/hrtimer.h>

#include <asm/processor.h>

struct exec_domain;
struct futex_pi_state;

#define CLONE_KERNEL	(CLONE_FS | CLONE_FILES | CLONE_SIGHAND)

extern unsigned long avenrun[];		

#define FSHIFT		11		
#define FIXED_1		(1<<FSHIFT)	
#define LOAD_FREQ	(5*HZ)		
#define EXP_1		1884		
#define EXP_5		2014		
#define EXP_15		2037		

#define CALC_LOAD(load,exp,n) \
	load *= exp; \
	load += n*(FIXED_1-exp); \
	load >>= FSHIFT;

extern unsigned long total_forks;
extern int nr_threads;
extern int last_pid;
DECLARE_PER_CPU(unsigned long, process_counts);
extern int nr_processes(void);
extern unsigned long nr_running(void);
extern unsigned long nr_uninterruptible(void);
extern unsigned long nr_active(void);
extern unsigned long nr_iowait(void);
extern unsigned long weighted_cpuload(const int cpu);


#define TASK_RUNNING		0
#define TASK_INTERRUPTIBLE	1
#define TASK_UNINTERRUPTIBLE	2
#define TASK_STOPPED		4
#define TASK_TRACED		8
#define EXIT_ZOMBIE		16
#define EXIT_DEAD		32
#define TASK_NONINTERACTIVE	64

#define __set_task_state(tsk, state_value)		\
	do { (tsk)->state = (state_value); } while (0)
#define set_task_state(tsk, state_value)		\
	set_mb((tsk)->state, (state_value))

#define __set_current_state(state_value)			\
	do { current->state = (state_value); } while (0)
#define set_current_state(state_value)		\
	set_mb(current->state, (state_value))

#define TASK_COMM_LEN 16

#include <linux/spinlock.h>

extern rwlock_t tasklist_lock;
extern spinlock_t mmlist_lock;

struct task_struct;

extern void sched_init(void);
extern void sched_init_smp(void);
extern void init_idle(struct task_struct *idle, int cpu);

extern cpumask_t nohz_cpu_mask;

extern void show_state(void);
extern void show_regs(struct pt_regs *);

extern void show_stack(struct task_struct *task, unsigned long *sp);

void io_schedule(void);
long io_schedule_timeout(long timeout);

extern void cpu_init (void);
extern void trap_init(void);
extern void update_process_times(int user);
extern void scheduler_tick(void);

#ifdef CONFIG_DETECT_SOFTLOCKUP
extern void softlockup_tick(void);
extern void spawn_softlockup_task(void);
extern void touch_softlockup_watchdog(void);
#else
static inline void softlockup_tick(void)
{
}
static inline void spawn_softlockup_task(void)
{
}
static inline void touch_softlockup_watchdog(void)
{
}
#endif


#define __sched		__attribute__((__section__(".sched.text")))
extern int in_sched_functions(unsigned long addr);

#define	MAX_SCHEDULE_TIMEOUT	LONG_MAX
extern signed long FASTCALL(schedule_timeout(signed long timeout));
extern signed long schedule_timeout_interruptible(signed long timeout);
extern signed long schedule_timeout_uninterruptible(signed long timeout);
asmlinkage void schedule(void);

struct namespace;

#define DEFAULT_MAX_MAP_COUNT	65536

extern int sysctl_max_map_count;

#include <linux/aio.h>

extern unsigned long
arch_get_unmapped_area(struct file *, unsigned long, unsigned long,
		       unsigned long, unsigned long);
extern unsigned long
arch_get_unmapped_area_topdown(struct file *filp, unsigned long addr,
			  unsigned long len, unsigned long pgoff,
			  unsigned long flags);
extern void arch_unmap_area(struct mm_struct *, unsigned long);
extern void arch_unmap_area_topdown(struct mm_struct *, unsigned long);

#if NR_CPUS >= CONFIG_SPLIT_PTLOCK_CPUS
#define set_mm_counter(mm, member, value) atomic_long_set(&(mm)->_##member, value)
#define get_mm_counter(mm, member) ((unsigned long)atomic_long_read(&(mm)->_##member))
#define add_mm_counter(mm, member, value) atomic_long_add(value, &(mm)->_##member)
#define inc_mm_counter(mm, member) atomic_long_inc(&(mm)->_##member)
#define dec_mm_counter(mm, member) atomic_long_dec(&(mm)->_##member)
typedef atomic_long_t mm_counter_t;

#else  
#define set_mm_counter(mm, member, value) (mm)->_##member = (value)
#define get_mm_counter(mm, member) ((mm)->_##member)
#define add_mm_counter(mm, member, value) (mm)->_##member += (value)
#define inc_mm_counter(mm, member) (mm)->_##member++
#define dec_mm_counter(mm, member) (mm)->_##member--
typedef unsigned long mm_counter_t;

#endif 

#define get_mm_rss(mm)					\
	(get_mm_counter(mm, file_rss) + get_mm_counter(mm, anon_rss))
#define update_hiwater_rss(mm)	do {			\
	unsigned long _rss = get_mm_rss(mm);		\
	if ((mm)->hiwater_rss < _rss)			\
		(mm)->hiwater_rss = _rss;		\
} while (0)
#define update_hiwater_vm(mm)	do {			\
	if ((mm)->hiwater_vm < (mm)->total_vm)		\
		(mm)->hiwater_vm = (mm)->total_vm;	\
} while (0)

struct mm_struct {
	struct vm_area_struct * mmap;		
	struct rb_root mm_rb;
	struct vm_area_struct * mmap_cache;	
	unsigned long (*get_unmapped_area) (struct file *filp,
				unsigned long addr, unsigned long len,
				unsigned long pgoff, unsigned long flags);
	void (*unmap_area) (struct mm_struct *mm, unsigned long addr);
	unsigned long mmap_base;		
	unsigned long task_size;		
	unsigned long cached_hole_size;         
	unsigned long free_area_cache;		
	pgd_t * pgd;
	atomic_t mm_users;			
	atomic_t mm_count;			
	int map_count;				
	struct rw_semaphore mmap_sem;
	spinlock_t page_table_lock;		

	struct list_head mmlist;		

	mm_counter_t _file_rss;
	mm_counter_t _anon_rss;

	unsigned long hiwater_rss;	
	unsigned long hiwater_vm;	

	unsigned long total_vm, locked_vm, shared_vm, exec_vm;
	unsigned long stack_vm, reserved_vm, def_flags, nr_ptes;
	unsigned long start_code, end_code, start_data, end_data;
	unsigned long start_brk, brk, start_stack;
	unsigned long arg_start, arg_end, env_start, env_end;

	unsigned long saved_auxv[AT_VECTOR_SIZE]; 

	unsigned dumpable:2;
	cpumask_t cpu_vm_mask;

	
	mm_context_t context;

	
	unsigned long swap_token_time;
	char recent_pagein;

	
	int core_waiters;
	struct completion *core_startup_done, core_done;

	
	rwlock_t		ioctx_list_lock;
	struct kioctx		*ioctx_list;
};

struct sighand_struct {
	atomic_t		count;
	struct k_sigaction	action[_NSIG];
	spinlock_t		siglock;
};

struct pacct_struct {
	int			ac_flag;
	long			ac_exitcode;
	unsigned long		ac_mem;
	cputime_t		ac_utime, ac_stime;
	unsigned long		ac_minflt, ac_majflt;
};

struct signal_struct {
	atomic_t		count;
	atomic_t		live;

	wait_queue_head_t	wait_chldexit;	

	
	struct task_struct	*curr_target;

	
	struct sigpending	shared_pending;

	
	int			group_exit_code;
	struct task_struct	*group_exit_task;
	int			notify_count;

	
	int			group_stop_count;
	unsigned int		flags; 

	
	struct list_head posix_timers;

	
	struct hrtimer real_timer;
	struct task_struct *tsk;
	ktime_t it_real_incr;

	
	cputime_t it_prof_expires, it_virt_expires;
	cputime_t it_prof_incr, it_virt_incr;

	
	pid_t pgrp;
	pid_t tty_old_pgrp;
	pid_t session;
	
	int leader;

	struct tty_struct *tty; 

	cputime_t utime, stime, cutime, cstime;
	unsigned long nvcsw, nivcsw, cnvcsw, cnivcsw;
	unsigned long min_flt, maj_flt, cmin_flt, cmaj_flt;

	unsigned long long sched_time;

	struct rlimit rlim[RLIM_NLIMITS];

	struct list_head cpu_timers[3];

#ifdef CONFIG_KEYS
	struct key *session_keyring;	
	struct key *process_keyring;	
#endif
#ifdef CONFIG_BSD_PROCESS_ACCT
	struct pacct_struct pacct;	
#endif
#ifdef CONFIG_TASKSTATS
	spinlock_t stats_lock;
	struct taskstats *stats;
#endif
};

#ifdef __ARCH_WANT_INTERRUPTS_ON_CTXSW
# define __ARCH_WANT_UNLOCKED_CTXSW
#endif

#define SIGNAL_STOP_STOPPED	0x00000001 
#define SIGNAL_STOP_DEQUEUED	0x00000002 
#define SIGNAL_STOP_CONTINUED	0x00000004 
#define SIGNAL_GROUP_EXIT	0x00000008 



#define MAX_USER_RT_PRIO	100
#define MAX_RT_PRIO		MAX_USER_RT_PRIO

#define MAX_PRIO		(MAX_RT_PRIO + 40)

#define rt_prio(prio)		unlikely((prio) < MAX_RT_PRIO)
#define rt_task(p)		rt_prio((p)->prio)
#define batch_task(p)		(unlikely((p)->policy == SCHED_BATCH))
#define has_rt_policy(p) \
	unlikely((p)->policy != SCHED_NORMAL && (p)->policy != SCHED_BATCH)

struct user_struct {
	atomic_t __count;	
	atomic_t processes;	
	atomic_t files;		
	atomic_t sigpending;	
#ifdef CONFIG_INOTIFY_USER
	atomic_t inotify_watches; 
	atomic_t inotify_devs;	
#endif
	
	unsigned long mq_bytes;	
	unsigned long locked_shm; 

#ifdef CONFIG_KEYS
	struct key *uid_keyring;	
	struct key *session_keyring;	
#endif

	
	struct list_head uidhash_list;
	uid_t uid;
};

extern struct user_struct *find_user(uid_t);

extern struct user_struct root_user;
#define INIT_USER (&root_user)

struct backing_dev_info;
struct reclaim_state;

#if defined(CONFIG_SCHEDSTATS) || defined(CONFIG_TASK_DELAY_ACCT)
struct sched_info {
	
	unsigned long	cpu_time,	
			run_delay,	
			pcnt;		

	
	unsigned long	last_arrival,	
			last_queued;	
};
#endif 

#ifdef CONFIG_SCHEDSTATS
extern struct file_operations proc_schedstat_operations;
#endif 

#ifdef CONFIG_TASK_DELAY_ACCT
struct task_delay_info {
	spinlock_t	lock;
	unsigned int	flags;	


	struct timespec blkio_start, blkio_end;	
	u64 blkio_delay;	
	u64 swapin_delay;	
	u32 blkio_count;	
				
	u32 swapin_count;	
				
};
#endif	

static inline int sched_info_on(void)
{
#ifdef CONFIG_SCHEDSTATS
	return 1;
#elif defined(CONFIG_TASK_DELAY_ACCT)
	extern int delayacct_on;
	return delayacct_on;
#else
	return 0;
#endif
}

enum idle_type
{
	SCHED_IDLE,
	NOT_IDLE,
	NEWLY_IDLE,
	MAX_IDLE_TYPES
};

#define SCHED_LOAD_SCALE	128UL	

#ifdef CONFIG_SMP
#define SD_LOAD_BALANCE		1	
#define SD_BALANCE_NEWIDLE	2	
#define SD_BALANCE_EXEC		4	
#define SD_BALANCE_FORK		8	
#define SD_WAKE_IDLE		16	
#define SD_WAKE_AFFINE		32	
#define SD_WAKE_BALANCE		64	
#define SD_SHARE_CPUPOWER	128	
#define SD_POWERSAVINGS_BALANCE	256	

#define BALANCE_FOR_POWER	((sched_mc_power_savings || sched_smt_power_savings) \
				 ? SD_POWERSAVINGS_BALANCE : 0)


struct sched_group {
	struct sched_group *next;	
	cpumask_t cpumask;

	unsigned long cpu_power;
};

struct sched_domain {
	
	struct sched_domain *parent;	
	struct sched_group *groups;	
	cpumask_t span;			
	unsigned long min_interval;	
	unsigned long max_interval;	
	unsigned int busy_factor;	
	unsigned int imbalance_pct;	
	unsigned long long cache_hot_time; 
	unsigned int cache_nice_tries;	
	unsigned int per_cpu_gain;	
	unsigned int busy_idx;
	unsigned int idle_idx;
	unsigned int newidle_idx;
	unsigned int wake_idx;
	unsigned int forkexec_idx;
	int flags;			

	
	unsigned long last_balance;	
	unsigned int balance_interval;	
	unsigned int nr_balance_failed; 

#ifdef CONFIG_SCHEDSTATS
	
	unsigned long lb_cnt[MAX_IDLE_TYPES];
	unsigned long lb_failed[MAX_IDLE_TYPES];
	unsigned long lb_balanced[MAX_IDLE_TYPES];
	unsigned long lb_imbalance[MAX_IDLE_TYPES];
	unsigned long lb_gained[MAX_IDLE_TYPES];
	unsigned long lb_hot_gained[MAX_IDLE_TYPES];
	unsigned long lb_nobusyg[MAX_IDLE_TYPES];
	unsigned long lb_nobusyq[MAX_IDLE_TYPES];

	
	unsigned long alb_cnt;
	unsigned long alb_failed;
	unsigned long alb_pushed;

	
	unsigned long sbe_cnt;
	unsigned long sbe_balanced;
	unsigned long sbe_pushed;

	
	unsigned long sbf_cnt;
	unsigned long sbf_balanced;
	unsigned long sbf_pushed;

	
	unsigned long ttwu_wake_remote;
	unsigned long ttwu_move_affine;
	unsigned long ttwu_move_balance;
#endif
};

extern int partition_sched_domains(cpumask_t *partition1,
				    cpumask_t *partition2);

extern unsigned int max_cache_size;

#endif	


struct io_context;			
void exit_io_context(void);
struct cpuset;

#define NGROUPS_SMALL		32
#define NGROUPS_PER_BLOCK	((int)(PAGE_SIZE / sizeof(gid_t)))
struct group_info {
	int ngroups;
	atomic_t usage;
	gid_t small_block[NGROUPS_SMALL];
	int nblocks;
	gid_t *blocks[0];
};

#define get_group_info(group_info) do { \
	atomic_inc(&(group_info)->usage); \
} while (0)

#define put_group_info(group_info) do { \
	if (atomic_dec_and_test(&(group_info)->usage)) \
		groups_free(group_info); \
} while (0)

extern struct group_info *groups_alloc(int gidsetsize);
extern void groups_free(struct group_info *group_info);
extern int set_current_groups(struct group_info *group_info);
extern int groups_search(struct group_info *group_info, gid_t grp);
#define GROUP_AT(gi, i) \
    ((gi)->blocks[(i)/NGROUPS_PER_BLOCK][(i)%NGROUPS_PER_BLOCK])

#ifdef ARCH_HAS_PREFETCH_SWITCH_STACK
extern void prefetch_stack(struct task_struct *t);
#else
static inline void prefetch_stack(struct task_struct *t) { }
#endif

struct audit_context;		
struct mempolicy;
struct pipe_inode_info;

enum sleep_type {
	SLEEP_NORMAL,
	SLEEP_NONINTERACTIVE,
	SLEEP_INTERACTIVE,
	SLEEP_INTERRUPTED,
};

struct prio_array;

struct task_struct {
	volatile long state;	
	struct thread_info *thread_info;
	atomic_t usage;
	unsigned long flags;	
	unsigned long ptrace;

	int lock_depth;		

#ifdef CONFIG_SMP
#ifdef __ARCH_WANT_UNLOCKED_CTXSW
	int oncpu;
#endif
#endif
	int load_weight;	
	int prio, static_prio, normal_prio;
	struct list_head run_list;
	struct prio_array *array;

	unsigned short ioprio;
	unsigned int btrace_seq;

	unsigned long sleep_avg;
	unsigned long long timestamp, last_ran;
	unsigned long long sched_time; 
	enum sleep_type sleep_type;

	unsigned long policy;
	cpumask_t cpus_allowed;
	unsigned int time_slice, first_time_slice;

#if defined(CONFIG_SCHEDSTATS) || defined(CONFIG_TASK_DELAY_ACCT)
	struct sched_info sched_info;
#endif

	struct list_head tasks;
	struct list_head ptrace_children;
	struct list_head ptrace_list;

	struct mm_struct *mm, *active_mm;

	struct linux_binfmt *binfmt;
	long exit_state;
	int exit_code, exit_signal;
	int pdeath_signal;  
	
	unsigned long personality;
	unsigned did_exec:1;
	pid_t pid;
	pid_t tgid;
	struct task_struct *real_parent; 
	struct task_struct *parent;	
	struct list_head children;	
	struct list_head sibling;	
	struct task_struct *group_leader;	

	
	struct pid_link pids[PIDTYPE_MAX];
	struct list_head thread_group;

	struct completion *vfork_done;		
	int __user *set_child_tid;		
	int __user *clear_child_tid;		

	unsigned long rt_priority;
	cputime_t utime, stime;
	unsigned long nvcsw, nivcsw; 
	struct timespec start_time;
	unsigned long min_flt, maj_flt;

  	cputime_t it_prof_expires, it_virt_expires;
	unsigned long long it_sched_expires;
	struct list_head cpu_timers[3];

	uid_t uid,euid,suid,fsuid;
	gid_t gid,egid,sgid,fsgid;
	struct group_info *group_info;
	kernel_cap_t   cap_effective, cap_inheritable, cap_permitted;
	unsigned keep_capabilities:1;
	struct user_struct *user;
#ifdef CONFIG_KEYS
	struct key *request_key_auth;	
	struct key *thread_keyring;	
	unsigned char jit_keyring;	
#endif
	int oomkilladj; 
	char comm[TASK_COMM_LEN]; 
	int link_count, total_link_count;
	struct sysv_sem sysvsem;
	struct thread_struct thread;
	struct fs_struct *fs;
	struct files_struct *files;
	struct namespace *namespace;
	struct signal_struct *signal;
	struct sighand_struct *sighand;

	sigset_t blocked, real_blocked;
	sigset_t saved_sigmask;		
	struct sigpending pending;

	unsigned long sas_ss_sp;
	size_t sas_ss_size;
	int (*notifier)(void *priv);
	void *notifier_data;
	sigset_t *notifier_mask;
	
	void *security;
	struct audit_context *audit_context;
	seccomp_t seccomp;

   	u32 parent_exec_id;
   	u32 self_exec_id;
	spinlock_t alloc_lock;

	
	spinlock_t pi_lock;

#ifdef CONFIG_RT_MUTEXES
	
	struct plist_head pi_waiters;
	
	struct rt_mutex_waiter *pi_blocked_on;
#endif

#ifdef CONFIG_DEBUG_MUTEXES
	
	struct mutex_waiter *blocked_on;
#endif
#ifdef CONFIG_TRACE_IRQFLAGS
	unsigned int irq_events;
	int hardirqs_enabled;
	unsigned long hardirq_enable_ip;
	unsigned int hardirq_enable_event;
	unsigned long hardirq_disable_ip;
	unsigned int hardirq_disable_event;
	int softirqs_enabled;
	unsigned long softirq_disable_ip;
	unsigned int softirq_disable_event;
	unsigned long softirq_enable_ip;
	unsigned int softirq_enable_event;
	int hardirq_context;
	int softirq_context;
#endif
#ifdef CONFIG_LOCKDEP
# define MAX_LOCK_DEPTH 30UL
	u64 curr_chain_key;
	int lockdep_depth;
	struct held_lock held_locks[MAX_LOCK_DEPTH];
	unsigned int lockdep_recursion;
#endif

	void *journal_info;

	struct reclaim_state *reclaim_state;

	struct backing_dev_info *backing_dev_info;

	struct io_context *io_context;

	unsigned long ptrace_message;
	siginfo_t *last_siginfo; 
	wait_queue_t *io_wait;
/* i/o counters(bytes read/written, #syscalls */
	u64 rchar, wchar, syscr, syscw;
#if defined(CONFIG_BSD_PROCESS_ACCT)
	u64 acct_rss_mem1;	
	u64 acct_vm_mem1;	
	clock_t acct_stimexpd;	
#endif
#ifdef CONFIG_NUMA
  	struct mempolicy *mempolicy;
	short il_next;
#endif
#ifdef CONFIG_CPUSETS
	struct cpuset *cpuset;
	nodemask_t mems_allowed;
	int cpuset_mems_generation;
	int cpuset_mem_spread_rotor;
#endif
	struct robust_list_head __user *robust_list;
#ifdef CONFIG_COMPAT
	struct compat_robust_list_head __user *compat_robust_list;
#endif
	struct list_head pi_state_list;
	struct futex_pi_state *pi_state_cache;

	atomic_t fs_excl;	
	struct rcu_head rcu;

	struct pipe_inode_info *splice_pipe;
#ifdef	CONFIG_TASK_DELAY_ACCT
	struct task_delay_info *delays;
#endif
};

static inline pid_t process_group(struct task_struct *tsk)
{
	return tsk->signal->pgrp;
}

static inline int pid_alive(struct task_struct *p)
{
	return p->pids[PIDTYPE_PID].pid != NULL;
}

extern void free_task(struct task_struct *tsk);
#define get_task_struct(tsk) do { atomic_inc(&(tsk)->usage); } while(0)

extern void __put_task_struct(struct task_struct *t);

static inline void put_task_struct(struct task_struct *t)
{
	if (atomic_dec_and_test(&t->usage))
		__put_task_struct(t);
}

#define PF_ALIGNWARN	0x00000001	
					
#define PF_STARTING	0x00000002	
#define PF_EXITING	0x00000004	
#define PF_DEAD		0x00000008	
#define PF_FORKNOEXEC	0x00000040	
#define PF_SUPERPRIV	0x00000100	
#define PF_DUMPCORE	0x00000200	
#define PF_SIGNALED	0x00000400	
#define PF_MEMALLOC	0x00000800	
#define PF_FLUSHER	0x00001000	
#define PF_USED_MATH	0x00002000	
#define PF_FREEZE	0x00004000	
#define PF_NOFREEZE	0x00008000	
#define PF_FROZEN	0x00010000	
#define PF_FSTRANS	0x00020000	
#define PF_KSWAPD	0x00040000	
#define PF_SWAPOFF	0x00080000	
#define PF_LESS_THROTTLE 0x00100000	
#define PF_BORROWED_MM	0x00200000	
#define PF_RANDOMIZE	0x00400000	
#define PF_SWAPWRITE	0x00800000	
#define PF_SPREAD_PAGE	0x01000000	
#define PF_SPREAD_SLAB	0x02000000	
#define PF_MEMPOLICY	0x10000000	
#define PF_MUTEX_TESTER	0x20000000	

#define clear_stopped_child_used_math(child) do { (child)->flags &= ~PF_USED_MATH; } while (0)
#define set_stopped_child_used_math(child) do { (child)->flags |= PF_USED_MATH; } while (0)
#define clear_used_math() clear_stopped_child_used_math(current)
#define set_used_math() set_stopped_child_used_math(current)
#define conditional_stopped_child_used_math(condition, child) \
	do { (child)->flags &= ~PF_USED_MATH, (child)->flags |= (condition) ? PF_USED_MATH : 0; } while (0)
#define conditional_used_math(condition) \
	conditional_stopped_child_used_math(condition, current)
#define copy_to_stopped_child_used_math(child) \
	do { (child)->flags &= ~PF_USED_MATH, (child)->flags |= current->flags & PF_USED_MATH; } while (0)
#define tsk_used_math(p) ((p)->flags & PF_USED_MATH)
#define used_math() tsk_used_math(current)

#ifdef CONFIG_SMP
extern int set_cpus_allowed(struct task_struct *p, cpumask_t new_mask);
#else
static inline int set_cpus_allowed(struct task_struct *p, cpumask_t new_mask)
{
	if (!cpu_isset(0, new_mask))
		return -EINVAL;
	return 0;
}
#endif

extern unsigned long long sched_clock(void);
extern unsigned long long
current_sched_time(const struct task_struct *current_task);

#ifdef CONFIG_SMP
extern void sched_exec(void);
#else
#define sched_exec()   {}
#endif

#ifdef CONFIG_HOTPLUG_CPU
extern void idle_task_exit(void);
#else
static inline void idle_task_exit(void) {}
#endif

extern void sched_idle_next(void);

#ifdef CONFIG_RT_MUTEXES
extern int rt_mutex_getprio(struct task_struct *p);
extern void rt_mutex_setprio(struct task_struct *p, int prio);
extern void rt_mutex_adjust_pi(struct task_struct *p);
#else
static inline int rt_mutex_getprio(struct task_struct *p)
{
	return p->normal_prio;
}
# define rt_mutex_adjust_pi(p)		do { } while (0)
#endif

extern void set_user_nice(struct task_struct *p, long nice);
extern int task_prio(const struct task_struct *p);
extern int task_nice(const struct task_struct *p);
extern int can_nice(const struct task_struct *p, const int nice);
extern int task_curr(const struct task_struct *p);
extern int idle_cpu(int cpu);
extern int sched_setscheduler(struct task_struct *, int, struct sched_param *);
extern struct task_struct *idle_task(int cpu);
extern struct task_struct *curr_task(int cpu);
extern void set_curr_task(int cpu, struct task_struct *p);

void yield(void);

extern struct exec_domain	default_exec_domain;

union thread_union {
	struct thread_info thread_info;
	unsigned long stack[THREAD_SIZE/sizeof(long)];
};

#ifndef __HAVE_ARCH_KSTACK_END
static inline int kstack_end(void *addr)
{
	return !(((unsigned long)addr+sizeof(void*)-1) & (THREAD_SIZE-sizeof(void*)));
}
#endif

extern union thread_union init_thread_union;
extern struct task_struct init_task;

extern struct   mm_struct init_mm;

#define find_task_by_pid(nr)	find_task_by_pid_type(PIDTYPE_PID, nr)
extern struct task_struct *find_task_by_pid_type(int type, int pid);
extern void set_special_pids(pid_t session, pid_t pgrp);
extern void __set_special_pids(pid_t session, pid_t pgrp);

extern struct user_struct * alloc_uid(uid_t);
static inline struct user_struct *get_uid(struct user_struct *u)
{
	atomic_inc(&u->__count);
	return u;
}
extern void free_uid(struct user_struct *);
extern void switch_uid(struct user_struct *);

#include <asm/current.h>

extern void do_timer(struct pt_regs *);

extern int FASTCALL(wake_up_state(struct task_struct * tsk, unsigned int state));
extern int FASTCALL(wake_up_process(struct task_struct * tsk));
extern void FASTCALL(wake_up_new_task(struct task_struct * tsk,
						unsigned long clone_flags));
#ifdef CONFIG_SMP
 extern void kick_process(struct task_struct *tsk);
#else
 static inline void kick_process(struct task_struct *tsk) { }
#endif
extern void FASTCALL(sched_fork(struct task_struct * p, int clone_flags));
extern void FASTCALL(sched_exit(struct task_struct * p));

extern int in_group_p(gid_t);
extern int in_egroup_p(gid_t);

extern void proc_caches_init(void);
extern void flush_signals(struct task_struct *);
extern void flush_signal_handlers(struct task_struct *, int force_default);
extern int dequeue_signal(struct task_struct *tsk, sigset_t *mask, siginfo_t *info);

static inline int dequeue_signal_lock(struct task_struct *tsk, sigset_t *mask, siginfo_t *info)
{
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&tsk->sighand->siglock, flags);
	ret = dequeue_signal(tsk, mask, info);
	spin_unlock_irqrestore(&tsk->sighand->siglock, flags);

	return ret;
}	

extern void block_all_signals(int (*notifier)(void *priv), void *priv,
			      sigset_t *mask);
extern void unblock_all_signals(void);
extern void release_task(struct task_struct * p);
extern int send_sig_info(int, struct siginfo *, struct task_struct *);
extern int send_group_sig_info(int, struct siginfo *, struct task_struct *);
extern int force_sigsegv(int, struct task_struct *);
extern int force_sig_info(int, struct siginfo *, struct task_struct *);
extern int __kill_pg_info(int sig, struct siginfo *info, pid_t pgrp);
extern int kill_pg_info(int, struct siginfo *, pid_t);
extern int kill_proc_info(int, struct siginfo *, pid_t);
extern int kill_proc_info_as_uid(int, struct siginfo *, pid_t, uid_t, uid_t, u32);
extern void do_notify_parent(struct task_struct *, int);
extern void force_sig(int, struct task_struct *);
extern void force_sig_specific(int, struct task_struct *);
extern int send_sig(int, struct task_struct *, int);
extern void zap_other_threads(struct task_struct *p);
extern int kill_pg(pid_t, int, int);
extern int kill_proc(pid_t, int, int);
extern struct sigqueue *sigqueue_alloc(void);
extern void sigqueue_free(struct sigqueue *);
extern int send_sigqueue(int, struct sigqueue *,  struct task_struct *);
extern int send_group_sigqueue(int, struct sigqueue *,  struct task_struct *);
extern int do_sigaction(int, struct k_sigaction *, struct k_sigaction *);
extern int do_sigaltstack(const stack_t __user *, stack_t __user *, unsigned long);

#define SEND_SIG_NOINFO ((struct siginfo *) 0)
#define SEND_SIG_PRIV	((struct siginfo *) 1)
#define SEND_SIG_FORCED	((struct siginfo *) 2)

static inline int is_si_special(const struct siginfo *info)
{
	return info <= SEND_SIG_FORCED;
}


static inline int on_sig_stack(unsigned long sp)
{
	return (sp - current->sas_ss_sp < current->sas_ss_size);
}

static inline int sas_ss_flags(unsigned long sp)
{
	return (current->sas_ss_size == 0 ? SS_DISABLE
		: on_sig_stack(sp) ? SS_ONSTACK : 0);
}

extern struct mm_struct * mm_alloc(void);

extern void FASTCALL(__mmdrop(struct mm_struct *));
static inline void mmdrop(struct mm_struct * mm)
{
	if (atomic_dec_and_test(&mm->mm_count))
		__mmdrop(mm);
}

extern void mmput(struct mm_struct *);
extern struct mm_struct *get_task_mm(struct task_struct *task);
extern void mm_release(struct task_struct *, struct mm_struct *);

extern int  copy_thread(int, unsigned long, unsigned long, unsigned long, struct task_struct *, struct pt_regs *);
extern void flush_thread(void);
extern void exit_thread(void);

extern void exit_files(struct task_struct *);
extern void __cleanup_signal(struct signal_struct *);
extern void __cleanup_sighand(struct sighand_struct *);
extern void exit_itimers(struct signal_struct *);

extern NORET_TYPE void do_group_exit(int);

extern void daemonize(const char *, ...);
extern int allow_signal(int);
extern int disallow_signal(int);
extern struct task_struct *child_reaper;

extern int do_execve(char *, char __user * __user *, char __user * __user *, struct pt_regs *);
extern long do_fork(unsigned long, unsigned long, struct pt_regs *, unsigned long, int __user *, int __user *);
struct task_struct *fork_idle(int);

extern void set_task_comm(struct task_struct *tsk, char *from);
extern void get_task_comm(char *to, struct task_struct *tsk);

#ifdef CONFIG_SMP
extern void wait_task_inactive(struct task_struct * p);
#else
#define wait_task_inactive(p)	do { } while (0)
#endif

#define remove_parent(p)	list_del_init(&(p)->sibling)
#define add_parent(p)		list_add_tail(&(p)->sibling,&(p)->parent->children)

#define next_task(p)	list_entry(rcu_dereference((p)->tasks.next), struct task_struct, tasks)

#define for_each_process(p) \
	for (p = &init_task ; (p = next_task(p)) != &init_task ; )

#define do_each_thread(g, t) \
	for (g = t = &init_task ; (g = t = next_task(g)) != &init_task ; ) do

#define while_each_thread(g, t) \
	while ((t = next_thread(t)) != g)

#define thread_group_leader(p)	(p == p->group_leader)

static inline struct task_struct *next_thread(const struct task_struct *p)
{
	return list_entry(rcu_dereference(p->thread_group.next),
			  struct task_struct, thread_group);
}

static inline int thread_group_empty(struct task_struct *p)
{
	return list_empty(&p->thread_group);
}

#define delay_group_leader(p) \
		(thread_group_leader(p) && !thread_group_empty(p))

static inline void task_lock(struct task_struct *p)
{
	spin_lock(&p->alloc_lock);
}

static inline void task_unlock(struct task_struct *p)
{
	spin_unlock(&p->alloc_lock);
}

extern struct sighand_struct *lock_task_sighand(struct task_struct *tsk,
							unsigned long *flags);

static inline void unlock_task_sighand(struct task_struct *tsk,
						unsigned long *flags)
{
	spin_unlock_irqrestore(&tsk->sighand->siglock, *flags);
}

#ifndef __HAVE_THREAD_FUNCTIONS

#define task_thread_info(task) (task)->thread_info
#define task_stack_page(task) ((void*)((task)->thread_info))

static inline void setup_thread_stack(struct task_struct *p, struct task_struct *org)
{
	*task_thread_info(p) = *task_thread_info(org);
	task_thread_info(p)->task = p;
}

static inline unsigned long *end_of_stack(struct task_struct *p)
{
	return (unsigned long *)(p->thread_info + 1);
}

#endif

static inline void set_tsk_thread_flag(struct task_struct *tsk, int flag)
{
	set_ti_thread_flag(task_thread_info(tsk), flag);
}

static inline void clear_tsk_thread_flag(struct task_struct *tsk, int flag)
{
	clear_ti_thread_flag(task_thread_info(tsk), flag);
}

static inline int test_and_set_tsk_thread_flag(struct task_struct *tsk, int flag)
{
	return test_and_set_ti_thread_flag(task_thread_info(tsk), flag);
}

static inline int test_and_clear_tsk_thread_flag(struct task_struct *tsk, int flag)
{
	return test_and_clear_ti_thread_flag(task_thread_info(tsk), flag);
}

static inline int test_tsk_thread_flag(struct task_struct *tsk, int flag)
{
	return test_ti_thread_flag(task_thread_info(tsk), flag);
}

static inline void set_tsk_need_resched(struct task_struct *tsk)
{
	set_tsk_thread_flag(tsk,TIF_NEED_RESCHED);
}

static inline void clear_tsk_need_resched(struct task_struct *tsk)
{
	clear_tsk_thread_flag(tsk,TIF_NEED_RESCHED);
}

static inline int signal_pending(struct task_struct *p)
{
	return unlikely(test_tsk_thread_flag(p,TIF_SIGPENDING));
}
  
static inline int need_resched(void)
{
	return unlikely(test_thread_flag(TIF_NEED_RESCHED));
}

extern int cond_resched(void);
extern int cond_resched_lock(spinlock_t * lock);
extern int cond_resched_softirq(void);

#if defined(CONFIG_PREEMPT) && defined(CONFIG_SMP)
# define need_lockbreak(lock) ((lock)->break_lock)
#else
# define need_lockbreak(lock) 0
#endif

static inline int lock_need_resched(spinlock_t *lock)
{
	if (need_lockbreak(lock) || need_resched())
		return 1;
	return 0;
}


extern FASTCALL(void recalc_sigpending_tsk(struct task_struct *t));
extern void recalc_sigpending(void);

extern void signal_wake_up(struct task_struct *t, int resume_stopped);

#ifdef CONFIG_SMP

static inline unsigned int task_cpu(const struct task_struct *p)
{
	return task_thread_info(p)->cpu;
}

static inline void set_task_cpu(struct task_struct *p, unsigned int cpu)
{
	task_thread_info(p)->cpu = cpu;
}

#else

static inline unsigned int task_cpu(const struct task_struct *p)
{
	return 0;
}

static inline void set_task_cpu(struct task_struct *p, unsigned int cpu)
{
}

#endif 

#ifdef HAVE_ARCH_PICK_MMAP_LAYOUT
extern void arch_pick_mmap_layout(struct mm_struct *mm);
#else
static inline void arch_pick_mmap_layout(struct mm_struct *mm)
{
	mm->mmap_base = TASK_UNMAPPED_BASE;
	mm->get_unmapped_area = arch_get_unmapped_area;
	mm->unmap_area = arch_unmap_area;
}
#endif

extern long sched_setaffinity(pid_t pid, cpumask_t new_mask);
extern long sched_getaffinity(pid_t pid, cpumask_t *mask);

#include <linux/sysdev.h>
extern int sched_mc_power_savings, sched_smt_power_savings;
extern struct sysdev_attribute attr_sched_mc_power_savings, attr_sched_smt_power_savings;
extern int sched_create_sysfs_power_savings_entries(struct sysdev_class *cls);

extern void normalize_rt_tasks(void);

#ifdef CONFIG_PM
static inline int frozen(struct task_struct *p)
{
	return p->flags & PF_FROZEN;
}

static inline int freezing(struct task_struct *p)
{
	return p->flags & PF_FREEZE;
}

static inline void freeze(struct task_struct *p)
{
	p->flags |= PF_FREEZE;
}

static inline void do_not_freeze(struct task_struct *p)
{
	p->flags &= ~PF_FREEZE;
}

static inline int thaw_process(struct task_struct *p)
{
	if (frozen(p)) {
		p->flags &= ~PF_FROZEN;
		wake_up_process(p);
		return 1;
	}
	return 0;
}

static inline void frozen_process(struct task_struct *p)
{
	p->flags = (p->flags & ~PF_FREEZE) | PF_FROZEN;
}

extern void refrigerator(void);
extern int freeze_processes(void);
extern void thaw_processes(void);

static inline int try_to_freeze(void)
{
	if (freezing(current)) {
		refrigerator();
		return 1;
	} else
		return 0;
}
#else
static inline int frozen(struct task_struct *p) { return 0; }
static inline int freezing(struct task_struct *p) { return 0; }
static inline void freeze(struct task_struct *p) { BUG(); }
static inline int thaw_process(struct task_struct *p) { return 1; }
static inline void frozen_process(struct task_struct *p) { BUG(); }

static inline void refrigerator(void) {}
static inline int freeze_processes(void) { BUG(); return 0; }
static inline void thaw_processes(void) {}

static inline int try_to_freeze(void) { return 0; }

#endif 
#endif 

#endif
