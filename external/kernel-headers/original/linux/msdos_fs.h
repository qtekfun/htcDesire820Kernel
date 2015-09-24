#ifndef _LINUX_MSDOS_FS_H
#define _LINUX_MSDOS_FS_H

#include <linux/magic.h>

#include <asm/byteorder.h>

#define SECTOR_SIZE	512		
#define SECTOR_BITS	9		
#define MSDOS_DPB	(MSDOS_DPS)	
#define MSDOS_DPB_BITS	4		
#define MSDOS_DPS	(SECTOR_SIZE / sizeof(struct msdos_dir_entry))
#define MSDOS_DPS_BITS	4		
#define CF_LE_W(v)	le16_to_cpu(v)
#define CF_LE_L(v)	le32_to_cpu(v)
#define CT_LE_W(v)	cpu_to_le16(v)
#define CT_LE_L(v)	cpu_to_le32(v)


#define MSDOS_ROOT_INO	1	
#define MSDOS_DIR_BITS	5	

#define FAT_MAX_DIR_ENTRIES	(65536)
#define FAT_MAX_DIR_SIZE	(FAT_MAX_DIR_ENTRIES << MSDOS_DIR_BITS)

#define ATTR_NONE	0	
#define ATTR_RO		1	
#define ATTR_HIDDEN	2	
#define ATTR_SYS	4	
#define ATTR_VOLUME	8	
#define ATTR_DIR	16	
#define ATTR_ARCH	32	

#define ATTR_UNUSED	(ATTR_VOLUME | ATTR_ARCH | ATTR_SYS | ATTR_HIDDEN)
#define ATTR_EXT	(ATTR_RO | ATTR_HIDDEN | ATTR_SYS | ATTR_VOLUME)

#define CASE_LOWER_BASE	8	
#define CASE_LOWER_EXT	16	

#define DELETED_FLAG	0xe5	
#define IS_FREE(n)	(!*(n) || *(n) == DELETED_FLAG)

#define MSDOS_VALID_MODE (S_IFREG | S_IFDIR | S_IRWXU | S_IRWXG | S_IRWXO)
#define MSDOS_MKMODE(a, m) (m & (a & ATTR_RO ? S_IRUGO|S_IXUGO : S_IRWXUGO))

#define MSDOS_NAME	11	
#define MSDOS_LONGNAME	256	
#define MSDOS_SLOTS	21	
#define MSDOS_DOT	".          "	
#define MSDOS_DOTDOT	"..         "	

#define FAT_VALID_MEDIA(x)	((0xF8 <= (x) && (x) <= 0xFF) || (x) == 0xF0)
#define FAT_FIRST_ENT(s, x)	((MSDOS_SB(s)->fat_bits == 32 ? 0x0FFFFF00 : \
	MSDOS_SB(s)->fat_bits == 16 ? 0xFF00 : 0xF00) | (x))

#define FAT_START_ENT	2

#define MAX_FAT12	0xFF4
#define MAX_FAT16	0xFFF4
#define MAX_FAT32	0x0FFFFFF6
#define MAX_FAT(s)	(MSDOS_SB(s)->fat_bits == 32 ? MAX_FAT32 : \
	MSDOS_SB(s)->fat_bits == 16 ? MAX_FAT16 : MAX_FAT12)

#define BAD_FAT12	0xFF7
#define BAD_FAT16	0xFFF7
#define BAD_FAT32	0x0FFFFFF7

#define EOF_FAT12	0xFFF
#define EOF_FAT16	0xFFFF
#define EOF_FAT32	0x0FFFFFFF

#define FAT_ENT_FREE	(0)
#define FAT_ENT_BAD	(BAD_FAT32)
#define FAT_ENT_EOF	(EOF_FAT32)

#define FAT_FSINFO_SIG1	0x41615252
#define FAT_FSINFO_SIG2	0x61417272
#define IS_FSINFO(x)	(le32_to_cpu((x)->signature1) == FAT_FSINFO_SIG1 \
			 && le32_to_cpu((x)->signature2) == FAT_FSINFO_SIG2)

