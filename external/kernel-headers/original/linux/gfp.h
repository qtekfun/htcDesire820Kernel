#ifndef __LINUX_GFP_H
#define __LINUX_GFP_H

#include <linux/mmzone.h>
#include <linux/stddef.h>
#include <linux/linkage.h>

struct vm_area_struct;

#define __GFP_DMA	((__force gfp_t)0x01u)
#define __GFP_HIGHMEM	((__force gfp_t)0x02u)
#ifdef CONFIG_DMA_IS_DMA32
#define __GFP_DMA32	((__force gfp_t)0x01)	
#elif BITS_PER_LONG < 64
#define __GFP_DMA32	((__force gfp_t)0x00)	
#else
#define __GFP_DMA32	((__force gfp_t)0x04)	
#endif

#define __GFP_WAIT	((__force gfp_t)0x10u)	
#define __GFP_HIGH	((__force gfp_t)0x20u)	
#define __GFP_IO	((__force gfp_t)0x40u)	
#define __GFP_FS	((__force gfp_t)0x80u)	
#define __GFP_COLD	((__force gfp_t)0x100u)	
#define __GFP_NOWARN	((__force gfp_t)0x200u)	
#define __GFP_REPEAT	((__force gfp_t)0x400u)	
#define __GFP_NOFAIL	((__force gfp_t)0x800u)	
#define __GFP_NORETRY	((__force gfp_t)0x1000u)
#define __GFP_NO_GROW	((__force gfp_t)0x2000u)
#define __GFP_COMP	((__force gfp_t)0x4000u)
#define __GFP_ZERO	((__force gfp_t)0x8000u)
#define __GFP_NOMEMALLOC ((__force gfp_t)0x10000u) 
#define __GFP_HARDWALL   ((__force gfp_t)0x20000u) 

#define __GFP_BITS_SHIFT 20	
#define __GFP_BITS_MASK ((__force gfp_t)((1 << __GFP_BITS_SHIFT) - 1))

#define GFP_LEVEL_MASK (__GFP_WAIT|__GFP_HIGH|__GFP_IO|__GFP_FS| \
			__GFP_COLD|__GFP_NOWARN|__GFP_REPEAT| \
			__GFP_NOFAIL|__GFP_NORETRY|__GFP_NO_GROW|__GFP_COMP| \
			__GFP_NOMEMALLOC|__GFP_HARDWALL)

#define GFP_NOWAIT	(GFP_ATOMIC & ~__GFP_HIGH)
#define GFP_ATOMIC	(__GFP_HIGH)
#define GFP_NOIO	(__GFP_WAIT)
#define GFP_NOFS	(__GFP_WAIT | __GFP_IO)
#define GFP_KERNEL	(__GFP_WAIT | __GFP_IO | __GFP_FS)
#define GFP_USER	(__GFP_WAIT | __GFP_IO | __GFP_FS | __GFP_HARDWALL)
#define GFP_HIGHUSER	(__GFP_WAIT | __GFP_IO | __GFP_FS | __GFP_HARDWALL | \
			 __GFP_HIGHMEM)


#define GFP_DMA		__GFP_DMA

#define GFP_DMA32	__GFP_DMA32


static inline int gfp_zone(gfp_t gfp)
{
	int zone = GFP_ZONEMASK & (__force int) gfp;
	BUG_ON(zone >= GFP_ZONETYPES);
	return zone;
}



#ifndef HAVE_ARCH_FREE_PAGE
static inline void arch_free_page(struct page *page, int order) { }
#endif

extern struct page *
FASTCALL(__alloc_pages(gfp_t, unsigned int, struct zonelist *));

static inline struct page *alloc_pages_node(int nid, gfp_t gfp_mask,
						unsigned int order)
{
	if (unlikely(order >= MAX_ORDER))
		return NULL;

	
	if (nid < 0)
		nid = numa_node_id();

	return __alloc_pages(gfp_mask, order,
		NODE_DATA(nid)->node_zonelists + gfp_zone(gfp_mask));
}

#ifdef CONFIG_NUMA
extern struct page *alloc_pages_current(gfp_t gfp_mask, unsigned order);

static inline struct page *
alloc_pages(gfp_t gfp_mask, unsigned int order)
{
	if (unlikely(order >= MAX_ORDER))
		return NULL;

	return alloc_pages_current(gfp_mask, order);
}
extern struct page *alloc_page_vma(gfp_t gfp_mask,
			struct vm_area_struct *vma, unsigned long addr);
#else
#define alloc_pages(gfp_mask, order) \
		alloc_pages_node(numa_node_id(), gfp_mask, order)
#define alloc_page_vma(gfp_mask, vma, addr) alloc_pages(gfp_mask, 0)
#endif
#define alloc_page(gfp_mask) alloc_pages(gfp_mask, 0)

extern unsigned long FASTCALL(__get_free_pages(gfp_t gfp_mask, unsigned int order));
extern unsigned long FASTCALL(get_zeroed_page(gfp_t gfp_mask));

#define __get_free_page(gfp_mask) \
		__get_free_pages((gfp_mask),0)

#define __get_dma_pages(gfp_mask, order) \
		__get_free_pages((gfp_mask) | GFP_DMA,(order))

extern void FASTCALL(__free_pages(struct page *page, unsigned int order));
extern void FASTCALL(free_pages(unsigned long addr, unsigned int order));
extern void FASTCALL(free_hot_page(struct page *page));
extern void FASTCALL(free_cold_page(struct page *page));

#define __free_page(page) __free_pages((page), 0)
#define free_page(addr) free_pages((addr),0)

void page_alloc_init(void);
#ifdef CONFIG_NUMA
void drain_node_pages(int node);
#else
static inline void drain_node_pages(int node) { };
#endif

#endif 
