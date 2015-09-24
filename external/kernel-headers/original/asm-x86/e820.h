#ifndef __ASM_E820_H
#define __ASM_E820_H
#define E820MAP	0x2d0		
#define E820MAX	128		
#define E820NR	0x1e8		

#define E820_RAM	1
#define E820_RESERVED	2
#define E820_ACPI	3
#define E820_NVS	4

#ifndef __ASSEMBLY__
struct e820entry {
	__u64 addr;	
	__u64 size;	
	__u32 type;	
} __attribute__((packed));

struct e820map {
	__u32 nr_map;
	struct e820entry map[E820MAX];
};
#endif 

#ifdef __KERNEL__
#ifdef CONFIG_X86_32
# include "e820_32.h"
#else
# include "e820_64.h"
#endif
#endif 

#endif  
