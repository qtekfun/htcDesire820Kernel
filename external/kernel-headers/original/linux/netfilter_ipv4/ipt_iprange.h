#ifndef _IPT_IPRANGE_H
#define _IPT_IPRANGE_H

#define IPRANGE_SRC		0x01	
#define IPRANGE_DST		0x02	
#define IPRANGE_SRC_INV		0x10	
#define IPRANGE_DST_INV		0x20	

struct ipt_iprange {
	
	u_int32_t min_ip, max_ip;
};

struct ipt_iprange_info
{
	struct ipt_iprange src;
	struct ipt_iprange dst;

	
	u_int8_t flags;
};

#endif 
