#ifndef _NF_NAT_H
#define _NF_NAT_H
#include <linux/netfilter_ipv4.h>
#include <net/netfilter/nf_conntrack_tuple.h>

#define NF_NAT_MAPPING_TYPE_MAX_NAMELEN 16

enum nf_nat_manip_type
{
	IP_NAT_MANIP_SRC,
	IP_NAT_MANIP_DST
};

#define HOOK2MANIP(hooknum) ((hooknum) != NF_INET_POST_ROUTING && \
			     (hooknum) != NF_INET_LOCAL_IN)

#define IP_NAT_RANGE_MAP_IPS 1
#define IP_NAT_RANGE_PROTO_SPECIFIED 2
#define IP_NAT_RANGE_PROTO_RANDOM 4
#define IP_NAT_RANGE_PERSISTENT 8

struct nf_nat_seq {
	
	u_int32_t correction_pos;

	
	int16_t offset_before, offset_after;
};

struct nf_nat_range
{
	
	unsigned int flags;

	
	__be32 min_ip, max_ip;

	
	union nf_conntrack_man_proto min, max;
};

struct nf_nat_multi_range_compat
{
	unsigned int rangesize; 

	
	struct nf_nat_range range[1];
};

#define nf_nat_multi_range nf_nat_multi_range_compat
#endif
