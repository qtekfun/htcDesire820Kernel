#ifndef _XT_MARK_H_target
#define _XT_MARK_H_target

struct xt_mark_target_info {
	unsigned long mark;
};

enum {
	XT_MARK_SET=0,
	XT_MARK_AND,
	XT_MARK_OR,
};

struct xt_mark_target_info_v1 {
	unsigned long mark;
	u_int8_t mode;
};

#endif 
