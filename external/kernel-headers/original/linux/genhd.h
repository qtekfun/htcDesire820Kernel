#ifndef _LINUX_GENHD_H
#define _LINUX_GENHD_H

/*
 * 	genhd.h Copyright (C) 1992 Drew Eckhardt
 *	Generic hard disk header file by  
 * 		Drew Eckhardt
 *
 *		<drew@colorado.edu>
 */

#include <linux/types.h>

enum {
	DOS_EXTENDED_PARTITION = 5,
	LINUX_EXTENDED_PARTITION = 0x85,
	WIN98_EXTENDED_PARTITION = 0x0f,

	LINUX_SWAP_PARTITION = 0x82,
	LINUX_RAID_PARTITION = 0xfd,	

	SOLARIS_X86_PARTITION =	LINUX_SWAP_PARTITION,
	NEW_SOLARIS_X86_PARTITION = 0xbf,

	DM6_AUX1PARTITION = 0x51,	
	DM6_AUX3PARTITION = 0x53,	
	DM6_PARTITION =	0x54,		
	EZD_PARTITION =	0x55,		

	FREEBSD_PARTITION = 0xa5,	
	OPENBSD_PARTITION = 0xa6,	
	NETBSD_PARTITION = 0xa9,	
	BSDI_PARTITION = 0xb7,		
	MINIX_PARTITION = 0x81,		
	UNIXWARE_PARTITION = 0x63,	
};

#ifndef __KERNEL__

struct partition {
	unsigned char boot_ind;		
	unsigned char head;		
	unsigned char sector;		
	unsigned char cyl;		
	unsigned char sys_ind;		
	unsigned char end_head;		
	unsigned char end_sector;	
	unsigned char end_cyl;		
	unsigned int start_sect;	
	unsigned int nr_sects;		
} __attribute__((packed));

#endif

#ifdef __KERNEL__
#include <linux/major.h>
#include <linux/device.h>
#include <linux/smp.h>
#include <linux/string.h>
#include <linux/fs.h>

struct partition {
	unsigned char boot_ind;		
	unsigned char head;		
	unsigned char sector;		
	unsigned char cyl;		
	unsigned char sys_ind;		
	unsigned char end_head;		
	unsigned char end_sector;	
	unsigned char end_cyl;		
	__le32 start_sect;	
	__le32 nr_sects;		
} __attribute__((packed));

struct hd_struct {
	sector_t start_sect;
	sector_t nr_sects;
	struct kobject kobj;
	struct kobject *holder_dir;
	unsigned ios[2], sectors[2];	
	int policy, partno;
};

#define GENHD_FL_REMOVABLE			1
#define GENHD_FL_DRIVERFS			2
#define GENHD_FL_CD				8
#define GENHD_FL_UP				16
#define GENHD_FL_SUPPRESS_PARTITION_INFO	32

struct disk_stats {
	unsigned long sectors[2];	
	unsigned long ios[2];
	unsigned long merges[2];
	unsigned long ticks[2];
	unsigned long io_ticks;
	unsigned long time_in_queue;
};
	
struct gendisk {
	int major;			
	int first_minor;
	int minors;                     
	char disk_name[32];		
	struct hd_struct **part;	
	int part_uevent_suppress;
	struct block_device_operations *fops;
	struct request_queue *queue;
	void *private_data;
	sector_t capacity;

	int flags;
	struct device *driverfs_dev;
	struct kobject kobj;
	struct kobject *holder_dir;
	struct kobject *slave_dir;

	struct timer_rand_state *random;
	int policy;

	atomic_t sync_io;		
	unsigned long stamp;
	int in_flight;
#ifdef	CONFIG_SMP
	struct disk_stats *dkstats;
#else
	struct disk_stats dkstats;
#endif
};

struct disk_attribute {
	struct attribute attr;
	ssize_t (*show)(struct gendisk *, char *);
	ssize_t (*store)(struct gendisk *, const char *, size_t);
};

#ifdef	CONFIG_SMP
#define __disk_stat_add(gendiskp, field, addnd) 	\
	(per_cpu_ptr(gendiskp->dkstats, smp_processor_id())->field += addnd)

