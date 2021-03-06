#ifndef _XT_CONNLIMIT_H
#define _XT_CONNLIMIT_H

struct xt_connlimit_data;

enum {
	XT_CONNLIMIT_INVERT = 1 << 0,
	XT_CONNLIMIT_DADDR  = 1 << 1,
};

struct xt_connlimit_info {
	union {
		union nf_inet_addr mask;
		union {
			__be32 v4_mask;
			__be32 v6_mask[4];
		};
	};
	unsigned int limit;
	union {
		
		unsigned int inverse;

		
		__u32 flags;
	};

	
	struct xt_connlimit_data *data __attribute__((aligned(8)));
};

#endif 
