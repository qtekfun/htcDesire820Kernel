/*
 * include/linux/nfsd/nfsfh.h
 *
 * This file describes the layout of the file handles as passed
 * over the wire.
 *
 * Earlier versions of knfsd used to sign file handles using keyed MD5
 * or SHA. I've removed this code, because it doesn't give you more
 * security than blocking external access to port 2049 on your firewall.
 *
 * Copyright (C) 1995, 1996, 1997 Olaf Kirch <okir@monad.swb.de>
 */

#ifndef _LINUX_NFSD_FH_H
#define _LINUX_NFSD_FH_H

#include <asm/types.h>
#ifdef __KERNEL__
# include <linux/types.h>
# include <linux/string.h>
# include <linux/fs.h>
#endif
#include <linux/nfsd/const.h>
#include <linux/nfsd/debug.h>

struct nfs_fhbase_old {
	__u32		fb_dcookie;	
	__u32		fb_ino;		
	__u32		fb_dirino;	
	__u32		fb_dev;		
	__u32		fb_xdev;
	__u32		fb_xino;
	__u32		fb_generation;
};

struct nfs_fhbase_new {
	__u8		fb_version;	
	__u8		fb_auth_type;
	__u8		fb_fsid_type;
	__u8		fb_fileid_type;
	__u32		fb_auth[1];
};

struct knfsd_fh {
	unsigned int	fh_size;	
	union {
		struct nfs_fhbase_old	fh_old;
		__u32			fh_pad[NFS4_FHSIZE/4];
		struct nfs_fhbase_new	fh_new;
	} fh_base;
};

#define ofh_dcookie		fh_base.fh_old.fb_dcookie
#define ofh_ino			fh_base.fh_old.fb_ino
#define ofh_dirino		fh_base.fh_old.fb_dirino
#define ofh_dev			fh_base.fh_old.fb_dev
#define ofh_xdev		fh_base.fh_old.fb_xdev
#define ofh_xino		fh_base.fh_old.fb_xino
#define ofh_generation		fh_base.fh_old.fb_generation

#define	fh_version		fh_base.fh_new.fb_version
#define	fh_fsid_type		fh_base.fh_new.fb_fsid_type
#define	fh_auth_type		fh_base.fh_new.fb_auth_type
#define	fh_fileid_type		fh_base.fh_new.fb_fileid_type
#define	fh_auth			fh_base.fh_new.fb_auth
#define	fh_fsid			fh_base.fh_new.fb_auth

#ifdef __KERNEL__

static inline __u32 ino_t_to_u32(ino_t ino)
{
	return (__u32) ino;
}

static inline ino_t u32_to_ino_t(__u32 uino)
{
	return (ino_t) uino;
}

typedef struct svc_fh {
	struct knfsd_fh		fh_handle;	
	struct dentry *		fh_dentry;	
	struct svc_export *	fh_export;	
	int			fh_maxsize;	

	unsigned char		fh_locked;	

#ifdef CONFIG_NFSD_V3
	unsigned char		fh_post_saved;	
	unsigned char		fh_pre_saved;	

	
	__u64			fh_pre_size;	
	struct timespec		fh_pre_mtime;	
	struct timespec		fh_pre_ctime;	

	
	umode_t			fh_post_mode;	
	nlink_t			fh_post_nlink;	
	uid_t			fh_post_uid;	
	gid_t			fh_post_gid;	
	__u64			fh_post_size;	
	unsigned long		fh_post_blocks; 
	unsigned long		fh_post_blksize;
	__u32			fh_post_rdev[2];
	struct timespec		fh_post_atime;	
	struct timespec		fh_post_mtime;	
	struct timespec		fh_post_ctime;	
#endif 

} svc_fh;

static inline void mk_fsid_v0(u32 *fsidv, dev_t dev, ino_t ino)
{
	fsidv[0] = htonl((MAJOR(dev)<<16) |
			MINOR(dev));
	fsidv[1] = ino_t_to_u32(ino);
}

static inline void mk_fsid_v1(u32 *fsidv, u32 fsid)
{
	fsidv[0] = fsid;
}

static inline void mk_fsid_v2(u32 *fsidv, dev_t dev, ino_t ino)
{
	fsidv[0] = htonl(MAJOR(dev));
	fsidv[1] = htonl(MINOR(dev));
	fsidv[2] = ino_t_to_u32(ino);
}

