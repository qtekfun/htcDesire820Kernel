
#ifndef _ARPTABLES_H
#define _ARPTABLES_H

#ifdef __KERNEL__
#include <linux/if.h>
#include <linux/types.h>
#include <linux/in.h>
#include <linux/if_arp.h>
#include <linux/skbuff.h>
#endif
#include <linux/compiler.h>
#include <linux/netfilter_arp.h>

#include <linux/netfilter/x_tables.h>

#define ARPT_FUNCTION_MAXNAMELEN XT_FUNCTION_MAXNAMELEN
#define ARPT_TABLE_MAXNAMELEN XT_TABLE_MAXNAMELEN
#define arpt_target xt_target
#define arpt_table xt_table

#define ARPT_DEV_ADDR_LEN_MAX 16

struct arpt_devaddr_info {
	char addr[ARPT_DEV_ADDR_LEN_MAX];
	char mask[ARPT_DEV_ADDR_LEN_MAX];
};

struct arpt_arp {
	
	struct in_addr src, tgt;
	
	struct in_addr smsk, tmsk;

	
	u_int8_t arhln, arhln_mask;
	struct arpt_devaddr_info src_devaddr;
	struct arpt_devaddr_info tgt_devaddr;

	
	u_int16_t arpop, arpop_mask;

	
	u_int16_t arhrd, arhrd_mask;
	u_int16_t arpro, arpro_mask;


	char iniface[IFNAMSIZ], outiface[IFNAMSIZ];
	unsigned char iniface_mask[IFNAMSIZ], outiface_mask[IFNAMSIZ];

	
	u_int8_t flags;
	
	u_int16_t invflags;
};

#define arpt_entry_target xt_entry_target
#define arpt_standard_target xt_standard_target

#define ARPT_F_MASK		0x00	

#define ARPT_INV_VIA_IN		0x0001	
#define ARPT_INV_VIA_OUT	0x0002	
#define ARPT_INV_SRCIP		0x0004	
#define ARPT_INV_TGTIP		0x0008	
#define ARPT_INV_SRCDEVADDR	0x0010	
#define ARPT_INV_TGTDEVADDR	0x0020	
#define ARPT_INV_ARPOP		0x0040	
#define ARPT_INV_ARPHRD		0x0080	
#define ARPT_INV_ARPPRO		0x0100	
#define ARPT_INV_ARPHLN		0x0200	
#define ARPT_INV_MASK		0x03FF	

struct arpt_entry
{
	struct arpt_arp arp;

	
	u_int16_t target_offset;
	
	u_int16_t next_offset;

	
	unsigned int comefrom;

	
	struct xt_counters counters;

	
	unsigned char elems[0];
};

#define ARPT_CTL_OFFSET		32
#define ARPT_BASE_CTL		(XT_BASE_CTL+ARPT_CTL_OFFSET)

#define ARPT_SO_SET_REPLACE		(XT_SO_SET_REPLACE+ARPT_CTL_OFFSET)
#define ARPT_SO_SET_ADD_COUNTERS	(XT_SO_SET_ADD_COUNTERS+ARPT_CTL_OFFSET)
#define ARPT_SO_SET_MAX			(XT_SO_SET_MAX+ARPT_CTL_OFFSET)

#define ARPT_SO_GET_INFO		(XT_SO_GET_INFO+ARPT_CTL_OFFSET)
#define ARPT_SO_GET_ENTRIES		(XT_SO_GET_ENTRIES+ARPT_CTL_OFFSET)
#define ARPT_SO_GET_REVISION_TARGET	(XT_SO_GET_REVISION_TARGET+ARPT_CTL_OFFSET)
#define ARPT_SO_GET_MAX			(XT_SO_GET_REVISION_TARGET+ARPT_CTL_OFFSET)

#define ARPT_CONTINUE XT_CONTINUE

#define ARPT_RETURN XT_RETURN

struct arpt_getinfo
{
	
	char name[ARPT_TABLE_MAXNAMELEN];

	
	
	unsigned int valid_hooks;

	
	unsigned int hook_entry[NF_ARP_NUMHOOKS];

	
	unsigned int underflow[NF_ARP_NUMHOOKS];

	
	unsigned int num_entries;

	
	unsigned int size;
};

struct arpt_replace
{
	
	char name[ARPT_TABLE_MAXNAMELEN];

	unsigned int valid_hooks;

	
	unsigned int num_entries;

	
	unsigned int size;

	
	unsigned int hook_entry[NF_ARP_NUMHOOKS];

	
	unsigned int underflow[NF_ARP_NUMHOOKS];

	
	
	unsigned int num_counters;
	
	struct xt_counters __user *counters;

	
	struct arpt_entry entries[0];
};

#define arpt_counters_info xt_counters_info

struct arpt_get_entries
{
	
	char name[ARPT_TABLE_MAXNAMELEN];

	
	unsigned int size;

	
	struct arpt_entry entrytable[0];
};

#define ARPT_STANDARD_TARGET XT_STANDARD_TARGET
#define ARPT_ERROR_TARGET XT_ERROR_TARGET

static __inline__ struct arpt_entry_target *arpt_get_target(struct arpt_entry *e)
{
	return (void *)e + e->target_offset;
}

#define ARPT_ENTRY_ITERATE(entries, size, fn, args...)		\
({								\
	unsigned int __i;					\
	int __ret = 0;						\
	struct arpt_entry *__entry;				\
								\
	for (__i = 0; __i < (size); __i += __entry->next_offset) { \
		__entry = (void *)(entries) + __i;		\
								\
		__ret = fn(__entry , ## args);			\
		if (__ret != 0)					\
			break;					\
	}							\
	__ret;							\
})

#ifdef __KERNEL__

#define arpt_register_target(tgt) 	\
({	(tgt)->family = NF_ARP;		\
 	xt_register_target(tgt); })
#define arpt_unregister_target(tgt) xt_unregister_target(tgt)

extern int arpt_register_table(struct arpt_table *table,
			       const struct arpt_replace *repl);
extern void arpt_unregister_table(struct arpt_table *table);
extern unsigned int arpt_do_table(struct sk_buff **pskb,
				  unsigned int hook,
				  const struct net_device *in,
				  const struct net_device *out,
				  struct arpt_table *table,
				  void *userdata);

#define ARPT_ALIGN(s) (((s) + (__alignof__(struct arpt_entry)-1)) & ~(__alignof__(struct arpt_entry)-1))
#endif 
#endif 
