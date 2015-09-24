#ifndef _XT_MULTIPORT_H
#define _XT_MULTIPORT_H

enum xt_multiport_flags
{
	XT_MULTIPORT_SOURCE,
	XT_MULTIPORT_DESTINATION,
	XT_MULTIPORT_EITHER
};

#define XT_MULTI_PORTS	15

struct xt_multiport
{
	u_int8_t flags;				
	u_int8_t count;				
	u_int16_t ports[XT_MULTI_PORTS];	
};

struct xt_multiport_v1
{
	u_int8_t flags;				
	u_int8_t count;				
	u_int16_t ports[XT_MULTI_PORTS];	
	u_int8_t pflags[XT_MULTI_PORTS];	
	u_int8_t invert;			
};

#endif 
