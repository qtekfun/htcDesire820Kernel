/* key.h: authentication token and access key management
 *
 * Copyright (C) 2004 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
 *
 * See Documentation/keys.txt for information on keys/keyrings.
 */

#ifndef _LINUX_KEY_H
#define _LINUX_KEY_H

#include <linux/types.h>
#include <linux/list.h>
#include <linux/rbtree.h>
#include <linux/rcupdate.h>
#include <asm/atomic.h>

#ifdef __KERNEL__

typedef int32_t key_serial_t;

typedef uint32_t key_perm_t;

struct key;

#ifdef CONFIG_KEYS

#undef KEY_DEBUGGING

#define KEY_POS_VIEW	0x01000000	
#define KEY_POS_READ	0x02000000	
#define KEY_POS_WRITE	0x04000000	
#define KEY_POS_SEARCH	0x08000000	
#define KEY_POS_LINK	0x10000000	
#define KEY_POS_SETATTR	0x20000000	
#define KEY_POS_ALL	0x3f000000

#define KEY_USR_VIEW	0x00010000	
#define KEY_USR_READ	0x00020000
#define KEY_USR_WRITE	0x00040000
#define KEY_USR_SEARCH	0x00080000
#define KEY_USR_LINK	0x00100000
#define KEY_USR_SETATTR	0x00200000
#define KEY_USR_ALL	0x003f0000

#define KEY_GRP_VIEW	0x00000100	
#define KEY_GRP_READ	0x00000200
#define KEY_GRP_WRITE	0x00000400
#define KEY_GRP_SEARCH	0x00000800
#define KEY_GRP_LINK	0x00001000
#define KEY_GRP_SETATTR	0x00002000
#define KEY_GRP_ALL	0x00003f00

#define KEY_OTH_VIEW	0x00000001	
#define KEY_OTH_READ	0x00000002
#define KEY_OTH_WRITE	0x00000004
#define KEY_OTH_SEARCH	0x00000008
#define KEY_OTH_LINK	0x00000010
#define KEY_OTH_SETATTR	0x00000020
#define KEY_OTH_ALL	0x0000003f

struct seq_file;
struct user_struct;
struct signal_struct;

struct key_type;
struct key_owner;
struct keyring_list;
struct keyring_name;

typedef struct __key_reference_with_attributes *key_ref_t;

static inline key_ref_t make_key_ref(const struct key *key,
				     unsigned long possession)
{
	return (key_ref_t) ((unsigned long) key | possession);
}

static inline struct key *key_ref_to_ptr(const key_ref_t key_ref)
{
	return (struct key *) ((unsigned long) key_ref & ~1UL);
}

static inline unsigned long is_key_possessed(const key_ref_t key_ref)
{
	return (unsigned long) key_ref & 1UL;
}

struct key {
	atomic_t		usage;		
	key_serial_t		serial;		
	struct rb_node		serial_node;
	struct key_type		*type;		
	struct rw_semaphore	sem;		
	struct key_user		*user;		
	void			*security;	
	time_t			expiry;		
	uid_t			uid;
	gid_t			gid;
	key_perm_t		perm;		
	unsigned short		quotalen;	
	unsigned short		datalen;	

#ifdef KEY_DEBUGGING
	unsigned		magic;
#define KEY_DEBUG_MAGIC		0x18273645u
#define KEY_DEBUG_MAGIC_X	0xf8e9dacbu
#endif

	unsigned long		flags;		
#define KEY_FLAG_INSTANTIATED	0	
#define KEY_FLAG_DEAD		1	
#define KEY_FLAG_REVOKED	2	
#define KEY_FLAG_IN_QUOTA	3	
#define KEY_FLAG_USER_CONSTRUCT	4	
#define KEY_FLAG_NEGATIVE	5	

	char			*description;

	union {
		struct list_head	link;
	} type_data;

	union {
		unsigned long		value;
		void			*data;
		struct keyring_list	*subscriptions;
	} payload;
};

typedef int (*request_key_actor_t)(struct key *key, struct key *authkey,
				   const char *op, void *aux);

struct key_type {
	
	const char *name;

	size_t def_datalen;

	int (*instantiate)(struct key *key, const void *data, size_t datalen);

	int (*update)(struct key *key, const void *data, size_t datalen);

	
	int (*match)(const struct key *key, const void *desc);

	void (*revoke)(struct key *key);

	
	void (*destroy)(struct key *key);

	
	void (*describe)(const struct key *key, struct seq_file *p);

	long (*read)(const struct key *key, char __user *buffer, size_t buflen);

