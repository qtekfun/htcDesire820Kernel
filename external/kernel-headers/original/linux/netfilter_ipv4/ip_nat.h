#ifndef _IP_NAT_H
#define _IP_NAT_H
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv4/ip_conntrack_tuple.h>

#define IP_NAT_MAPPING_TYPE_MAX_NAMELEN 16

enum ip_nat_manip_type
{
	IP_NAT_MANIP_SRC,
	IP_NAT_MANIP_DST
};

#define HOOK2MANIP(hooknum) ((hooknum) != NF_IP_POST_ROUTING && (hooknum) != NF_IP_LOCAL_IN)

#define IP_NAT_RANGE_MAP_IPS 1
#define IP_NAT_RANGE_PROTO_SPECIFIED 2

struct ip_nat_seq {
	u_int32_t correction_pos;
	
	int16_t offset_before, offset_after;
};

struct ip_nat_range
{
	
	unsigned int flags;

	
	u_int32_t min_ip, max_ip;

	
	union ip_conntrack_manip_proto min, max;
};

struct ip_nat_multi_range_compat
{
	unsigned int rangesize; 

	
	struct ip_nat_range range[1];
};

#ifdef __KERNEL__
#include <linux/list.h>

extern rwlock_t ip_nat_lock;

struct ip_nat_info
{
	struct list_head bysource;
	struct ip_nat_seq seq[IP_CT_DIR_MAX];
};

struct ip_conntrack;

extern unsigned int ip_nat_setup_info(struct ip_conntrack *conntrack,
				      const struct ip_nat_range *range,
				      unsigned int hooknum);

extern int ip_nat_used_tuple(const struct ip_conntrack_tuple *tuple,
			     const struct ip_conntrack *ignored_conntrack);

extern u_int16_t ip_nat_cheat_check(u_int32_t oldvalinv,
				    u_int32_t newval,
				    u_int16_t oldcheck);
#else  
#define ip_nat_multi_range ip_nat_multi_range_compat
#endif 
#endif
