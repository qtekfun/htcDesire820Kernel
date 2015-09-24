#ifndef _LINUX_FUTEX_H
#define _LINUX_FUTEX_H

#include <linux/sched.h>



#define FUTEX_WAIT		0
#define FUTEX_WAKE		1
#define FUTEX_FD		2
#define FUTEX_REQUEUE		3
#define FUTEX_CMP_REQUEUE	4
#define FUTEX_WAKE_OP		5
#define FUTEX_LOCK_PI		6
#define FUTEX_UNLOCK_PI		7
#define FUTEX_TRYLOCK_PI	8


struct robust_list {
	struct robust_list __user *next;
};

struct robust_list_head {
	struct robust_list list;

	long futex_offset;

	struct robust_list __user *list_op_pending;
};

#define FUTEX_WAITERS		0x80000000

#define FUTEX_OWNER_DIED	0x40000000

#define FUTEX_TID_MASK		0x3fffffff

#define ROBUST_LIST_LIMIT	2048

long do_futex(u32 __user *uaddr, int op, u32 val, unsigned long timeout,
	      u32 __user *uaddr2, u32 val2, u32 val3);

extern int
handle_futex_death(u32 __user *uaddr, struct task_struct *curr, int pi);

#ifdef CONFIG_FUTEX
extern void exit_robust_list(struct task_struct *curr);
extern void exit_pi_state_list(struct task_struct *curr);
#else
static inline void exit_robust_list(struct task_struct *curr)
{
}
static inline void exit_pi_state_list(struct task_struct *curr)
{
}
#endif

#define FUTEX_OP_SET		0	
#define FUTEX_OP_ADD		1	
#define FUTEX_OP_OR		2	
#define FUTEX_OP_ANDN		3	
#define FUTEX_OP_XOR		4	

#define FUTEX_OP_OPARG_SHIFT	8	

#define FUTEX_OP_CMP_EQ		0	
#define FUTEX_OP_CMP_NE		1	
#define FUTEX_OP_CMP_LT		2	
#define FUTEX_OP_CMP_LE		3	
#define FUTEX_OP_CMP_GT		4	
#define FUTEX_OP_CMP_GE		5	


#define FUTEX_OP(op, oparg, cmp, cmparg) \
  (((op & 0xf) << 28) | ((cmp & 0xf) << 24)		\
   | ((oparg & 0xfff) << 12) | (cmparg & 0xfff))

#endif