#define VFAT_IOCTL_READDIR_BOTH		_IOR('r', 1, struct dirent [2])
#define VFAT_IOCTL_READDIR_SHORT	_IOR('r', 2, struct dirent [2])
#define FAT_IOCTL_GET_ATTRIBUTES	_IOR('r', 0x10, __u32)
#define FAT_IOCTL_SET_ATTRIBUTES	_IOW('r', 0x11, __u32)
#define VFAT_IOCTL_GET_VOLUME_ID	_IOR('r', 0x12, __u32)

#define VFAT_SFN_DISPLAY_LOWER	0x0001 
#define VFAT_SFN_DISPLAY_WIN95	0x0002 
#define VFAT_SFN_DISPLAY_WINNT	0x0004 
#define VFAT_SFN_CREATE_WIN95	0x0100 
#define VFAT_SFN_CREATE_WINNT	0x0200 

struct fat_boot_sector {
	__u8	ignored[3];	
	__u8	system_id[8];	
	__u8	sector_size[2];	
	__u8	sec_per_clus;	
	__le16	reserved;	
	__u8	fats;		
	__u8	dir_entries[2];	
	__u8	sectors[2];	
	__u8	media;		
	__le16	fat_length;	
	__le16	secs_track;	
	__le16	heads;		
	__le32	hidden;		
	__le32	total_sect;	

	
	__le32	fat32_length;	
	__le16	flags;		
	__u8	version[2];	
	__le32	root_cluster;	
	__le16	info_sector;	
	__le16	backup_boot;	
	__le16	reserved2[6];	
};

struct fat_boot_fsinfo {
	__le32   signature1;	
	__le32   reserved1[120];	
	__le32   signature2;	
	__le32   free_clusters;	
	__le32   next_cluster;	
	__le32   reserved2[4];
};

struct fat_boot_bsx {
	__u8     drive;		    
	__u8     reserved1;
	__u8     signature;	    
	__u8     vol_id[4];     
	__u8     vol_label[11]; 
	__u8     type[8];       
};
#define FAT16_BSX_OFFSET	36 
#define FAT32_BSX_OFFSET	64 

struct msdos_dir_entry {
	__u8	name[MSDOS_NAME];
	__u8	attr;		
	__u8    lcase;		
	__u8	ctime_cs;	
	__le16	ctime;		
	__le16	cdate;		
	__le16	adate;		
	__le16	starthi;	
	__le16	time,date,start;
	__le32	size;		
};

struct msdos_dir_slot {
	__u8    id;		
	__u8    name0_4[10];	
	__u8    attr;		
	__u8    reserved;	
	__u8    alias_checksum;	
	__u8    name5_10[12];	
	__le16   start;		
	__u8    name11_12[4];	
};

struct fat_slot_info {
	loff_t i_pos;		
	loff_t slot_off;	
	int nr_slots;		
	struct msdos_dir_entry *de;
	struct buffer_head *bh;
};

#ifdef __KERNEL__

#include <linux/buffer_head.h>
#include <linux/string.h>
#include <linux/nls.h>
#include <linux/fs.h>
#include <linux/mutex.h>

struct fat_mount_options {
	uid_t fs_uid;
	gid_t fs_gid;
	unsigned short fs_fmask;
	unsigned short fs_dmask;
	unsigned short codepage;  
	char *iocharset;          
	unsigned short shortname; 
	unsigned char name_check; 
	unsigned quiet:1,         
		 showexec:1,      
		 sys_immutable:1, 
		 dotsOK:1,        
		 isvfat:1,        
		 utf8:1,	  
		 unicode_xlate:1, 
		 numtail:1,       
		 atari:1,         
		 flush:1,	  
		 nocase:1,	  
		 usefree:1;	  
};

#define FAT_HASH_BITS	8
#define FAT_HASH_SIZE	(1UL << FAT_HASH_BITS)
#define FAT_HASH_MASK	(FAT_HASH_SIZE-1)

