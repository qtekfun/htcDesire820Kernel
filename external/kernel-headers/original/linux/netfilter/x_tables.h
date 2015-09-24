#ifndef _X_TABLES_H
#define _X_TABLES_H

#define XT_FUNCTION_MAXNAMELEN 30
#define XT_TABLE_MAXNAMELEN 32

struct xt_entry_match
{
	union {
		struct {
			u_int16_t match_size;

			
			char name[XT_FUNCTION_MAXNAMELEN-1];

			u_int8_t revision;
		} user;
		struct {
			u_int16_t match_size;

			
			struct xt_match *match;
		} kernel;

		
		u_int16_t match_size;
	} u;

	unsigned char data[0];
};

struct xt_entry_target
{
	union {
		struct {
			u_int16_t target_size;

			
			char name[XT_FUNCTION_MAXNAMELEN-1];

			u_int8_t revision;
		} user;
		struct {
			u_int16_t target_size;

			
			struct xt_target *target;
		} kernel;

		
		u_int16_t target_size;
	} u;

	unsigned char data[0];
};

struct xt_standard_target
{
	struct xt_entry_target target;
	int verdict;
};

struct xt_get_revision
{
	char name[XT_FUNCTION_MAXNAMELEN-1];

	u_int8_t revision;
};

#define XT_CONTINUE 0xFFFFFFFF

#define XT_RETURN (-NF_REPEAT - 1)

struct _xt_align
{
	u_int8_t u8;
	u_int16_t u16;
	u_int32_t u32;
	u_int64_t u64;
};

#define XT_ALIGN(s) (((s) + (__alignof__(struct _xt_align)-1)) 	\
			& ~(__alignof__(struct _xt_align)-1))

#define XT_STANDARD_TARGET ""
#define XT_ERROR_TARGET "ERROR"

#define XT_BASE_CTL		64	

#define XT_SO_SET_REPLACE	(XT_BASE_CTL)
#define XT_SO_SET_ADD_COUNTERS	(XT_BASE_CTL + 1)
#define XT_SO_SET_MAX		XT_SO_SET_ADD_COUNTERS

#define XT_SO_GET_INFO			(XT_BASE_CTL)
#define XT_SO_GET_ENTRIES		(XT_BASE_CTL + 1)
#define XT_SO_GET_REVISION_MATCH	(XT_BASE_CTL + 2)
#define XT_SO_GET_REVISION_TARGET	(XT_BASE_CTL + 3)
#define XT_SO_GET_MAX			XT_SO_GET_REVISION_TARGET

#define SET_COUNTER(c,b,p) do { (c).bcnt = (b); (c).pcnt = (p); } while(0)
#define ADD_COUNTER(c,b,p) do { (c).bcnt += (b); (c).pcnt += (p); } while(0)

struct xt_counters
{
	u_int64_t pcnt, bcnt;			
};

struct xt_counters_info
{
	
	char name[XT_TABLE_MAXNAMELEN];

	unsigned int num_counters;

	
	struct xt_counters counters[0];
};

#define XT_INV_PROTO		0x40	

#ifdef __KERNEL__

#include <linux/netdevice.h>

#define ASSERT_READ_LOCK(x)
#define ASSERT_WRITE_LOCK(x)
#include <linux/netfilter_ipv4/listhelp.h>

#ifdef CONFIG_COMPAT
#define COMPAT_TO_USER		1
#define COMPAT_FROM_USER	-1
#define COMPAT_CALC_SIZE	0
#endif

struct xt_match
{
	struct list_head list;

	const char name[XT_FUNCTION_MAXNAMELEN-1];

	int (*match)(const struct sk_buff *skb,
		     const struct net_device *in,
		     const struct net_device *out,
		     const struct xt_match *match,
		     const void *matchinfo,
		     int offset,
		     unsigned int protoff,
		     int *hotdrop);

	
	
	int (*checkentry)(const char *tablename,
			  const void *ip,
			  const struct xt_match *match,
			  void *matchinfo,
			  unsigned int matchinfosize,
			  unsigned int hook_mask);

	
	void (*destroy)(const struct xt_match *match, void *matchinfo,
			unsigned int matchinfosize);

	
	int (*compat)(void *match, void **dstptr, int *size, int convert);

	
	struct module *me;

	char *table;
	unsigned int matchsize;
	unsigned int hooks;
	unsigned short proto;

	unsigned short family;
	u_int8_t revision;
};

struct xt_target
{
	struct list_head list;

	const char name[XT_FUNCTION_MAXNAMELEN-1];

