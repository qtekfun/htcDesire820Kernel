/*
 * Performance events:
 *
 *    Copyright (C) 2008-2009, Thomas Gleixner <tglx@linutronix.de>
 *    Copyright (C) 2008-2009, Red Hat, Inc., Ingo Molnar
 *    Copyright (C) 2008-2009, Red Hat, Inc., Peter Zijlstra
 *
 * Data type definitions, declarations, prototypes.
 *
 *    Started by: Thomas Gleixner and Ingo Molnar
 *
 * For licencing details see kernel-base/COPYING
 */
#ifndef _LINUX_PERF_EVENT_H
#define _LINUX_PERF_EVENT_H

#include <linux/types.h>
#include <linux/ioctl.h>
#include <asm/byteorder.h>


enum perf_type_id {
	PERF_TYPE_HARDWARE			= 0,
	PERF_TYPE_SOFTWARE			= 1,
	PERF_TYPE_TRACEPOINT			= 2,
	PERF_TYPE_HW_CACHE			= 3,
	PERF_TYPE_RAW				= 4,
	PERF_TYPE_BREAKPOINT			= 5,

	PERF_TYPE_MAX,				
};

enum perf_hw_id {
	PERF_COUNT_HW_CPU_CYCLES		= 0,
	PERF_COUNT_HW_INSTRUCTIONS		= 1,
	PERF_COUNT_HW_CACHE_REFERENCES		= 2,
	PERF_COUNT_HW_CACHE_MISSES		= 3,
	PERF_COUNT_HW_BRANCH_INSTRUCTIONS	= 4,
	PERF_COUNT_HW_BRANCH_MISSES		= 5,
	PERF_COUNT_HW_BUS_CYCLES		= 6,

	PERF_COUNT_HW_MAX,			
};

enum perf_hw_cache_id {
	PERF_COUNT_HW_CACHE_L1D			= 0,
	PERF_COUNT_HW_CACHE_L1I			= 1,
	PERF_COUNT_HW_CACHE_LL			= 2,
	PERF_COUNT_HW_CACHE_DTLB		= 3,
	PERF_COUNT_HW_CACHE_ITLB		= 4,
	PERF_COUNT_HW_CACHE_BPU			= 5,

	PERF_COUNT_HW_CACHE_MAX,		
};

enum perf_hw_cache_op_id {
	PERF_COUNT_HW_CACHE_OP_READ		= 0,
	PERF_COUNT_HW_CACHE_OP_WRITE		= 1,
	PERF_COUNT_HW_CACHE_OP_PREFETCH		= 2,

	PERF_COUNT_HW_CACHE_OP_MAX,		
};

enum perf_hw_cache_op_result_id {
	PERF_COUNT_HW_CACHE_RESULT_ACCESS	= 0,
	PERF_COUNT_HW_CACHE_RESULT_MISS		= 1,

	PERF_COUNT_HW_CACHE_RESULT_MAX,		
};

enum perf_sw_ids {
	PERF_COUNT_SW_CPU_CLOCK			= 0,
	PERF_COUNT_SW_TASK_CLOCK		= 1,
	PERF_COUNT_SW_PAGE_FAULTS		= 2,
	PERF_COUNT_SW_CONTEXT_SWITCHES		= 3,
	PERF_COUNT_SW_CPU_MIGRATIONS		= 4,
	PERF_COUNT_SW_PAGE_FAULTS_MIN		= 5,
	PERF_COUNT_SW_PAGE_FAULTS_MAJ		= 6,
	PERF_COUNT_SW_ALIGNMENT_FAULTS		= 7,
	PERF_COUNT_SW_EMULATION_FAULTS		= 8,

	PERF_COUNT_SW_MAX,			
};

enum perf_event_sample_format {
	PERF_SAMPLE_IP				= 1U << 0,
	PERF_SAMPLE_TID				= 1U << 1,
	PERF_SAMPLE_TIME			= 1U << 2,
	PERF_SAMPLE_ADDR			= 1U << 3,
	PERF_SAMPLE_READ			= 1U << 4,
	PERF_SAMPLE_CALLCHAIN			= 1U << 5,
	PERF_SAMPLE_ID				= 1U << 6,
	PERF_SAMPLE_CPU				= 1U << 7,
	PERF_SAMPLE_PERIOD			= 1U << 8,
	PERF_SAMPLE_STREAM_ID			= 1U << 9,
	PERF_SAMPLE_RAW				= 1U << 10,

