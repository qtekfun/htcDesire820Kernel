/*
 * Read-Copy Update mechanism for mutual exclusion 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Copyright (C) IBM Corporation, 2001
 *
 * Author: Dipankar Sarma <dipankar@in.ibm.com>
 * 
 * Based on the original work by Paul McKenney <paul.mckenney@us.ibm.com>
 * and inputs from Rusty Russell, Andrea Arcangeli and Andi Kleen.
 * Papers:
 * http://www.rdrop.com/users/paulmck/paper/rclockpdcsproof.pdf
 * http://lse.sourceforge.net/locking/rclock_OLS.2001.05.01c.sc.pdf (OLS2001)
 *
 * For detailed explanation of Read-Copy Update mechanism see -
 * 		http://lse.sourceforge.net/locking/rcupdate.html
 *
 */

#ifndef __LINUX_RCUPDATE_H
#define __LINUX_RCUPDATE_H

#ifdef __KERNEL__

#include <linux/cache.h>
#include <linux/spinlock.h>
#include <linux/threads.h>
#include <linux/percpu.h>
#include <linux/cpumask.h>
#include <linux/seqlock.h>

struct rcu_head {
	struct rcu_head *next;
	void (*func)(struct rcu_head *head);
};

#define RCU_HEAD_INIT 	{ .next = NULL, .func = NULL }
#define RCU_HEAD(head) struct rcu_head head = RCU_HEAD_INIT
#define INIT_RCU_HEAD(ptr) do { \
       (ptr)->next = NULL; (ptr)->func = NULL; \
} while (0)



struct rcu_ctrlblk {
	long	cur;		
	long	completed;	
	int	next_pending;	

	spinlock_t	lock	____cacheline_internodealigned_in_smp;
	cpumask_t	cpumask; 
	                         
} ____cacheline_internodealigned_in_smp;

static inline int rcu_batch_before(long a, long b)
{
        return (a - b) < 0;
}

static inline int rcu_batch_after(long a, long b)
{
        return (a - b) > 0;
}

struct rcu_data {
	
	long		quiescbatch;     
	int		passed_quiesc;	 
	int		qs_pending;	 

	
	long  	       	batch;           
	struct rcu_head *nxtlist;
	struct rcu_head **nxttail;
	long            qlen; 	 	 
	struct rcu_head *curlist;
	struct rcu_head **curtail;
	struct rcu_head *donelist;
	struct rcu_head **donetail;
	long		blimit;		 
	int cpu;
	struct rcu_head barrier;
#ifdef CONFIG_SMP
	long		last_rs_qlen;	 
#endif
};

DECLARE_PER_CPU(struct rcu_data, rcu_data);
DECLARE_PER_CPU(struct rcu_data, rcu_bh_data);

static inline void rcu_qsctr_inc(int cpu)
{
	struct rcu_data *rdp = &per_cpu(rcu_data, cpu);
	rdp->passed_quiesc = 1;
}
static inline void rcu_bh_qsctr_inc(int cpu)
{
	struct rcu_data *rdp = &per_cpu(rcu_bh_data, cpu);
	rdp->passed_quiesc = 1;
}

extern int rcu_pending(int cpu);
extern int rcu_needs_cpu(int cpu);

#define rcu_read_lock() \
	do { \
		preempt_disable(); \
		__acquire(RCU); \
	} while(0)

#define rcu_read_unlock() \
	do { \
		__release(RCU); \
		preempt_enable(); \
	} while(0)


#define rcu_read_lock_bh() \
	do { \
		local_bh_disable(); \
		__acquire(RCU_BH); \
	} while(0)

#define rcu_read_unlock_bh() \
	do { \
		__release(RCU_BH); \
		local_bh_enable(); \
	} while(0)


#define rcu_dereference(p)     ({ \
				typeof(p) _________p1 = p; \
				smp_read_barrier_depends(); \
				(_________p1); \
				})


#define rcu_assign_pointer(p, v)	({ \
						smp_wmb(); \
						(p) = (v); \
					})

#define synchronize_sched() synchronize_rcu()

extern void rcu_init(void);
extern void rcu_check_callbacks(int cpu, int user);
extern void rcu_restart_cpu(int cpu);
extern long rcu_batches_completed(void);
extern long rcu_batches_completed_bh(void);

extern void FASTCALL(call_rcu(struct rcu_head *head, 
				void (*func)(struct rcu_head *head)));
extern void FASTCALL(call_rcu_bh(struct rcu_head *head,
				void (*func)(struct rcu_head *head)));
extern void synchronize_rcu(void);
void synchronize_idle(void);
extern void rcu_barrier(void);

#endif 
#endif 
