/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Robert Elz at The University of Melbourne.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * Version: $Id: quota.h,v 2.0 1996/11/17 16:48:14 mvw Exp mvw $
 */

#ifndef _LINUX_QUOTA_
#define _LINUX_QUOTA_

#include <linux/errno.h>
#include <linux/types.h>

#define __DQUOT_VERSION__	"dquot_6.5.1"
#define __DQUOT_NUM_VERSION__	6*10000+5*100+1

typedef __kernel_uid32_t qid_t; 
typedef __u64 qsize_t;          

extern spinlock_t dq_data_lock;

#define QUOTABLOCK_BITS 10
#define QUOTABLOCK_SIZE (1 << QUOTABLOCK_BITS)

#define qb2kb(x) ((x) << (QUOTABLOCK_BITS-10))
#define kb2qb(x) ((x) >> (QUOTABLOCK_BITS-10))
#define toqb(x) (((x) + QUOTABLOCK_SIZE - 1) >> QUOTABLOCK_BITS)

#define MAXQUOTAS 2
#define USRQUOTA  0		
#define GRPQUOTA  1		

#define INITQFNAMES { \
	"user",     \
	"group",    \
	"undefined", \
};

#define SUBCMDMASK  0x00ff
#define SUBCMDSHIFT 8
#define QCMD(cmd, type)  (((cmd) << SUBCMDSHIFT) | ((type) & SUBCMDMASK))

#define Q_SYNC     0x800001	
#define Q_QUOTAON  0x800002	
#define Q_QUOTAOFF 0x800003	
#define Q_GETFMT   0x800004	
#define Q_GETINFO  0x800005	
#define Q_SETINFO  0x800006	
#define Q_GETQUOTA 0x800007	
#define Q_SETQUOTA 0x800008	

#define QIF_BLIMITS	1
#define QIF_SPACE	2
#define QIF_ILIMITS	4
#define QIF_INODES	8
#define QIF_BTIME	16
#define QIF_ITIME	32
#define QIF_LIMITS	(QIF_BLIMITS | QIF_ILIMITS)
#define QIF_USAGE	(QIF_SPACE | QIF_INODES)
#define QIF_TIMES	(QIF_BTIME | QIF_ITIME)
#define QIF_ALL		(QIF_LIMITS | QIF_USAGE | QIF_TIMES)

struct if_dqblk {
	__u64 dqb_bhardlimit;
	__u64 dqb_bsoftlimit;
	__u64 dqb_curspace;
	__u64 dqb_ihardlimit;
	__u64 dqb_isoftlimit;
	__u64 dqb_curinodes;
	__u64 dqb_btime;
	__u64 dqb_itime;
	__u32 dqb_valid;
};

#define IIF_BGRACE	1
#define IIF_IGRACE	2
#define IIF_FLAGS	4
#define IIF_ALL		(IIF_BGRACE | IIF_IGRACE | IIF_FLAGS)

struct if_dqinfo {
	__u64 dqi_bgrace;
	__u64 dqi_igrace;
	__u32 dqi_flags;
	__u32 dqi_valid;
};

#ifdef __KERNEL__
#include <linux/spinlock.h>
#include <linux/mutex.h>

#include <linux/dqblk_xfs.h>
#include <linux/dqblk_v1.h>
#include <linux/dqblk_v2.h>

#define DQUOT_INIT_ALLOC max(V1_INIT_ALLOC, V2_INIT_ALLOC)
#define DQUOT_INIT_REWRITE max(V1_INIT_REWRITE, V2_INIT_REWRITE)
#define DQUOT_DEL_ALLOC max(V1_DEL_ALLOC, V2_DEL_ALLOC)
#define DQUOT_DEL_REWRITE max(V1_DEL_REWRITE, V2_DEL_REWRITE)

struct mem_dqblk {
	__u32 dqb_bhardlimit;	
	__u32 dqb_bsoftlimit;	
	qsize_t dqb_curspace;	
	__u32 dqb_ihardlimit;	
	__u32 dqb_isoftlimit;	
	__u32 dqb_curinodes;	
	time_t dqb_btime;	
	time_t dqb_itime;	
};

struct quota_format_type;

struct mem_dqinfo {
	struct quota_format_type *dqi_format;
	struct list_head dqi_dirty_list;	
	unsigned long dqi_flags;
	unsigned int dqi_bgrace;
	unsigned int dqi_igrace;
	union {
		struct v1_mem_dqinfo v1_i;
		struct v2_mem_dqinfo v2_i;
	} u;
};

struct super_block;

#define DQF_MASK 0xffff		
#define DQF_INFO_DIRTY_B 16
#define DQF_INFO_DIRTY (1 << DQF_INFO_DIRTY_B)	

extern void mark_info_dirty(struct super_block *sb, int type);
#define info_dirty(info) test_bit(DQF_INFO_DIRTY_B, &(info)->dqi_flags)
#define info_any_dquot_dirty(info) (!list_empty(&(info)->dqi_dirty_list))
#define info_any_dirty(info) (info_dirty(info) || info_any_dquot_dirty(info))