	PERF_SAMPLE_MAX = 1U << 11,		
};

enum perf_event_read_format {
	PERF_FORMAT_TOTAL_TIME_ENABLED		= 1U << 0,
	PERF_FORMAT_TOTAL_TIME_RUNNING		= 1U << 1,
	PERF_FORMAT_ID				= 1U << 2,
	PERF_FORMAT_GROUP			= 1U << 3,

	PERF_FORMAT_MAX = 1U << 4,		
};

#define PERF_ATTR_SIZE_VER0	64	

struct perf_event_attr {

	__u32			type;

	__u32			size;

	__u64			config;

	union {
		__u64		sample_period;
		__u64		sample_freq;
	};

	__u64			sample_type;
	__u64			read_format;

	__u64			disabled       :  1, 
				inherit	       :  1, 
				pinned	       :  1, 
				exclusive      :  1, 
				exclude_user   :  1, 
				exclude_kernel :  1, 
				exclude_hv     :  1, 
				exclude_idle   :  1, 
				mmap           :  1, 
				comm	       :  1, 
				freq           :  1, 
				inherit_stat   :  1, 
				enable_on_exec :  1, 
				task           :  1, 
				watermark      :  1, 
				precise_ip     :  2, 

				__reserved_1   : 47;

	union {
		__u32		wakeup_events;	  
		__u32		wakeup_watermark; 
	};

	__u32			bp_type;
	__u64			bp_addr;
	__u64			bp_len;
};

#define PERF_EVENT_IOC_ENABLE		_IO ('$', 0)
#define PERF_EVENT_IOC_DISABLE		_IO ('$', 1)
#define PERF_EVENT_IOC_REFRESH		_IO ('$', 2)
#define PERF_EVENT_IOC_RESET		_IO ('$', 3)
#define PERF_EVENT_IOC_PERIOD		_IOW('$', 4, __u64)
#define PERF_EVENT_IOC_SET_OUTPUT	_IO ('$', 5)
#define PERF_EVENT_IOC_SET_FILTER	_IOW('$', 6, char *)

enum perf_event_ioc_flags {
	PERF_IOC_FLAG_GROUP		= 1U << 0,
};

struct perf_event_mmap_page {
	__u32	version;		
	__u32	compat_version;		

	__u32	lock;			
	__u32	index;			
	__s64	offset;			
	__u64	time_enabled;		
	__u64	time_running;		


	__u64	__reserved[123];	

	/*
	 * Control data for the mmap() data buffer.
	 *
	 * User-space reading the @data_head value should issue an rmb(), on
	 * SMP capable platforms, after reading this value -- see
	 * perf_event_wakeup().
	 *
	 * When the mapping is PROT_WRITE the @data_tail value should be
	 * written by userspace to reflect the last read data. In this case
	 * the kernel will not over-write unread data.
	 */
	__u64   data_head;		
	__u64	data_tail;		/* user-space written tail */
};

#define PERF_RECORD_MISC_CPUMODE_MASK		(7 << 0)
#define PERF_RECORD_MISC_CPUMODE_UNKNOWN	(0 << 0)
#define PERF_RECORD_MISC_KERNEL			(1 << 0)
#define PERF_RECORD_MISC_USER			(2 << 0)
#define PERF_RECORD_MISC_HYPERVISOR		(3 << 0)
#define PERF_RECORD_MISC_GUEST_KERNEL		(4 << 0)
#define PERF_RECORD_MISC_GUEST_USER		(5 << 0)

#define PERF_RECORD_MISC_EXACT_IP		(1 << 14)
#define PERF_RECORD_MISC_EXT_RESERVED		(1 << 15)

struct perf_event_header {
	__u32	type;
	__u16	misc;
	__u16	size;
};

enum perf_event_type {

	PERF_RECORD_MMAP			= 1,

	PERF_RECORD_LOST			= 2,

	PERF_RECORD_COMM			= 3,

	PERF_RECORD_EXIT			= 4,