	unsigned int (*target)(struct sk_buff **pskb,
			       const struct net_device *in,
			       const struct net_device *out,
			       unsigned int hooknum,
			       const struct xt_target *target,
			       const void *targinfo,
			       void *userdata);

	
	int (*checkentry)(const char *tablename,
			  const void *entry,
			  const struct xt_target *target,
			  void *targinfo,
			  unsigned int targinfosize,
			  unsigned int hook_mask);

	
	void (*destroy)(const struct xt_target *target, void *targinfo,
			unsigned int targinfosize);

	
	int (*compat)(void *target, void **dstptr, int *size, int convert);

	
	struct module *me;

	char *table;
	unsigned int targetsize;
	unsigned int hooks;
	unsigned short proto;

	unsigned short family;
	u_int8_t revision;
};

struct xt_table
{
	struct list_head list;

	
	char name[XT_TABLE_MAXNAMELEN];

	
	unsigned int valid_hooks;

	
	rwlock_t lock;

	
	
	void *private;

	
	struct module *me;

	int af;		
};

#include <linux/netfilter_ipv4.h>

struct xt_table_info
{
	
	unsigned int size;
	
	unsigned int number;
	
	unsigned int initial_entries;

	
	unsigned int hook_entry[NF_IP_NUMHOOKS];
	unsigned int underflow[NF_IP_NUMHOOKS];

	
	char *entries[NR_CPUS];
};

extern int xt_register_target(struct xt_target *target);
extern void xt_unregister_target(struct xt_target *target);
extern int xt_register_match(struct xt_match *target);
extern void xt_unregister_match(struct xt_match *target);

extern int xt_check_match(const struct xt_match *match, unsigned short family,
			  unsigned int size, const char *table, unsigned int hook,
			  unsigned short proto, int inv_proto);
extern int xt_check_target(const struct xt_target *target, unsigned short family,
			   unsigned int size, const char *table, unsigned int hook,
			   unsigned short proto, int inv_proto);

extern int xt_register_table(struct xt_table *table,
			     struct xt_table_info *bootstrap,
			     struct xt_table_info *newinfo);
extern void *xt_unregister_table(struct xt_table *table);

extern struct xt_table_info *xt_replace_table(struct xt_table *table,
					      unsigned int num_counters,
					      struct xt_table_info *newinfo,
					      int *error);

extern struct xt_match *xt_find_match(int af, const char *name, u8 revision);
extern struct xt_target *xt_find_target(int af, const char *name, u8 revision);
extern struct xt_target *xt_request_find_target(int af, const char *name, 
						u8 revision);
extern int xt_find_revision(int af, const char *name, u8 revision, int target,
			    int *err);

extern struct xt_table *xt_find_table_lock(int af, const char *name);
extern void xt_table_unlock(struct xt_table *t);

extern int xt_proto_init(int af);
extern void xt_proto_fini(int af);

extern struct xt_table_info *xt_alloc_table_info(unsigned int size);
extern void xt_free_table_info(struct xt_table_info *info);

#ifdef CONFIG_COMPAT
#include <net/compat.h>

struct compat_xt_entry_match
{
	union {
		struct {
			u_int16_t match_size;
			char name[XT_FUNCTION_MAXNAMELEN - 1];
			u_int8_t revision;
		} user;
		struct {
			u_int16_t match_size;
			compat_uptr_t match;
		} kernel;
		u_int16_t match_size;
	} u;
	unsigned char data[0];
};

struct compat_xt_entry_target
{
	union {
		struct {
			u_int16_t target_size;
			char name[XT_FUNCTION_MAXNAMELEN - 1];
			u_int8_t revision;
		} user;
		struct {
			u_int16_t target_size;
			compat_uptr_t target;
		} kernel;
		u_int16_t target_size;
	} u;
	unsigned char data[0];
};


struct compat_xt_counters
{
#if defined(CONFIG_X86_64) || defined(CONFIG_IA64)
	u_int32_t cnt[4];
#else
	u_int64_t cnt[2];
#endif
};

struct compat_xt_counters_info
{
	char name[XT_TABLE_MAXNAMELEN];
	compat_uint_t num_counters;
	struct compat_xt_counters counters[0];
};

#define COMPAT_XT_ALIGN(s) (((s) + (__alignof__(struct compat_xt_counters)-1)) \
		& ~(__alignof__(struct compat_xt_counters)-1))

extern void xt_compat_lock(int af);
extern void xt_compat_unlock(int af);
extern int xt_compat_match(void *match, void **dstptr, int *size, int convert);
extern int xt_compat_target(void *target, void **dstptr, int *size,
		int convert);

#endif 
#endif 

#endif 