#define sb_dqopt(sb) (&(sb)->s_dquot)
#define sb_dqinfo(sb, type) (sb_dqopt(sb)->info+(type))

struct dqstats {
	int lookups;
	int drops;
	int reads;
	int writes;
	int cache_hits;
	int allocated_dquots;
	int free_dquots;
	int syncs;
};

extern struct dqstats dqstats;

#define DQ_MOD_B	0	
#define DQ_BLKS_B	1	
#define DQ_INODES_B	2	
#define DQ_FAKE_B	3	
#define DQ_READ_B	4	
#define DQ_ACTIVE_B	5	

struct dquot {
	struct hlist_node dq_hash;	
	struct list_head dq_inuse;	
	struct list_head dq_free;	
	struct list_head dq_dirty;	
	struct mutex dq_lock;		
	atomic_t dq_count;		
	wait_queue_head_t dq_wait_unused;	
	struct super_block *dq_sb;	
	unsigned int dq_id;		
	loff_t dq_off;			
	unsigned long dq_flags;		
	short dq_type;			
	struct mem_dqblk dq_dqb;	
};

#define NODQUOT (struct dquot *)NULL

#define QUOTA_OK          0
#define NO_QUOTA          1

struct quota_format_ops {
	int (*check_quota_file)(struct super_block *sb, int type);	
	int (*read_file_info)(struct super_block *sb, int type);	
	int (*write_file_info)(struct super_block *sb, int type);	
	int (*free_file_info)(struct super_block *sb, int type);	
	int (*read_dqblk)(struct dquot *dquot);		
	int (*commit_dqblk)(struct dquot *dquot);	
	int (*release_dqblk)(struct dquot *dquot);	
};

struct dquot_operations {
	int (*initialize) (struct inode *, int);
	int (*drop) (struct inode *);
	int (*alloc_space) (struct inode *, qsize_t, int);
	int (*alloc_inode) (const struct inode *, unsigned long);
	int (*free_space) (struct inode *, qsize_t);
	int (*free_inode) (const struct inode *, unsigned long);
	int (*transfer) (struct inode *, struct iattr *);
	int (*write_dquot) (struct dquot *);		
	int (*acquire_dquot) (struct dquot *);		
	int (*release_dquot) (struct dquot *);		
	int (*mark_dirty) (struct dquot *);		
	int (*write_info) (struct super_block *, int);	
};

struct quotactl_ops {
	int (*quota_on)(struct super_block *, int, int, char *);
	int (*quota_off)(struct super_block *, int);
	int (*quota_sync)(struct super_block *, int);
	int (*get_info)(struct super_block *, int, struct if_dqinfo *);
	int (*set_info)(struct super_block *, int, struct if_dqinfo *);
	int (*get_dqblk)(struct super_block *, int, qid_t, struct if_dqblk *);
	int (*set_dqblk)(struct super_block *, int, qid_t, struct if_dqblk *);
	int (*get_xstate)(struct super_block *, struct fs_quota_stat *);
	int (*set_xstate)(struct super_block *, unsigned int, int);
	int (*get_xquota)(struct super_block *, int, qid_t, struct fs_disk_quota *);
	int (*set_xquota)(struct super_block *, int, qid_t, struct fs_disk_quota *);
};

struct quota_format_type {
	int qf_fmt_id;	
	struct quota_format_ops *qf_ops;	
	struct module *qf_owner;		
	struct quota_format_type *qf_next;
};

#define DQUOT_USR_ENABLED	0x01		
#define DQUOT_GRP_ENABLED	0x02		

struct quota_info {
	unsigned int flags;			
	struct mutex dqio_mutex;		
	struct mutex dqonoff_mutex;		
	struct rw_semaphore dqptr_sem;		
	struct inode *files[MAXQUOTAS];		
	struct mem_dqinfo info[MAXQUOTAS];	
	struct quota_format_ops *ops[MAXQUOTAS];	
};

int mark_dquot_dirty(struct dquot *dquot);

#define dquot_dirty(dquot) test_bit(DQ_MOD_B, &(dquot)->dq_flags)

#define sb_has_quota_enabled(sb, type) ((type)==USRQUOTA ? \
	(sb_dqopt(sb)->flags & DQUOT_USR_ENABLED) : (sb_dqopt(sb)->flags & DQUOT_GRP_ENABLED))

#define sb_any_quota_enabled(sb) (sb_has_quota_enabled(sb, USRQUOTA) | \
				  sb_has_quota_enabled(sb, GRPQUOTA))

int register_quota_format(struct quota_format_type *fmt);
void unregister_quota_format(struct quota_format_type *fmt);

struct quota_module_name {
	int qm_fmt_id;
	char *qm_mod_name;
};

#define INIT_QUOTA_MODULE_NAMES {\
	{QFMT_VFS_OLD, "quota_v1"},\
	{QFMT_VFS_V0, "quota_v2"},\
	{0, NULL}}

#else

#  include <sys/cdefs.h>

__BEGIN_DECLS
long quotactl __P ((unsigned int, const char *, int, caddr_t));
__END_DECLS

#endif 
#endif 