	PERF_RECORD_THROTTLE			= 5,
	PERF_RECORD_UNTHROTTLE			= 6,

	PERF_RECORD_FORK			= 7,

	PERF_RECORD_READ			= 8,

	PERF_RECORD_SAMPLE			= 9,

	PERF_RECORD_MAX,			
};

enum perf_callchain_context {
	PERF_CONTEXT_HV			= (__u64)-32,
	PERF_CONTEXT_KERNEL		= (__u64)-128,
	PERF_CONTEXT_USER		= (__u64)-512,

	PERF_CONTEXT_GUEST		= (__u64)-2048,
	PERF_CONTEXT_GUEST_KERNEL	= (__u64)-2176,
	PERF_CONTEXT_GUEST_USER		= (__u64)-2560,

	PERF_CONTEXT_MAX		= (__u64)-4095,
};

#define PERF_FLAG_FD_NO_GROUP	(1U << 0)
#define PERF_FLAG_FD_OUTPUT	(1U << 1)

#ifdef __KERNEL__

#ifdef CONFIG_PERF_EVENTS
# include <asm/perf_event.h>
#endif

struct perf_guest_info_callbacks {
	int (*is_in_guest) (void);
	int (*is_user_mode) (void);
	unsigned long (*get_guest_ip) (void);
};

#ifdef CONFIG_HAVE_HW_BREAKPOINT
#include <asm/hw_breakpoint.h>
#endif

#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/rculist.h>
#include <linux/rcupdate.h>
#include <linux/spinlock.h>
#include <linux/hrtimer.h>
#include <linux/fs.h>
#include <linux/pid_namespace.h>
#include <linux/workqueue.h>
#include <linux/ftrace.h>
#include <linux/cpu.h>
#include <asm/atomic.h>
#include <asm/local.h>

#define PERF_MAX_STACK_DEPTH		255

struct perf_callchain_entry {
	__u64				nr;
	__u64				ip[PERF_MAX_STACK_DEPTH];
};

struct perf_raw_record {
	u32				size;
	void				*data;
};

struct perf_branch_entry {
	__u64				from;
	__u64				to;
	__u64				flags;
};

struct perf_branch_stack {
	__u64				nr;
	struct perf_branch_entry	entries[0];
};

struct task_struct;

struct hw_perf_event {
#ifdef CONFIG_PERF_EVENTS
	union {
		struct { 
			u64		config;
			u64		last_tag;
			unsigned long	config_base;
			unsigned long	event_base;
			int		idx;
			int		last_cpu;
		};
		struct { 
			s64		remaining;
			struct hrtimer	hrtimer;
		};
#ifdef CONFIG_HAVE_HW_BREAKPOINT
		
		struct arch_hw_breakpoint	info;
#endif
	};
	atomic64_t			prev_count;
	u64				sample_period;
	u64				last_period;
	atomic64_t			period_left;
	u64				interrupts;

	u64				freq_time_stamp;
	u64				freq_count_stamp;
#endif
};

struct perf_event;

#define PERF_EVENT_TXN_STARTED 1

struct pmu {
	int (*enable)			(struct perf_event *event);
	void (*disable)			(struct perf_event *event);
	int (*start)			(struct perf_event *event);
	void (*stop)			(struct perf_event *event);
	void (*read)			(struct perf_event *event);
	void (*unthrottle)		(struct perf_event *event);


	void (*start_txn)	(const struct pmu *pmu);
	void (*cancel_txn)	(const struct pmu *pmu);
	int  (*commit_txn)	(const struct pmu *pmu);
};

enum perf_event_active_state {
	PERF_EVENT_STATE_ERROR		= -2,
	PERF_EVENT_STATE_OFF		= -1,
	PERF_EVENT_STATE_INACTIVE	=  0,
	PERF_EVENT_STATE_ACTIVE		=  1,
};

struct file;

struct perf_mmap_data {
	atomic_t			refcount;
	struct rcu_head			rcu_head;
#ifdef CONFIG_PERF_USE_VMALLOC
	struct work_struct		work;
	int				page_order;	
#endif
	int				nr_pages;	
	int				writable;	

	atomic_t			poll;		

	local_t				head;		
	local_t				nest;		
	local_t				events;		
	local_t				wakeup;		
	local_t				lost;		

