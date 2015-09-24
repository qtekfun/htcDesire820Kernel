#ifndef _LINUX_PERCPU_COUNTER_H
#define _LINUX_PERCPU_COUNTER_H

#include <linux/spinlock.h>
#include <linux/smp.h>
#include <linux/threads.h>
#include <linux/percpu.h>
#include <linux/types.h>

#ifdef CONFIG_SMP

struct percpu_counter {
	spinlock_t lock;
	s64 count;
	s32 *counters;
};

#if NR_CPUS >= 16
#define FBC_BATCH	(NR_CPUS*2)
#else
#define FBC_BATCH	(NR_CPUS*4)
#endif

static inline void percpu_counter_init(struct percpu_counter *fbc, s64 amount)
{
	spin_lock_init(&fbc->lock);
	fbc->count = amount;
	fbc->counters = alloc_percpu(s32);
}

static inline void percpu_counter_destroy(struct percpu_counter *fbc)
{
	free_percpu(fbc->counters);
}

void percpu_counter_mod(struct percpu_counter *fbc, s32 amount);
s64 percpu_counter_sum(struct percpu_counter *fbc);

static inline s64 percpu_counter_read(struct percpu_counter *fbc)
{
	return fbc->count;
}

static inline s64 percpu_counter_read_positive(struct percpu_counter *fbc)
{
	s64 ret = fbc->count;

	barrier();		
	if (ret >= 0)
		return ret;
	return 1;
}

#else

struct percpu_counter {
	s64 count;
};

static inline void percpu_counter_init(struct percpu_counter *fbc, s64 amount)
{
	fbc->count = amount;
}

static inline void percpu_counter_destroy(struct percpu_counter *fbc)
{
}

static inline void
percpu_counter_mod(struct percpu_counter *fbc, s32 amount)
{
	preempt_disable();
	fbc->count += amount;
	preempt_enable();
}

static inline s64 percpu_counter_read(struct percpu_counter *fbc)
{
	return fbc->count;
}

static inline s64 percpu_counter_read_positive(struct percpu_counter *fbc)
{
	return fbc->count;
}

static inline s64 percpu_counter_sum(struct percpu_counter *fbc)
{
	return percpu_counter_read_positive(fbc);
}

#endif	

static inline void percpu_counter_inc(struct percpu_counter *fbc)
{
	percpu_counter_mod(fbc, 1);
}

static inline void percpu_counter_dec(struct percpu_counter *fbc)
{
	percpu_counter_mod(fbc, -1);
}

#endif 