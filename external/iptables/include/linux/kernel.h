#ifndef _LINUX_KERNEL_H
#define _LINUX_KERNEL_H

#define __ALIGN_KERNEL(x, a)		__ALIGN_KERNEL_MASK(x, (typeof(x))(a) - 1)
#define __ALIGN_KERNEL_MASK(x, mask)	(((x) + (mask)) & ~(mask))



#define SI_LOAD_SHIFT	16
struct sysinfo {
	long uptime;			
	unsigned long loads[3];		
	unsigned long totalram;		
	unsigned long freeram;		
	unsigned long sharedram;	
	unsigned long bufferram;	
	unsigned long totalswap;	
	unsigned long freeswap;		
	unsigned short procs;		
	unsigned short pad;		
	unsigned long totalhigh;	
	unsigned long freehigh;		
	unsigned int mem_unit;		
	char _f[20-2*sizeof(long)-sizeof(int)];	
};

#define BUILD_BUG_ON(condition) ((void)BUILD_BUG_ON_ZERO(condition))

#define MAYBE_BUILD_BUG_ON(cond) ((void)sizeof(char[1 - 2 * !!(cond)]))

#define BUILD_BUG_ON_NOT_POWER_OF_2(n)			\
	BUILD_BUG_ON((n) == 0 || (((n) & ((n) - 1)) != 0))

#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))
#define BUILD_BUG_ON_NULL(e) ((void *)sizeof(struct { int:-!!(e); }))

#define __FUNCTION__ (__func__)

#ifdef CONFIG_NUMA
#define NUMA_BUILD 1
#else
#define NUMA_BUILD 0
#endif

#ifdef CONFIG_FTRACE_MCOUNT_RECORD
# define REBUILD_DUE_TO_FTRACE_MCOUNT_RECORD
#endif

#endif