	long				watermark;	

	struct perf_event_mmap_page	*user_page;
	void				*data_pages[0];
};

struct perf_pending_entry {
	struct perf_pending_entry *next;
	void (*func)(struct perf_pending_entry *);
};

struct perf_sample_data;

typedef void (*perf_overflow_handler_t)(struct perf_event *, int,
					struct perf_sample_data *,
					struct pt_regs *regs);

enum perf_group_flag {
	PERF_GROUP_SOFTWARE = 0x1,
};

#define SWEVENT_HLIST_BITS	8
#define SWEVENT_HLIST_SIZE	(1 << SWEVENT_HLIST_BITS)

struct swevent_hlist {
	struct hlist_head	heads[SWEVENT_HLIST_SIZE];
	struct rcu_head		rcu_head;
};

#define PERF_ATTACH_CONTEXT	0x01
#define PERF_ATTACH_GROUP	0x02

struct perf_event {
#ifdef CONFIG_PERF_EVENTS
	struct list_head		group_entry;
	struct list_head		event_entry;
	struct list_head		sibling_list;
	struct hlist_node		hlist_entry;
	int				nr_siblings;
	int				group_flags;
	struct perf_event		*group_leader;
	const struct pmu		*pmu;

	enum perf_event_active_state	state;
	unsigned int			attach_state;
	atomic64_t			count;

	u64				total_time_enabled;
	u64				total_time_running;

	u64				tstamp_enabled;
	u64				tstamp_running;
	u64				tstamp_stopped;

	struct perf_event_attr		attr;
	struct hw_perf_event		hw;

	struct perf_event_context	*ctx;
	struct file			*filp;

	atomic64_t			child_total_time_enabled;
	atomic64_t			child_total_time_running;

	struct mutex			child_mutex;
	struct list_head		child_list;
	struct perf_event		*parent;

	int				oncpu;
	int				cpu;

	struct list_head		owner_entry;
	struct task_struct		*owner;

	
	struct mutex			mmap_mutex;
	atomic_t			mmap_count;
	int				mmap_locked;
	struct user_struct		*mmap_user;
	struct perf_mmap_data		*data;

	
	wait_queue_head_t		waitq;
	struct fasync_struct		*fasync;

	
	int				pending_wakeup;
	int				pending_kill;
	int				pending_disable;
	struct perf_pending_entry	pending;

	atomic_t			event_limit;

	void (*destroy)(struct perf_event *);
	struct rcu_head			rcu_head;

	struct pid_namespace		*ns;
	u64				id;

	perf_overflow_handler_t		overflow_handler;

#ifdef CONFIG_EVENT_TRACING
	struct ftrace_event_call	*tp_event;
	struct event_filter		*filter;
#endif

#endif 
};

struct perf_event_context {
	raw_spinlock_t			lock;
	struct mutex			mutex;

	struct list_head		pinned_groups;
	struct list_head		flexible_groups;
	struct list_head		event_list;
	int				nr_events;
	int				nr_active;
	int				is_active;
	int				nr_stat;
	atomic_t			refcount;
	struct task_struct		*task;

	u64				time;
	u64				timestamp;

	struct perf_event_context	*parent_ctx;
	u64				parent_gen;
	u64				generation;
	int				pin_count;
	struct rcu_head			rcu_head;
};

struct perf_cpu_context {
	struct perf_event_context	ctx;
	struct perf_event_context	*task_ctx;
	int				active_oncpu;
	int				max_pertask;
	int				exclusive;
	struct swevent_hlist		*swevent_hlist;
	struct mutex			hlist_mutex;
	int				hlist_refcount;

	int				recursion[4];
};

struct perf_output_handle {
	struct perf_event		*event;
	struct perf_mmap_data		*data;
	unsigned long			wakeup;
	unsigned long			size;
	void				*addr;
	int				page;
	int				nmi;
	int				sample;
};

#ifdef CONFIG_PERF_EVENTS

extern int perf_max_events;

extern const struct pmu *hw_perf_event_init(struct perf_event *event);

