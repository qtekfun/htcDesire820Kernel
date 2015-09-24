#ifndef _LINUX_MOUNT_H
#define _LINUX_MOUNT_H
#ifdef __KERNEL__

#include <linux/types.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <asm/atomic.h>

struct super_block;
struct vfsmount;
struct dentry;
struct namespace;

#define MNT_NOSUID	0x01
#define MNT_NODEV	0x02
#define MNT_NOEXEC	0x04
#define MNT_NOATIME	0x08
#define MNT_NODIRATIME	0x10

#define MNT_SHRINKABLE	0x100

#define MNT_SHARED	0x1000	
#define MNT_UNBINDABLE	0x2000	
#define MNT_PNODE_MASK	0x3000	

struct vfsmount {
	struct list_head mnt_hash;
	struct vfsmount *mnt_parent;	
	struct dentry *mnt_mountpoint;	
	struct dentry *mnt_root;	
	struct super_block *mnt_sb;	
	struct list_head mnt_mounts;	
	struct list_head mnt_child;	
	atomic_t mnt_count;
	int mnt_flags;
	int mnt_expiry_mark;		
	char *mnt_devname;		
	struct list_head mnt_list;
	struct list_head mnt_expire;	
	struct list_head mnt_share;	
	struct list_head mnt_slave_list;
	struct list_head mnt_slave;	
	struct vfsmount *mnt_master;	
	struct namespace *mnt_namespace; 
	int mnt_pinned;
};

static inline struct vfsmount *mntget(struct vfsmount *mnt)
{
	if (mnt)
		atomic_inc(&mnt->mnt_count);
	return mnt;
}

extern void mntput_no_expire(struct vfsmount *mnt);
extern void mnt_pin(struct vfsmount *mnt);
extern void mnt_unpin(struct vfsmount *mnt);

static inline void mntput(struct vfsmount *mnt)
{
	if (mnt) {
		mnt->mnt_expiry_mark = 0;
		mntput_no_expire(mnt);
	}
}

extern void free_vfsmnt(struct vfsmount *mnt);
extern struct vfsmount *alloc_vfsmnt(const char *name);
extern struct vfsmount *do_kern_mount(const char *fstype, int flags,
				      const char *name, void *data);

struct file_system_type;
extern struct vfsmount *vfs_kern_mount(struct file_system_type *type,
				      int flags, const char *name,
				      void *data);

struct nameidata;

extern int do_add_mount(struct vfsmount *newmnt, struct nameidata *nd,
			int mnt_flags, struct list_head *fslist);

extern void mark_mounts_for_expiry(struct list_head *mounts);
extern void shrink_submounts(struct vfsmount *mountpoint, struct list_head *mounts);

extern spinlock_t vfsmount_lock;
extern dev_t name_to_dev_t(char *name);

#endif
#endif 