struct msdos_sb_info {
	unsigned short sec_per_clus; 
	unsigned short cluster_bits; 
	unsigned int cluster_size;   
	unsigned char fats,fat_bits; 
	unsigned short fat_start;
	unsigned long fat_length;    
	unsigned long dir_start;
	unsigned short dir_entries;  
	unsigned long data_start;    
	unsigned long max_cluster;   
	unsigned long root_cluster;  
	unsigned long fsinfo_sector; 
	struct mutex fat_lock;
	unsigned int prev_free;      
	unsigned int free_clusters;  
	struct fat_mount_options options;
	struct nls_table *nls_disk;  
	struct nls_table *nls_io;    
	const void *dir_ops;		     
	int dir_per_block;	     
	int dir_per_block_bits;	     
	unsigned long vol_id;        

	int fatent_shift;
	struct fatent_operations *fatent_ops;

	spinlock_t inode_hash_lock;
	struct hlist_head inode_hashtable[FAT_HASH_SIZE];
};

#define FAT_CACHE_VALID	0	

struct msdos_inode_info {
	spinlock_t cache_lru_lock;
	struct list_head cache_lru;
	int nr_caches;
	
	unsigned int cache_valid_id;

	loff_t mmu_private;
	int i_start;		
	int i_logstart;		
	int i_attrs;		
	loff_t i_pos;		
	struct hlist_node i_fat_hash;	
	struct inode vfs_inode;
};

static inline struct msdos_sb_info *MSDOS_SB(struct super_block *sb)
{
	return sb->s_fs_info;
}

static inline struct msdos_inode_info *MSDOS_I(struct inode *inode)
{
	return container_of(inode, struct msdos_inode_info, vfs_inode);
}

static inline u8 fat_attr(struct inode *inode)
{
	return ((inode->i_mode & S_IWUGO) ? ATTR_NONE : ATTR_RO) |
		(S_ISDIR(inode->i_mode) ? ATTR_DIR : ATTR_NONE) |
		MSDOS_I(inode)->i_attrs;
}

static inline unsigned char fat_checksum(const __u8 *name)
{
	unsigned char s = name[0];
	s = (s<<7) + (s>>1) + name[1];	s = (s<<7) + (s>>1) + name[2];
	s = (s<<7) + (s>>1) + name[3];	s = (s<<7) + (s>>1) + name[4];
	s = (s<<7) + (s>>1) + name[5];	s = (s<<7) + (s>>1) + name[6];
	s = (s<<7) + (s>>1) + name[7];	s = (s<<7) + (s>>1) + name[8];
	s = (s<<7) + (s>>1) + name[9];	s = (s<<7) + (s>>1) + name[10];
	return s;
}

static inline sector_t fat_clus_to_blknr(struct msdos_sb_info *sbi, int clus)
{
	return ((sector_t)clus - FAT_START_ENT) * sbi->sec_per_clus
		+ sbi->data_start;
}

static inline void fat16_towchar(wchar_t *dst, const __u8 *src, size_t len)
{
#ifdef __BIG_ENDIAN
	while (len--) {
		*dst++ = src[0] | (src[1] << 8);
		src += 2;
	}
#else
	memcpy(dst, src, len * 2);
#endif
}

static inline void fatwchar_to16(__u8 *dst, const wchar_t *src, size_t len)
{
#ifdef __BIG_ENDIAN
	while (len--) {
		dst[0] = *src & 0x00FF;
		dst[1] = (*src & 0xFF00) >> 8;
		dst += 2;
		src++;
	}
#else
	memcpy(dst, src, len * 2);
#endif
}

extern void fat_cache_inval_inode(struct inode *inode);
extern int fat_get_cluster(struct inode *inode, int cluster,
			   int *fclus, int *dclus);
extern int fat_bmap(struct inode *inode, sector_t sector, sector_t *phys,
		    unsigned long *mapped_blocks);

