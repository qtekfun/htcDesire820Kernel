

#ifndef _NF_CONNTRACK_TUPLE_H
#define _NF_CONNTRACK_TUPLE_H

#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/nf_conntrack_tuple_common.h>


#define NF_CT_TUPLE_L3SIZE	ARRAY_SIZE(((union nf_inet_addr *)NULL)->all)

union nf_conntrack_man_proto
{
	
	__be16 all;

	struct {
		__be16 port;
	} tcp;
	struct {
		__be16 port;
	} udp;
	struct {
		__be16 id;
	} icmp;
	struct {
		__be16 port;
	} dccp;
	struct {
		__be16 port;
	} sctp;
	struct {
		__be16 key;	
	} gre;
};

struct nf_conntrack_man
{
	union nf_inet_addr u3;
	union nf_conntrack_man_proto u;
	
	u_int16_t l3num;
};

struct nf_conntrack_tuple
{
	struct nf_conntrack_man src;

	
	struct {
		union nf_inet_addr u3;
		union {
			
			__be16 all;

			struct {
				__be16 port;
			} tcp;
			struct {
				__be16 port;
			} udp;
			struct {
				u_int8_t type, code;
			} icmp;
			struct {
				__be16 port;
			} dccp;
			struct {
				__be16 port;
			} sctp;
			struct {
				__be16 key;
			} gre;
		} u;

		
		u_int8_t protonum;

		
		u_int8_t dir;
	} dst;
};

struct nf_conntrack_tuple_mask
{
	struct {
		union nf_inet_addr u3;
		union nf_conntrack_man_proto u;
	} src;
};

#endif 
