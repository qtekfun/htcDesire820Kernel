#ifndef _XT_RATE_H
#define _XT_RATE_H

#define XT_LIMIT_SCALE 10000

struct xt_rateinfo {
	u_int32_t avg;    
	u_int32_t burst;  

	
	unsigned long prev;
	u_int32_t credit;
	u_int32_t credit_cap, cost;

	
	struct xt_rateinfo *master;
};
#endif 