extern const struct file_operations fat_dir_operations;
extern int fat_search_long(struct inode *inode, const unsigned char *name,
			   int name_len, struct fat_slot_info *sinfo);
extern int fat_dir_empty(struct inode *dir);
extern int fat_subdirs(struct inode *dir);
extern int fat_scan(struct inode *dir, const unsigned char *name,
		    struct fat_slot_info *sinfo);
extern int fat_get_dotdot_entry(struct inode *dir, struct buffer_head **bh,
				struct msdos_dir_entry **de, loff_t *i_pos);
extern int fat_alloc_new_dir(struct inode *dir, struct timespec *ts);
extern int fat_add_entries(struct inode *dir, void *slots, int nr_slots,
			   struct fat_slot_info *sinfo);
extern int fat_remove_entries(struct inode *dir, struct fat_slot_info *sinfo);

struct fat_entry {
	int entry;
	union {
		u8 *ent12_p[2];
		__le16 *ent16_p;
		__le32 *ent32_p;
	} u;
	int nr_bhs;
	struct buffer_head *bhs[2];
};

static inline void fatent_init(struct fat_entry *fatent)
{
	fatent->nr_bhs = 0;
	fatent->entry = 0;
	fatent->u.ent32_p = NULL;
	fatent->bhs[0] = fatent->bhs[1] = NULL;
}

static inline void fatent_set_entry(struct fat_entry *fatent, int entry)
{
	fatent->entry = entry;
	fatent->u.ent32_p = NULL;
}

static inline void fatent_brelse(struct fat_entry *fatent)
{
	int i;
	fatent->u.ent32_p = NULL;
	for (i = 0; i < fatent->nr_bhs; i++)
		brelse(fatent->bhs[i]);
	fatent->nr_bhs = 0;
	fatent->bhs[0] = fatent->bhs[1] = NULL;
}

extern void fat_ent_access_init(struct super_block *sb);
extern int fat_ent_read(struct inode *inode, struct fat_entry *fatent,
			int entry);
extern int fat_ent_write(struct inode *inode, struct fat_entry *fatent,
			 int new, int wait);
extern int fat_alloc_clusters(struct inode *inode, int *cluster,
			      int nr_cluster);
extern int fat_free_clusters(struct inode *inode, int cluster);
extern int fat_count_free_clusters(struct super_block *sb);

extern int fat_generic_ioctl(struct inode *inode, struct file *filp,
			     unsigned int cmd, unsigned long arg);
extern const struct file_operations fat_file_operations;
extern const struct inode_operations fat_file_inode_operations;
extern int fat_notify_change(struct dentry * dentry, struct iattr * attr);
extern void fat_truncate(struct inode *inode);
extern int fat_getattr(struct vfsmount *mnt, struct dentry *dentry,
		       struct kstat *stat);

extern void fat_attach(struct inode *inode, loff_t i_pos);
extern void fat_detach(struct inode *inode);
extern struct inode *fat_iget(struct super_block *sb, loff_t i_pos);
extern struct inode *fat_build_inode(struct super_block *sb,
			struct msdos_dir_entry *de, loff_t i_pos);
extern int fat_sync_inode(struct inode *inode);
extern int fat_fill_super(struct super_block *sb, void *data, int silent,
			const struct inode_operations *fs_dir_inode_ops, int isvfat);

extern int fat_flush_inodes(struct super_block *sb, struct inode *i1,
		            struct inode *i2);
extern void fat_fs_panic(struct super_block *s, const char *fmt, ...);
extern void fat_clusters_flush(struct super_block *sb);
extern int fat_chain_add(struct inode *inode, int new_dclus, int nr_cluster);
extern int date_dos2unix(unsigned short time, unsigned short date);
extern void fat_date_unix2dos(int unix_date, __le16 *time, __le16 *date);
extern int fat_sync_bhs(struct buffer_head **bhs, int nr_bhs);

int fat_cache_init(void);
void fat_cache_destroy(void);

#endif 

#endif
