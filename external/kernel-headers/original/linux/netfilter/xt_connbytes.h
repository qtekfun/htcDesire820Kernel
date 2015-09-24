#ifndef _XT_CONNBYTES_H
#define _XT_CONNBYTES_H

enum xt_connbytes_what {
	XT_CONNBYTES_PKTS,
	XT_CONNBYTES_BYTES,
	XT_CONNBYTES_AVGPKT,
};

enum xt_connbytes_direction {
	XT_CONNBYTES_DIR_ORIGINAL,
	XT_CONNBYTES_DIR_REPLY,
	XT_CONNBYTES_DIR_BOTH,
};

struct xt_connbytes_info
{
	struct {
		aligned_u64 from;	
		aligned_u64 to;		
	} count;
	u_int8_t what;		
	u_int8_t direction;	
};
#endif
