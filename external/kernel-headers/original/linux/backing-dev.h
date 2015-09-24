
#ifndef _LINUX_BACKING_DEV_H
#define _LINUX_BACKING_DEV_H

#include <asm/atomic.h>

enum bdi_state {
	BDI_pdflush,		
	BDI_write_congested,	
	BDI_read_congested,	
	BDI_unused,		
};

typedef int (congested_fn)(void *, int);

struct backing_dev_info {
	unsigned long ra_pages;	
	unsigned long state;	
	unsigned int capabilities; 
	congested_fn *congested_fn; 
	void *congested_data;	
	void (*unplug_io_fn)(struct backing_dev_info *, struct page *);
	void *unplug_io_data;
};


#define BDI_CAP_NO_ACCT_DIRTY	0x00000001	
#define BDI_CAP_NO_WRITEBACK	0x00000002	
#define BDI_CAP_MAP_COPY	0x00000004	
#define BDI_CAP_MAP_DIRECT	0x00000008	
#define BDI_CAP_READ_MAP	0x00000010	
#define BDI_CAP_WRITE_MAP	0x00000020	
#define BDI_CAP_EXEC_MAP	0x00000040	
#define BDI_CAP_VMFLAGS \
	(BDI_CAP_READ_MAP | BDI_CAP_WRITE_MAP | BDI_CAP_EXEC_MAP)

#if defined(VM_MAYREAD) && \
	(BDI_CAP_READ_MAP != VM_MAYREAD || \
	 BDI_CAP_WRITE_MAP != VM_MAYWRITE || \
	 BDI_CAP_EXEC_MAP != VM_MAYEXEC)
#error please change backing_dev_info::capabilities flags
#endif

extern struct backing_dev_info default_backing_dev_info;
void default_unplug_io_fn(struct backing_dev_info *bdi, struct page *page);

int writeback_acquire(struct backing_dev_info *bdi);
int writeback_in_progress(struct backing_dev_info *bdi);
void writeback_release(struct backing_dev_info *bdi);

static inline int bdi_congested(struct backing_dev_info *bdi, int bdi_bits)
{
	if (bdi->congested_fn)
		return bdi->congested_fn(bdi->congested_data, bdi_bits);
	return (bdi->state & bdi_bits);
}

static inline int bdi_read_congested(struct backing_dev_info *bdi)
{
	return bdi_congested(bdi, 1 << BDI_read_congested);
}

static inline int bdi_write_congested(struct backing_dev_info *bdi)
{
	return bdi_congested(bdi, 1 << BDI_write_congested);
}

static inline int bdi_rw_congested(struct backing_dev_info *bdi)
{
	return bdi_congested(bdi, (1 << BDI_read_congested)|
				  (1 << BDI_write_congested));
}

#define bdi_cap_writeback_dirty(bdi) \
	(!((bdi)->capabilities & BDI_CAP_NO_WRITEBACK))

#define bdi_cap_account_dirty(bdi) \
	(!((bdi)->capabilities & BDI_CAP_NO_ACCT_DIRTY))

#define mapping_cap_writeback_dirty(mapping) \
	bdi_cap_writeback_dirty((mapping)->backing_dev_info)

#define mapping_cap_account_dirty(mapping) \
	bdi_cap_account_dirty((mapping)->backing_dev_info)


#endif		
