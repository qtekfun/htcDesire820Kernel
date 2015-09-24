#ifndef _ASM_GENERIC_MUTEX_XCHG_H
#define _ASM_GENERIC_MUTEX_XCHG_H

static inline void
__mutex_fastpath_lock(atomic_t *count, fastcall void (*fail_fn)(atomic_t *))
{
	if (unlikely(atomic_xchg(count, 0) != 1))
		fail_fn(count);
	else
		smp_mb();
}

static inline int
__mutex_fastpath_lock_retval(atomic_t *count, fastcall int (*fail_fn)(atomic_t *))
{
	if (unlikely(atomic_xchg(count, 0) != 1))
		return fail_fn(count);
	else {
		smp_mb();
		return 0;
	}
}

static inline void
__mutex_fastpath_unlock(atomic_t *count, fastcall void (*fail_fn)(atomic_t *))
{
	smp_mb();
	if (unlikely(atomic_xchg(count, 1) != 0))
		fail_fn(count);
}

#define __mutex_slowpath_needs_to_unlock()		0

static inline int
__mutex_fastpath_trylock(atomic_t *count, int (*fail_fn)(atomic_t *))
{
	int prev = atomic_xchg(count, 0);

	if (unlikely(prev < 0)) {
		prev = atomic_xchg(count, prev);
		if (prev < 0)
			prev = 0;
	}
	smp_mb();

	return prev;
}

#endif