static inline void mk_fsid_v3(u32 *fsidv, dev_t dev, ino_t ino)
{
	fsidv[0] = new_encode_dev(dev);
	fsidv[1] = ino_t_to_u32(ino);
}

static inline int key_len(int type)
{
	switch(type) {
	case 0: return 8;
	case 1: return 4;
	case 2: return 12;
	case 3: return 8;
	default: return 0;
	}
}

extern char * SVCFH_fmt(struct svc_fh *fhp);

u32	fh_verify(struct svc_rqst *, struct svc_fh *, int, int);
int	fh_compose(struct svc_fh *, struct svc_export *, struct dentry *, struct svc_fh *);
int	fh_update(struct svc_fh *);
void	fh_put(struct svc_fh *);

static __inline__ struct svc_fh *
fh_copy(struct svc_fh *dst, struct svc_fh *src)
{
	if (src->fh_dentry || src->fh_locked) {
		struct dentry *dentry = src->fh_dentry;
		printk(KERN_ERR "fh_copy: copying %s/%s, already verified!\n",
			dentry->d_parent->d_name.name, dentry->d_name.name);
	}
			
	*dst = *src;
	return dst;
}

static __inline__ struct svc_fh *
fh_init(struct svc_fh *fhp, int maxsize)
{
	memset(fhp, 0, sizeof(*fhp));
	fhp->fh_maxsize = maxsize;
	return fhp;
}

#ifdef CONFIG_NFSD_V3
static inline void
fill_pre_wcc(struct svc_fh *fhp)
{
	struct inode    *inode;

	inode = fhp->fh_dentry->d_inode;
	if (!fhp->fh_pre_saved) {
		fhp->fh_pre_mtime = inode->i_mtime;
		fhp->fh_pre_ctime = inode->i_ctime;
			fhp->fh_pre_size  = inode->i_size;
			fhp->fh_pre_saved = 1;
	}
}

static inline void
fill_post_wcc(struct svc_fh *fhp)
{
	struct inode    *inode = fhp->fh_dentry->d_inode;

	if (fhp->fh_post_saved)
		printk("nfsd: inode locked twice during operation.\n");

	fhp->fh_post_mode       = inode->i_mode;
	fhp->fh_post_nlink      = inode->i_nlink;
	fhp->fh_post_uid	= inode->i_uid;
	fhp->fh_post_gid	= inode->i_gid;
	fhp->fh_post_size       = inode->i_size;
	if (inode->i_blksize) {
		fhp->fh_post_blksize    = inode->i_blksize;
		fhp->fh_post_blocks     = inode->i_blocks;
	} else {
		fhp->fh_post_blksize    = BLOCK_SIZE;
		
		fhp->fh_post_blocks     = (inode->i_size+511) >> 9;
	}
	fhp->fh_post_rdev[0]    = htonl((u32)imajor(inode));
	fhp->fh_post_rdev[1]    = htonl((u32)iminor(inode));
	fhp->fh_post_atime      = inode->i_atime;
	fhp->fh_post_mtime      = inode->i_mtime;
	fhp->fh_post_ctime      = inode->i_ctime;
	fhp->fh_post_saved      = 1;
}
#else
#define	fill_pre_wcc(ignored)
#define fill_post_wcc(notused)
#endif 


static inline void
fh_lock(struct svc_fh *fhp)
{
	struct dentry	*dentry = fhp->fh_dentry;
	struct inode	*inode;

	dfprintk(FILEOP, "nfsd: fh_lock(%s) locked = %d\n",
			SVCFH_fmt(fhp), fhp->fh_locked);

	if (!fhp->fh_dentry) {
		printk(KERN_ERR "fh_lock: fh not verified!\n");
		return;
	}
	if (fhp->fh_locked) {
		printk(KERN_WARNING "fh_lock: %s/%s already locked!\n",
			dentry->d_parent->d_name.name, dentry->d_name.name);
		return;
	}

	inode = dentry->d_inode;
	mutex_lock(&inode->i_mutex);
	fill_pre_wcc(fhp);
	fhp->fh_locked = 1;
}

static inline void
fh_unlock(struct svc_fh *fhp)
{
	if (!fhp->fh_dentry)
		printk(KERN_ERR "fh_unlock: fh not verified!\n");

	if (fhp->fh_locked) {
		fill_post_wcc(fhp);
		mutex_unlock(&fhp->fh_dentry->d_inode->i_mutex);
		fhp->fh_locked = 0;
	}
}
#endif 


#endif 