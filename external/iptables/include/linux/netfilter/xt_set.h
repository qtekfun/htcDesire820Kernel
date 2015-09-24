#ifndef _XT_SET_H
#define _XT_SET_H

#define IPSET_PROTOCOL		5

#define IPSET_MAXNAMELEN	32

typedef uint16_t ip_set_id_t;

#define IPSET_INVALID_ID	65535

enum ip_set_dim {
	IPSET_DIM_ZERO = 0,
	IPSET_DIM_ONE,
	IPSET_DIM_TWO,
	IPSET_DIM_THREE,
	IPSET_DIM_MAX = 6,
};

enum ip_set_kopt {
	IPSET_INV_MATCH = (1 << IPSET_DIM_ZERO),
	IPSET_DIM_ONE_SRC = (1 << IPSET_DIM_ONE),
	IPSET_DIM_TWO_SRC = (1 << IPSET_DIM_TWO),
	IPSET_DIM_THREE_SRC = (1 << IPSET_DIM_THREE),
};


#define SO_IP_SET 		83

union ip_set_name_index {
	char name[IPSET_MAXNAMELEN];
	ip_set_id_t index;
};

#define IP_SET_OP_GET_BYNAME	0x00000006	
struct ip_set_req_get_set {
	unsigned op;
	unsigned version;
	union ip_set_name_index set;
};

#define IP_SET_OP_GET_BYINDEX	0x00000007	

#define IP_SET_OP_VERSION	0x00000100	
struct ip_set_req_version {
	unsigned op;
	unsigned version;
};


#define IPSET_SRC		0x01	
#define IPSET_DST		0x02	
#define IPSET_MATCH_INV		0x04	

struct xt_set_info_v0 {
	ip_set_id_t index;
	union {
		u_int32_t flags[IPSET_DIM_MAX + 1];
		struct {
			u_int32_t __flags[IPSET_DIM_MAX];
			u_int8_t dim;
			u_int8_t flags;
		} compat;
	} u;
};

struct xt_set_info_match_v0 {
	struct xt_set_info_v0 match_set;
};

struct xt_set_info_target_v0 {
	struct xt_set_info_v0 add_set;
	struct xt_set_info_v0 del_set;
};


struct xt_set_info {
	ip_set_id_t index;
	u_int8_t dim;
	u_int8_t flags;
};

struct xt_set_info_match_v1 {
	struct xt_set_info match_set;
};

struct xt_set_info_target_v1 {
	struct xt_set_info add_set;
	struct xt_set_info del_set;
};


enum ipset_cmd_flags {
	IPSET_FLAG_BIT_EXIST	= 0,
	IPSET_FLAG_EXIST	= (1 << IPSET_FLAG_BIT_EXIST),
};

struct xt_set_info_target_v2 {
	struct xt_set_info add_set;
	struct xt_set_info del_set;
	u_int32_t flags;
	u_int32_t timeout;
};

#endif 