extern void perf_event_task_sched_in(struct task_struct *task);
extern void perf_event_task_sched_out(struct task_struct *task, struct task_struct *next);
extern void perf_event_task_tick(struct task_struct *task);
extern int perf_event_init_task(struct task_struct *child);
extern void perf_event_exit_task(struct task_struct *child);
extern void perf_event_free_task(struct task_struct *task);
extern void set_perf_event_pending(void);
extern void perf_event_do_pending(void);
extern void perf_event_print_debug(void);
extern void __perf_disable(void);
extern bool __perf_enable(void);
extern void perf_disable(void);
extern void perf_enable(void);
extern int perf_event_task_disable(void);
extern int perf_event_task_enable(void);
extern void perf_event_update_userpage(struct perf_event *event);
extern int perf_event_release_kernel(struct perf_event *event);
extern struct perf_event *
perf_event_create_kernel_counter(struct perf_event_attr *attr,
				int cpu,
				pid_t pid,
				perf_overflow_handler_t callback);
extern u64 perf_event_read_value(struct perf_event *event,
				 u64 *enabled, u64 *running);

struct perf_sample_data {
	u64				type;

	u64				ip;
	struct {
		u32	pid;
		u32	tid;
	}				tid_entry;
	u64				time;
	u64				addr;
	u64				id;
	u64				stream_id;
	struct {
		u32	cpu;
		u32	reserved;
	}				cpu_entry;
	u64				period;
	struct perf_callchain_entry	*callchain;
	struct perf_raw_record		*raw;
};

static inline
void perf_sample_data_init(struct perf_sample_data *data, u64 addr)
{
	data->addr = addr;
	data->raw  = NULL;
}

extern void perf_output_sample(struct perf_output_handle *handle,
			       struct perf_event_header *header,
			       struct perf_sample_data *data,
			       struct perf_event *event);
extern void perf_prepare_sample(struct perf_event_header *header,
				struct perf_sample_data *data,
				struct perf_event *event,
				struct pt_regs *regs);

extern int perf_event_overflow(struct perf_event *event, int nmi,
				 struct perf_sample_data *data,
				 struct pt_regs *regs);

static inline int is_software_event(struct perf_event *event)
{
	switch (event->attr.type) {
	case PERF_TYPE_SOFTWARE:
	case PERF_TYPE_TRACEPOINT:
	
	case PERF_TYPE_BREAKPOINT:
		return 1;
	}
	return 0;
}

extern atomic_t perf_swevent_enabled[PERF_COUNT_SW_MAX];

extern void __perf_sw_event(u32, u64, int, struct pt_regs *, u64);

extern void
perf_arch_fetch_caller_regs(struct pt_regs *regs, unsigned long ip, int skip);

static inline void perf_fetch_caller_regs(struct pt_regs *regs, int skip)
{
	unsigned long ip;

	memset(regs, 0, sizeof(*regs));

	switch (skip) {
	case 1 :
		ip = CALLER_ADDR0;
		break;
	case 2 :
		ip = CALLER_ADDR1;
		break;
	case 3 :
		ip = CALLER_ADDR2;
		break;
	case 4:
		ip = CALLER_ADDR3;
		break;
	
	default:
		ip = 0;
	}

	return perf_arch_fetch_caller_regs(regs, ip, skip);
}

static inline void
perf_sw_event(u32 event_id, u64 nr, int nmi, struct pt_regs *regs, u64 addr)
{
	if (atomic_read(&perf_swevent_enabled[event_id])) {
		struct pt_regs hot_regs;

		if (!regs) {
			perf_fetch_caller_regs(&hot_regs, 1);
			regs = &hot_regs;
		}
		__perf_sw_event(event_id, nr, nmi, regs, addr);
	}
}

extern void __perf_event_mmap(struct vm_area_struct *vma);

static inline void perf_event_mmap(struct vm_area_struct *vma)
{
	if (vma->vm_flags & VM_EXEC)
		__perf_event_mmap(vma);
}

extern struct perf_guest_info_callbacks *perf_guest_cbs;
extern int perf_register_guest_info_callbacks(struct perf_guest_info_callbacks *callbacks);
extern int perf_unregister_guest_info_callbacks(struct perf_guest_info_callbacks *callbacks);

extern void perf_event_comm(struct task_struct *tsk);
extern void perf_event_fork(struct task_struct *tsk);

extern struct perf_callchain_entry *perf_callchain(struct pt_regs *regs);