#define disk_stat_read(gendiskp, field)					\
({									\
	typeof(gendiskp->dkstats->field) res = 0;			\
	int i;								\
	for_each_possible_cpu(i)					\
		res += per_cpu_ptr(gendiskp->dkstats, i)->field;	\
	res;								\
})

static inline void disk_stat_set_all(struct gendisk *gendiskp, int value)	{
	int i;
	for_each_possible_cpu(i)
		memset(per_cpu_ptr(gendiskp->dkstats, i), value,
				sizeof (struct disk_stats));
}		
				
#else
#define __disk_stat_add(gendiskp, field, addnd) \
				(gendiskp->dkstats.field += addnd)
#define disk_stat_read(gendiskp, field)	(gendiskp->dkstats.field)

static inline void disk_stat_set_all(struct gendisk *gendiskp, int value)	{
	memset(&gendiskp->dkstats, value, sizeof (struct disk_stats));
}
#endif

#define disk_stat_add(gendiskp, field, addnd)			\
	do {							\
		preempt_disable();				\
		__disk_stat_add(gendiskp, field, addnd);	\
		preempt_enable();				\
	} while (0)

#define __disk_stat_dec(gendiskp, field) __disk_stat_add(gendiskp, field, -1)
#define disk_stat_dec(gendiskp, field) disk_stat_add(gendiskp, field, -1)

#define __disk_stat_inc(gendiskp, field) __disk_stat_add(gendiskp, field, 1)
#define disk_stat_inc(gendiskp, field) disk_stat_add(gendiskp, field, 1)

#define __disk_stat_sub(gendiskp, field, subnd) \
		__disk_stat_add(gendiskp, field, -subnd)
#define disk_stat_sub(gendiskp, field, subnd) \
		disk_stat_add(gendiskp, field, -subnd)


#ifdef  CONFIG_SMP
static inline int init_disk_stats(struct gendisk *disk)
{
	disk->dkstats = alloc_percpu(struct disk_stats);
	if (!disk->dkstats)
		return 0;
	return 1;
}

static inline void free_disk_stats(struct gendisk *disk)
{
	free_percpu(disk->dkstats);
}
#else	
static inline int init_disk_stats(struct gendisk *disk)
{
	return 1;
}

static inline void free_disk_stats(struct gendisk *disk)
{
}
#endif	

extern void disk_round_stats(struct gendisk *disk);

extern int get_blkdev_list(char *, int);
extern void add_disk(struct gendisk *disk);
extern void del_gendisk(struct gendisk *gp);
extern void unlink_gendisk(struct gendisk *gp);
extern struct gendisk *get_gendisk(dev_t dev, int *part);

extern void set_device_ro(struct block_device *bdev, int flag);
extern void set_disk_ro(struct gendisk *disk, int flag);

extern void add_disk_randomness(struct gendisk *disk);
extern void rand_initialize_disk(struct gendisk *disk);

static inline sector_t get_start_sect(struct block_device *bdev)
{
	return bdev->bd_contains == bdev ? 0 : bdev->bd_part->start_sect;
}
static inline sector_t get_capacity(struct gendisk *disk)
{
	return disk->capacity;
}
static inline void set_capacity(struct gendisk *disk, sector_t size)
{
	disk->capacity = size;
}

#endif  

#ifdef CONFIG_SOLARIS_X86_PARTITION

#define SOLARIS_X86_NUMSLICE	8
#define SOLARIS_X86_VTOC_SANE	(0x600DDEEEUL)

struct solaris_x86_slice {
	__le16 s_tag;		
	__le16 s_flag;		
	__le32 s_start;		
	__le32 s_size;		
};

struct solaris_x86_vtoc {
	unsigned int v_bootinfo[3];	
	__le32 v_sanity;		
	__le32 v_version;		
	char	v_volume[8];		
	__le16	v_sectorsz;		
	__le16	v_nparts;		
	unsigned int v_reserved[10];	
	struct solaris_x86_slice
		v_slice[SOLARIS_X86_NUMSLICE]; 
	unsigned int timestamp[SOLARIS_X86_NUMSLICE]; 
	char	v_asciilabel[128];	
};

#endif 

#ifdef CONFIG_BSD_DISKLABEL