	request_key_actor_t request_key;

	
	struct list_head	link;		
};

extern struct key_type key_type_keyring;

extern int register_key_type(struct key_type *ktype);
extern void unregister_key_type(struct key_type *ktype);

extern struct key *key_alloc(struct key_type *type,
			     const char *desc,
			     uid_t uid, gid_t gid,
			     struct task_struct *ctx,
			     key_perm_t perm,
			     unsigned long flags);


#define KEY_ALLOC_IN_QUOTA	0x0000	
#define KEY_ALLOC_QUOTA_OVERRUN	0x0001	
#define KEY_ALLOC_NOT_IN_QUOTA	0x0002	

extern int key_payload_reserve(struct key *key, size_t datalen);
extern int key_instantiate_and_link(struct key *key,
				    const void *data,
				    size_t datalen,
				    struct key *keyring,
				    struct key *instkey);
extern int key_negate_and_link(struct key *key,
			       unsigned timeout,
			       struct key *keyring,
			       struct key *instkey);
extern void key_revoke(struct key *key);
extern void key_put(struct key *key);

static inline struct key *key_get(struct key *key)
{
	if (key)
		atomic_inc(&key->usage);
	return key;
}

static inline void key_ref_put(key_ref_t key_ref)
{
	key_put(key_ref_to_ptr(key_ref));
}

extern struct key *request_key(struct key_type *type,
			       const char *description,
			       const char *callout_info);

extern struct key *request_key_with_auxdata(struct key_type *type,
					    const char *description,
					    const char *callout_info,
					    void *aux);

extern int key_validate(struct key *key);

extern key_ref_t key_create_or_update(key_ref_t keyring,
				      const char *type,
				      const char *description,
				      const void *payload,
				      size_t plen,
				      unsigned long flags);

extern int key_update(key_ref_t key,
		      const void *payload,
		      size_t plen);

extern int key_link(struct key *keyring,
		    struct key *key);

extern int key_unlink(struct key *keyring,
		      struct key *key);

extern struct key *keyring_alloc(const char *description, uid_t uid, gid_t gid,
				 struct task_struct *ctx,
				 unsigned long flags,
				 struct key *dest);

extern int keyring_clear(struct key *keyring);

extern key_ref_t keyring_search(key_ref_t keyring,
				struct key_type *type,
				const char *description);

extern int keyring_add_key(struct key *keyring,
			   struct key *key);

extern struct key *key_lookup(key_serial_t id);

extern void keyring_replace_payload(struct key *key, void *replacement);

#define key_serial(key) ((key) ? (key)->serial : 0)

extern struct key root_user_keyring, root_session_keyring;
extern int alloc_uid_keyring(struct user_struct *user,
			     struct task_struct *ctx);
extern void switch_uid_keyring(struct user_struct *new_user);
extern int copy_keys(unsigned long clone_flags, struct task_struct *tsk);
extern int copy_thread_group_keys(struct task_struct *tsk);
extern void exit_keys(struct task_struct *tsk);
extern void exit_thread_group_keys(struct signal_struct *tg);
extern int suid_keys(struct task_struct *tsk);
extern int exec_keys(struct task_struct *tsk);
extern void key_fsuid_changed(struct task_struct *tsk);
extern void key_fsgid_changed(struct task_struct *tsk);
extern void key_init(void);

#define __install_session_keyring(tsk, keyring)			\
({								\
	struct key *old_session = tsk->signal->session_keyring;	\
	tsk->signal->session_keyring = keyring;			\
	old_session;						\
})

#else 

#define key_validate(k)			0
#define key_serial(k)			0
#define key_get(k) 			({ NULL; })
#define key_put(k)			do { } while(0)
#define key_ref_put(k)			do { } while(0)
#define make_key_ref(k)			({ NULL; })
#define key_ref_to_ptr(k)		({ NULL; })
#define is_key_possessed(k)		0
#define alloc_uid_keyring(u,c)		0
#define switch_uid_keyring(u)		do { } while(0)
#define __install_session_keyring(t, k)	({ NULL; })
#define copy_keys(f,t)			0
#define copy_thread_group_keys(t)	0
#define exit_keys(t)			do { } while(0)
#define exit_thread_group_keys(tg)	do { } while(0)
#define suid_keys(t)			do { } while(0)
#define exec_keys(t)			do { } while(0)
#define key_fsuid_changed(t)		do { } while(0)
#define key_fsgid_changed(t)		do { } while(0)
#define key_init()			do { } while(0)

extern struct key root_user_keyring;
extern struct key root_session_keyring;

#endif 
#endif 
#endif 