extern int sysctl_perf_event_paranoid;
extern int sysctl_perf_event_mlock;
extern int sysctl_perf_event_sample_rate;

static inline bool perf_paranoid_tracepoint_raw(void)
{
	return sysctl_perf_event_paranoid > -1;
}

static inline bool perf_paranoid_cpu(void)
{
	return sysctl_perf_event_paranoid > 0;
}

static inline bool perf_paranoid_kernel(void)
{
	return sysctl_perf_event_paranoid > 1;
}

extern void perf_event_init(void);
extern void perf_tp_event(u64 addr, u64 count, void *record,
			  int entry_size, struct pt_regs *regs,
			  struct hlist_head *head);
extern void perf_bp_event(struct perf_event *event, void *data);

#ifndef perf_misc_flags
#define perf_misc_flags(regs)	(user_mode(regs) ? PERF_RECORD_MISC_USER : \
				 PERF_RECORD_MISC_KERNEL)
#define perf_instruction_pointer(regs)	instruction_pointer(regs)
#endif

extern int perf_output_begin(struct perf_output_handle *handle,
			     struct perf_event *event, unsigned int size,
			     int nmi, int sample);
extern void perf_output_end(struct perf_output_handle *handle);
extern void perf_output_copy(struct perf_output_handle *handle,
			     const void *buf, unsigned int len);
extern int perf_swevent_get_recursion_context(void);
extern void perf_swevent_put_recursion_context(int rctx);
extern void perf_event_enable(struct perf_event *event);
extern void perf_event_disable(struct perf_event *event);
#else
static inline void
perf_event_task_sched_in(struct task_struct *task)			{ }
static inline void
perf_event_task_sched_out(struct task_struct *task,
			    struct task_struct *next)			{ }
static inline void
perf_event_task_tick(struct task_struct *task)				{ }
static inline int perf_event_init_task(struct task_struct *child)	{ return 0; }
static inline void perf_event_exit_task(struct task_struct *child)	{ }
static inline void perf_event_free_task(struct task_struct *task)	{ }
static inline void perf_event_do_pending(void)				{ }
static inline void perf_event_print_debug(void)				{ }
static inline void perf_disable(void)					{ }
static inline void perf_enable(void)					{ }
static inline int perf_event_task_disable(void)				{ return -EINVAL; }
static inline int perf_event_task_enable(void)				{ return -EINVAL; }

static inline void
perf_sw_event(u32 event_id, u64 nr, int nmi,
		     struct pt_regs *regs, u64 addr)			{ }
static inline void
perf_bp_event(struct perf_event *event, void *data)			{ }

static inline int perf_register_guest_info_callbacks
(struct perf_guest_info_callbacks *callbacks) { return 0; }
static inline int perf_unregister_guest_info_callbacks
(struct perf_guest_info_callbacks *callbacks) { return 0; }

static inline void perf_event_mmap(struct vm_area_struct *vma)		{ }
static inline void perf_event_comm(struct task_struct *tsk)		{ }
static inline void perf_event_fork(struct task_struct *tsk)		{ }
static inline void perf_event_init(void)				{ }
static inline int  perf_swevent_get_recursion_context(void)		{ return -1; }
static inline void perf_swevent_put_recursion_context(int rctx)		{ }
static inline void perf_event_enable(struct perf_event *event)		{ }
static inline void perf_event_disable(struct perf_event *event)		{ }
#endif

#define perf_output_put(handle, x) \
	perf_output_copy((handle), &(x), sizeof(x))

#define perf_cpu_notifier(fn)					\
do {								\
	static struct notifier_block fn##_nb __cpuinitdata =	\
		{ .notifier_call = fn, .priority = 20 };	\
	fn(&fn##_nb, (unsigned long)CPU_UP_PREPARE,		\
		(void *)(unsigned long)smp_processor_id());	\
	fn(&fn##_nb, (unsigned long)CPU_STARTING,		\
		(void *)(unsigned long)smp_processor_id());	\
	fn(&fn##_nb, (unsigned long)CPU_ONLINE,			\
		(void *)(unsigned long)smp_processor_id());	\
	register_cpu_notifier(&fn##_nb);			\
} while (0)

#endif 
#endif 