#define BSD_DISKMAGIC	(0x82564557UL)	
#define BSD_MAXPARTITIONS	16
#define OPENBSD_MAXPARTITIONS	16
#define BSD_FS_UNUSED		0	
struct bsd_disklabel {
	__le32	d_magic;		
	__s16	d_type;			
	__s16	d_subtype;		
	char	d_typename[16];		
	char	d_packname[16];			 
	__u32	d_secsize;		
	__u32	d_nsectors;		
	__u32	d_ntracks;		
	__u32	d_ncylinders;		
	__u32	d_secpercyl;		
	__u32	d_secperunit;		
	__u16	d_sparespertrack;	
	__u16	d_sparespercyl;		
	__u32	d_acylinders;		
	__u16	d_rpm;			
	__u16	d_interleave;		
	__u16	d_trackskew;		
	__u16	d_cylskew;		
	__u32	d_headswitch;		
	__u32	d_trkseek;		
	__u32	d_flags;		
#define NDDATA 5
	__u32	d_drivedata[NDDATA];	
#define NSPARE 5
	__u32	d_spare[NSPARE];	
	__le32	d_magic2;		
	__le16	d_checksum;		

			
	__le16	d_npartitions;		
	__le32	d_bbsize;		
	__le32	d_sbsize;		
	struct	bsd_partition {		
		__le32	p_size;		
		__le32	p_offset;	
		__le32	p_fsize;	
		__u8	p_fstype;	
		__u8	p_frag;		
		__le16	p_cpg;		
	} d_partitions[BSD_MAXPARTITIONS];	
};

#endif	

#ifdef CONFIG_UNIXWARE_DISKLABEL

#define UNIXWARE_DISKMAGIC     (0xCA5E600DUL)	
#define UNIXWARE_DISKMAGIC2    (0x600DDEEEUL)	
#define UNIXWARE_NUMSLICE      16
#define UNIXWARE_FS_UNUSED     0		

struct unixware_slice {
	__le16   s_label;	
	__le16   s_flags;	
	__le32   start_sect;	
	__le32   nr_sects;	
};

struct unixware_disklabel {
	__le32   d_type;               	
	__le32   d_magic;                
	__le32   d_version;              
	char    d_serial[12];           
	__le32   d_ncylinders;           
	__le32   d_ntracks;              
	__le32   d_nsectors;             
	__le32   d_secsize;              
	__le32   d_part_start;           
	__le32   d_unknown1[12];         
 	__le32	d_alt_tbl;              
 	__le32	d_alt_len;              
 	__le32	d_phys_cyl;             
 	__le32	d_phys_trk;             
 	__le32	d_phys_sec;             
 	__le32	d_phys_bytes;           
 	__le32	d_unknown2;             
	__le32   d_unknown3;             
	__le32	d_pad[8];               

	struct unixware_vtoc {
		__le32	v_magic;		
		__le32	v_version;		
		char	v_name[8];		
		__le16	v_nslices;		
		__le16	v_unknown1;		
		__le32	v_reserved[10];		
		struct unixware_slice
			v_slice[UNIXWARE_NUMSLICE];	
	} vtoc;

};  

#endif 

#ifdef CONFIG_MINIX_SUBPARTITION
#   define MINIX_NR_SUBPARTITIONS  4
#endif 

#ifdef __KERNEL__

char *disk_name (struct gendisk *hd, int part, char *buf);

extern int rescan_partitions(struct gendisk *disk, struct block_device *bdev);
extern void add_partition(struct gendisk *, int, sector_t, sector_t);
extern void delete_partition(struct gendisk *, int);

extern struct gendisk *alloc_disk_node(int minors, int node_id);
extern struct gendisk *alloc_disk(int minors);
extern struct kobject *get_disk(struct gendisk *disk);
extern void put_disk(struct gendisk *disk);

extern void blk_register_region(dev_t dev, unsigned long range,
			struct module *module,
			struct kobject *(*probe)(dev_t, int *, void *),
			int (*lock)(dev_t, void *),
			void *data);
extern void blk_unregister_region(dev_t dev, unsigned long range);

static inline struct block_device *bdget_disk(struct gendisk *disk, int index)
{
	return bdget(MKDEV(disk->major, disk->first_minor) + index);
}

#endif

#endif
