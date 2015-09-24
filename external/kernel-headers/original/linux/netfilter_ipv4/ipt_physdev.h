#ifndef _IPT_PHYSDEV_H
#define _IPT_PHYSDEV_H


#include <linux/netfilter/xt_physdev.h>

#define IPT_PHYSDEV_OP_IN		XT_PHYSDEV_OP_IN
#define IPT_PHYSDEV_OP_OUT		XT_PHYSDEV_OP_OUT
#define IPT_PHYSDEV_OP_BRIDGED		XT_PHYSDEV_OP_BRIDGED
#define IPT_PHYSDEV_OP_ISIN		XT_PHYSDEV_OP_ISIN
#define IPT_PHYSDEV_OP_ISOUT		XT_PHYSDEV_OP_ISOUT
#define IPT_PHYSDEV_OP_MASK		XT_PHYSDEV_OP_MASK

#define ipt_physdev_info xt_physdev_info

#endif 
