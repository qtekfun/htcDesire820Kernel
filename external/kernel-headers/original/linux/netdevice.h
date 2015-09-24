/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Definitions for the Interfaces handler.
 *
 * Version:	@(#)dev.h	1.0.10	08/12/93
 *
 * Authors:	Ross Biro
 *		Fred N. van Kempen, <waltje@uWalt.NL.Mugnet.ORG>
 *		Corey Minyard <wf-rch!minyard@relay.EU.net>
 *		Donald J. Becker, <becker@cesdis.gsfc.nasa.gov>
 *		Alan Cox, <Alan.Cox@linux.org>
 *		Bjorn Ekwall. <bj0rn@blox.se>
 *              Pekka Riikonen <priikone@poseidon.pspt.fi>
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 *		Moved to /usr/include/linux for NET3
 */
#ifndef _LINUX_NETDEVICE_H
#define _LINUX_NETDEVICE_H

#include <linux/if.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>

#ifdef __KERNEL__
#include <asm/atomic.h>
#include <asm/cache.h>
#include <asm/byteorder.h>

#include <linux/device.h>
#include <linux/percpu.h>
#include <linux/dmaengine.h>

struct divert_blk;
struct vlan_group;
struct ethtool_ops;
struct netpoll_info;
					
#define SET_ETHTOOL_OPS(netdev,ops) \
	( (netdev)->ethtool_ops = (ops) )

#define HAVE_ALLOC_NETDEV		
#define HAVE_FREE_NETDEV		
#define HAVE_NETDEV_PRIV		

#define NET_XMIT_SUCCESS	0
#define NET_XMIT_DROP		1	
#define NET_XMIT_CN		2	
#define NET_XMIT_POLICED	3	
#define NET_XMIT_BYPASS		4	

#define NET_RX_SUCCESS		0   
#define NET_RX_DROP		1  
#define NET_RX_CN_LOW		2   
#define NET_RX_CN_MOD		3   
#define NET_RX_CN_HIGH		4   
#define NET_RX_BAD		5  

#define net_xmit_errno(e)	((e) != NET_XMIT_CN ? -ENOBUFS : 0)

#endif

#define MAX_ADDR_LEN	32		

#define NETDEV_TX_OK 0		
#define NETDEV_TX_BUSY 1	
#define NETDEV_TX_LOCKED -1	

 
#if !defined(CONFIG_AX25) && !defined(CONFIG_AX25_MODULE) && !defined(CONFIG_TR)
#define LL_MAX_HEADER	32
#else
#if defined(CONFIG_AX25) || defined(CONFIG_AX25_MODULE)
#define LL_MAX_HEADER	96
#else
#define LL_MAX_HEADER	48
#endif
#endif

#if !defined(CONFIG_NET_IPIP) && \
    !defined(CONFIG_IPV6) && !defined(CONFIG_IPV6_MODULE)
#define MAX_HEADER LL_MAX_HEADER
#else
#define MAX_HEADER (LL_MAX_HEADER + 48)
#endif

 
struct net_device_stats
{
	unsigned long	rx_packets;		
	unsigned long	tx_packets;		
	unsigned long	rx_bytes;		
	unsigned long	tx_bytes;		
	unsigned long	rx_errors;		
	unsigned long	tx_errors;		
	unsigned long	rx_dropped;		
	unsigned long	tx_dropped;		
	unsigned long	multicast;		
	unsigned long	collisions;

	
	unsigned long	rx_length_errors;
	unsigned long	rx_over_errors;		
	unsigned long	rx_crc_errors;		
	unsigned long	rx_frame_errors;	
	unsigned long	rx_fifo_errors;		
	unsigned long	rx_missed_errors;	

	
	unsigned long	tx_aborted_errors;
	unsigned long	tx_carrier_errors;
	unsigned long	tx_fifo_errors;
	unsigned long	tx_heartbeat_errors;
	unsigned long	tx_window_errors;
	
	
	unsigned long	rx_compressed;
	unsigned long	tx_compressed;
};


enum {
        IF_PORT_UNKNOWN = 0,
        IF_PORT_10BASE2,
        IF_PORT_10BASET,
        IF_PORT_AUI,
        IF_PORT_100BASET,
        IF_PORT_100BASETX,
        IF_PORT_100BASEFX
};

#ifdef __KERNEL__

#include <linux/cache.h>
#include <linux/skbuff.h>

struct neighbour;
struct neigh_parms;
struct sk_buff;

struct netif_rx_stats
{
	unsigned total;
	unsigned dropped;
	unsigned time_squeeze;
	unsigned cpu_collision;
};

DECLARE_PER_CPU(struct netif_rx_stats, netdev_rx_stat);


 
struct dev_mc_list
{	
	struct dev_mc_list	*next;
	__u8			dmi_addr[MAX_ADDR_LEN];
	unsigned char		dmi_addrlen;
	int			dmi_users;
	int			dmi_gusers;
};

struct hh_cache
{
	struct hh_cache *hh_next;	
	atomic_t	hh_refcnt;	
	unsigned short  hh_type;	
	int		hh_len;		
	int		(*hh_output)(struct sk_buff *skb);
	rwlock_t	hh_lock;

	
#define HH_DATA_MOD	16
#define HH_DATA_OFF(__len) \
	(HH_DATA_MOD - (((__len - 1) & (HH_DATA_MOD - 1)) + 1))
#define HH_DATA_ALIGN(__len) \
	(((__len)+(HH_DATA_MOD-1))&~(HH_DATA_MOD - 1))
	unsigned long	hh_data[HH_DATA_ALIGN(LL_MAX_HEADER) / sizeof(long)];
};

#define LL_RESERVED_SPACE(dev) \
	(((dev)->hard_header_len&~(HH_DATA_MOD - 1)) + HH_DATA_MOD)
#define LL_RESERVED_SPACE_EXTRA(dev,extra) \
	((((dev)->hard_header_len+extra)&~(HH_DATA_MOD - 1)) + HH_DATA_MOD)


enum netdev_state_t
{
	__LINK_STATE_XOFF=0,
	__LINK_STATE_START,
	__LINK_STATE_PRESENT,
	__LINK_STATE_SCHED,
	__LINK_STATE_NOCARRIER,
	__LINK_STATE_RX_SCHED,
	__LINK_STATE_LINKWATCH_PENDING,
	__LINK_STATE_DORMANT,
	__LINK_STATE_QDISC_RUNNING,
};


struct netdev_boot_setup {
	char name[IFNAMSIZ];
	struct ifmap map;
};
#define NETDEV_BOOT_SETUP_MAX 8

extern int __init netdev_boot_setup(char *str);


struct net_device
{

	char			name[IFNAMSIZ];
	
	struct hlist_node	name_hlist;

	unsigned long		mem_end;	
	unsigned long		mem_start;	
	unsigned long		base_addr;	
	unsigned int		irq;		


	unsigned char		if_port;	
	unsigned char		dma;		

	unsigned long		state;

	struct net_device	*next;
	
	
	int			(*init)(struct net_device *dev);

	

	
	unsigned long		features;
#define NETIF_F_SG		1	
#define NETIF_F_IP_CSUM		2	
#define NETIF_F_NO_CSUM		4	
#define NETIF_F_HW_CSUM		8	
#define NETIF_F_HIGHDMA		32	
#define NETIF_F_FRAGLIST	64	
#define NETIF_F_HW_VLAN_TX	128	
#define NETIF_F_HW_VLAN_RX	256	
#define NETIF_F_HW_VLAN_FILTER	512	
#define NETIF_F_VLAN_CHALLENGED	1024	
#define NETIF_F_GSO		2048	
#define NETIF_F_LLTX		4096	

	
#define NETIF_F_GSO_SHIFT	16
#define NETIF_F_GSO_MASK	0xffff0000
#define NETIF_F_TSO		(SKB_GSO_TCPV4 << NETIF_F_GSO_SHIFT)
#define NETIF_F_UFO		(SKB_GSO_UDP << NETIF_F_GSO_SHIFT)
#define NETIF_F_GSO_ROBUST	(SKB_GSO_DODGY << NETIF_F_GSO_SHIFT)
#define NETIF_F_TSO_ECN		(SKB_GSO_TCP_ECN << NETIF_F_GSO_SHIFT)
#define NETIF_F_TSO6		(SKB_GSO_TCPV6 << NETIF_F_GSO_SHIFT)

	
#define NETIF_F_GSO_SOFTWARE	(NETIF_F_TSO | NETIF_F_TSO_ECN | NETIF_F_TSO6)

#define NETIF_F_GEN_CSUM	(NETIF_F_NO_CSUM | NETIF_F_HW_CSUM)
#define NETIF_F_ALL_CSUM	(NETIF_F_IP_CSUM | NETIF_F_GEN_CSUM)

	struct net_device	*next_sched;

	
	int			ifindex;
	int			iflink;


	struct net_device_stats* (*get_stats)(struct net_device *dev);
	struct iw_statistics*	(*get_wireless_stats)(struct net_device *dev);

	const struct iw_handler_def *	wireless_handlers;
	
	struct iw_public_data *	wireless_data;

	struct ethtool_ops *ethtool_ops;



	unsigned int		flags;	
	unsigned short		gflags;
        unsigned short          priv_flags; 
	unsigned short		padded;	

	unsigned char		operstate; 
	unsigned char		link_mode; 

	unsigned		mtu;	
	unsigned short		type;	
	unsigned short		hard_header_len;	

	struct net_device	*master; 

	
	unsigned char		perm_addr[MAX_ADDR_LEN]; 
	unsigned char		addr_len;	
	unsigned short          dev_id;		

	struct dev_mc_list	*mc_list;	
	int			mc_count;	
	int			promiscuity;
	int			allmulti;


	
	
	void 			*atalk_ptr;	
	void			*ip_ptr;	  
	void                    *dn_ptr;        
	void                    *ip6_ptr;       
	void			*ec_ptr;	
	void			*ax25_ptr;	

	struct list_head	poll_list ____cacheline_aligned_in_smp;
					

	int			(*poll) (struct net_device *dev, int *quota);
	int			quota;
	int			weight;
	unsigned long		last_rx;	
	
	unsigned char		dev_addr[MAX_ADDR_LEN];	

	unsigned char		broadcast[MAX_ADDR_LEN];	

	
	spinlock_t		queue_lock ____cacheline_aligned_in_smp;
	struct Qdisc		*qdisc;
	struct Qdisc		*qdisc_sleeping;
	struct list_head	qdisc_list;
	unsigned long		tx_queue_len;	

	
	struct sk_buff		*gso_skb;

	
	spinlock_t		ingress_lock;
	struct Qdisc		*qdisc_ingress;

	
	spinlock_t		_xmit_lock ____cacheline_aligned_in_smp;
	int			xmit_lock_owner;
	void			*priv;	
	int			(*hard_start_xmit) (struct sk_buff *skb,
						    struct net_device *dev);
	
	unsigned long		trans_start;	

	int			watchdog_timeo; 
	struct timer_list	watchdog_timer;

	
	atomic_t		refcnt ____cacheline_aligned_in_smp;

	
	struct list_head	todo_list;
	
	struct hlist_node	index_hlist;

	
	enum { NETREG_UNINITIALIZED=0,
	       NETREG_REGISTERED,	
	       NETREG_UNREGISTERING,	
	       NETREG_UNREGISTERED,	
	       NETREG_RELEASED,		
	} reg_state;

	
	void			(*uninit)(struct net_device *dev);
	
	void			(*destructor)(struct net_device *dev);

	
	int			(*open)(struct net_device *dev);
	int			(*stop)(struct net_device *dev);
#define HAVE_NETDEV_POLL
	int			(*hard_header) (struct sk_buff *skb,
						struct net_device *dev,
						unsigned short type,
						void *daddr,
						void *saddr,
						unsigned len);
	int			(*rebuild_header)(struct sk_buff *skb);
#define HAVE_MULTICAST			 
	void			(*set_multicast_list)(struct net_device *dev);
#define HAVE_SET_MAC_ADDR  		 
	int			(*set_mac_address)(struct net_device *dev,
						   void *addr);
#define HAVE_PRIVATE_IOCTL
	int			(*do_ioctl)(struct net_device *dev,
					    struct ifreq *ifr, int cmd);
#define HAVE_SET_CONFIG
	int			(*set_config)(struct net_device *dev,
					      struct ifmap *map);
#define HAVE_HEADER_CACHE
	int			(*hard_header_cache)(struct neighbour *neigh,
						     struct hh_cache *hh);
	void			(*header_cache_update)(struct hh_cache *hh,
						       struct net_device *dev,
						       unsigned char *  haddr);
#define HAVE_CHANGE_MTU
	int			(*change_mtu)(struct net_device *dev, int new_mtu);

#define HAVE_TX_TIMEOUT
	void			(*tx_timeout) (struct net_device *dev);

	void			(*vlan_rx_register)(struct net_device *dev,
						    struct vlan_group *grp);
	void			(*vlan_rx_add_vid)(struct net_device *dev,
						   unsigned short vid);
	void			(*vlan_rx_kill_vid)(struct net_device *dev,
						    unsigned short vid);

	int			(*hard_header_parse)(struct sk_buff *skb,
						     unsigned char *haddr);
	int			(*neigh_setup)(struct net_device *dev, struct neigh_parms *);
#ifdef CONFIG_NETPOLL
	struct netpoll_info	*npinfo;
#endif
#ifdef CONFIG_NET_POLL_CONTROLLER
	void                    (*poll_controller)(struct net_device *dev);
#endif

	
	struct net_bridge_port	*br_port;

#ifdef CONFIG_NET_DIVERT
	
	struct divert_blk	*divert;
#endif 

	
	struct class_device	class_dev;
	
	struct attribute_group  *sysfs_groups[3];
};

#define	NETDEV_ALIGN		32
#define	NETDEV_ALIGN_CONST	(NETDEV_ALIGN - 1)

static inline void *netdev_priv(struct net_device *dev)
{
	return (char *)dev + ((sizeof(struct net_device)
					+ NETDEV_ALIGN_CONST)
				& ~NETDEV_ALIGN_CONST);
}

#define SET_MODULE_OWNER(dev) do { } while (0)
#define SET_NETDEV_DEV(net, pdev)	((net)->class_dev.dev = (pdev))

struct packet_type {
	__be16			type;	
	struct net_device	*dev;	
	int			(*func) (struct sk_buff *,
					 struct net_device *,
					 struct packet_type *,
					 struct net_device *);
	struct sk_buff		*(*gso_segment)(struct sk_buff *skb,
						int features);
	int			(*gso_send_check)(struct sk_buff *skb);
	void			*af_packet_priv;
	struct list_head	list;
};

#include <linux/interrupt.h>
#include <linux/notifier.h>

extern struct net_device		loopback_dev;		
extern struct net_device		*dev_base;		
extern rwlock_t				dev_base_lock;		

extern int 			netdev_boot_setup_check(struct net_device *dev);
extern unsigned long		netdev_boot_base(const char *prefix, int unit);
extern struct net_device    *dev_getbyhwaddr(unsigned short type, char *hwaddr);
extern struct net_device *dev_getfirstbyhwtype(unsigned short type);
extern void		dev_add_pack(struct packet_type *pt);
extern void		dev_remove_pack(struct packet_type *pt);
extern void		__dev_remove_pack(struct packet_type *pt);

extern struct net_device	*dev_get_by_flags(unsigned short flags,
						  unsigned short mask);
extern struct net_device	*dev_get_by_name(const char *name);
extern struct net_device	*__dev_get_by_name(const char *name);
extern int		dev_alloc_name(struct net_device *dev, const char *name);
extern int		dev_open(struct net_device *dev);
extern int		dev_close(struct net_device *dev);
extern int		dev_queue_xmit(struct sk_buff *skb);
extern int		register_netdevice(struct net_device *dev);
extern int		unregister_netdevice(struct net_device *dev);
extern void		free_netdev(struct net_device *dev);
extern void		synchronize_net(void);
extern int 		register_netdevice_notifier(struct notifier_block *nb);
extern int		unregister_netdevice_notifier(struct notifier_block *nb);
extern int		call_netdevice_notifiers(unsigned long val, void *v);
extern struct net_device	*dev_get_by_index(int ifindex);
extern struct net_device	*__dev_get_by_index(int ifindex);
extern int		dev_restart(struct net_device *dev);
#ifdef CONFIG_NETPOLL_TRAP
extern int		netpoll_trap(void);
#endif

typedef int gifconf_func_t(struct net_device * dev, char __user * bufptr, int len);
extern int		register_gifconf(unsigned int family, gifconf_func_t * gifconf);
static inline int unregister_gifconf(unsigned int family)
{
	return register_gifconf(family, NULL);
}


struct softnet_data
{
	struct net_device	*output_queue;
	struct sk_buff_head	input_pkt_queue;
	struct list_head	poll_list;
	struct sk_buff		*completion_queue;

	struct net_device	backlog_dev;	
#ifdef CONFIG_NET_DMA
	struct dma_chan		*net_dma;
#endif
};

DECLARE_PER_CPU(struct softnet_data,softnet_data);

#define HAVE_NETIF_QUEUE

extern void __netif_schedule(struct net_device *dev);

static inline void netif_schedule(struct net_device *dev)
{
	if (!test_bit(__LINK_STATE_XOFF, &dev->state))
		__netif_schedule(dev);
}

static inline void netif_start_queue(struct net_device *dev)
{
	clear_bit(__LINK_STATE_XOFF, &dev->state);
}

static inline void netif_wake_queue(struct net_device *dev)
{
#ifdef CONFIG_NETPOLL_TRAP
	if (netpoll_trap())
		return;
#endif
	if (test_and_clear_bit(__LINK_STATE_XOFF, &dev->state))
		__netif_schedule(dev);
}

static inline void netif_stop_queue(struct net_device *dev)
{
#ifdef CONFIG_NETPOLL_TRAP
	if (netpoll_trap())
		return;
#endif
	set_bit(__LINK_STATE_XOFF, &dev->state);
}

static inline int netif_queue_stopped(const struct net_device *dev)
{
	return test_bit(__LINK_STATE_XOFF, &dev->state);
}

static inline int netif_running(const struct net_device *dev)
{
	return test_bit(__LINK_STATE_START, &dev->state);
}


static inline void dev_kfree_skb_irq(struct sk_buff *skb)
{
	if (atomic_dec_and_test(&skb->users)) {
		struct softnet_data *sd;
		unsigned long flags;

		local_irq_save(flags);
		sd = &__get_cpu_var(softnet_data);
		skb->next = sd->completion_queue;
		sd->completion_queue = skb;
		raise_softirq_irqoff(NET_TX_SOFTIRQ);
		local_irq_restore(flags);
	}
}

extern void dev_kfree_skb_any(struct sk_buff *skb);

#define HAVE_NETIF_RX 1
extern int		netif_rx(struct sk_buff *skb);
extern int		netif_rx_ni(struct sk_buff *skb);
#define HAVE_NETIF_RECEIVE_SKB 1
extern int		netif_receive_skb(struct sk_buff *skb);
extern int		dev_valid_name(const char *name);
extern int		dev_ioctl(unsigned int cmd, void __user *);
extern int		dev_ethtool(struct ifreq *);
extern unsigned		dev_get_flags(const struct net_device *);
extern int		dev_change_flags(struct net_device *, unsigned);
extern int		dev_change_name(struct net_device *, char *);
extern int		dev_set_mtu(struct net_device *, int);
extern int		dev_set_mac_address(struct net_device *,
					    struct sockaddr *);
extern int		dev_hard_start_xmit(struct sk_buff *skb,
					    struct net_device *dev);

extern void		dev_init(void);

extern int		netdev_budget;

extern void netdev_run_todo(void);

static inline void dev_put(struct net_device *dev)
{
	atomic_dec(&dev->refcnt);
}

static inline void dev_hold(struct net_device *dev)
{
	atomic_inc(&dev->refcnt);
}


extern void linkwatch_fire_event(struct net_device *dev);

static inline int netif_carrier_ok(const struct net_device *dev)
{
	return !test_bit(__LINK_STATE_NOCARRIER, &dev->state);
}

extern void __netdev_watchdog_up(struct net_device *dev);

extern void netif_carrier_on(struct net_device *dev);

extern void netif_carrier_off(struct net_device *dev);

static inline void netif_dormant_on(struct net_device *dev)
{
	if (!test_and_set_bit(__LINK_STATE_DORMANT, &dev->state))
		linkwatch_fire_event(dev);
}

static inline void netif_dormant_off(struct net_device *dev)
{
	if (test_and_clear_bit(__LINK_STATE_DORMANT, &dev->state))
		linkwatch_fire_event(dev);
}

static inline int netif_dormant(const struct net_device *dev)
{
	return test_bit(__LINK_STATE_DORMANT, &dev->state);
}


static inline int netif_oper_up(const struct net_device *dev) {
	return (dev->operstate == IF_OPER_UP ||
		dev->operstate == IF_OPER_UNKNOWN );
}

static inline int netif_device_present(struct net_device *dev)
{
	return test_bit(__LINK_STATE_PRESENT, &dev->state);
}

extern void netif_device_detach(struct net_device *dev);

extern void netif_device_attach(struct net_device *dev);

#define HAVE_NETIF_MSG 1

enum {
	NETIF_MSG_DRV		= 0x0001,
	NETIF_MSG_PROBE		= 0x0002,
	NETIF_MSG_LINK		= 0x0004,
	NETIF_MSG_TIMER		= 0x0008,
	NETIF_MSG_IFDOWN	= 0x0010,
	NETIF_MSG_IFUP		= 0x0020,
	NETIF_MSG_RX_ERR	= 0x0040,
	NETIF_MSG_TX_ERR	= 0x0080,
	NETIF_MSG_TX_QUEUED	= 0x0100,
	NETIF_MSG_INTR		= 0x0200,
	NETIF_MSG_TX_DONE	= 0x0400,
	NETIF_MSG_RX_STATUS	= 0x0800,
	NETIF_MSG_PKTDATA	= 0x1000,
	NETIF_MSG_HW		= 0x2000,
	NETIF_MSG_WOL		= 0x4000,
};

#define netif_msg_drv(p)	((p)->msg_enable & NETIF_MSG_DRV)
#define netif_msg_probe(p)	((p)->msg_enable & NETIF_MSG_PROBE)
#define netif_msg_link(p)	((p)->msg_enable & NETIF_MSG_LINK)
#define netif_msg_timer(p)	((p)->msg_enable & NETIF_MSG_TIMER)
#define netif_msg_ifdown(p)	((p)->msg_enable & NETIF_MSG_IFDOWN)
#define netif_msg_ifup(p)	((p)->msg_enable & NETIF_MSG_IFUP)
#define netif_msg_rx_err(p)	((p)->msg_enable & NETIF_MSG_RX_ERR)
#define netif_msg_tx_err(p)	((p)->msg_enable & NETIF_MSG_TX_ERR)
#define netif_msg_tx_queued(p)	((p)->msg_enable & NETIF_MSG_TX_QUEUED)
#define netif_msg_intr(p)	((p)->msg_enable & NETIF_MSG_INTR)
#define netif_msg_tx_done(p)	((p)->msg_enable & NETIF_MSG_TX_DONE)
#define netif_msg_rx_status(p)	((p)->msg_enable & NETIF_MSG_RX_STATUS)
#define netif_msg_pktdata(p)	((p)->msg_enable & NETIF_MSG_PKTDATA)
#define netif_msg_hw(p)		((p)->msg_enable & NETIF_MSG_HW)
#define netif_msg_wol(p)	((p)->msg_enable & NETIF_MSG_WOL)

static inline u32 netif_msg_init(int debug_value, int default_msg_enable_bits)
{
	
	if (debug_value < 0 || debug_value >= (sizeof(u32) * 8))
		return default_msg_enable_bits;
	if (debug_value == 0)	
		return 0;
	
	return (1 << debug_value) - 1;
}

static inline int __netif_rx_schedule_prep(struct net_device *dev)
{
	return !test_and_set_bit(__LINK_STATE_RX_SCHED, &dev->state);
}

static inline int netif_rx_schedule_prep(struct net_device *dev)
{
	return netif_running(dev) && __netif_rx_schedule_prep(dev);
}


extern void __netif_rx_schedule(struct net_device *dev);


static inline void netif_rx_schedule(struct net_device *dev)
{
	if (netif_rx_schedule_prep(dev))
		__netif_rx_schedule(dev);
}

static inline int netif_rx_reschedule(struct net_device *dev, int undo)
{
	if (netif_rx_schedule_prep(dev)) {
		unsigned long flags;

		dev->quota += undo;

		local_irq_save(flags);
		list_add_tail(&dev->poll_list, &__get_cpu_var(softnet_data).poll_list);
		__raise_softirq_irqoff(NET_RX_SOFTIRQ);
		local_irq_restore(flags);
		return 1;
	}
	return 0;
}

static inline void netif_rx_complete(struct net_device *dev)
{
	unsigned long flags;

	local_irq_save(flags);
	BUG_ON(!test_bit(__LINK_STATE_RX_SCHED, &dev->state));
	list_del(&dev->poll_list);
	smp_mb__before_clear_bit();
	clear_bit(__LINK_STATE_RX_SCHED, &dev->state);
	local_irq_restore(flags);
}

static inline void netif_poll_disable(struct net_device *dev)
{
	while (test_and_set_bit(__LINK_STATE_RX_SCHED, &dev->state))
		
		schedule_timeout_interruptible(1);
}

static inline void netif_poll_enable(struct net_device *dev)
{
	clear_bit(__LINK_STATE_RX_SCHED, &dev->state);
}

static inline void __netif_rx_complete(struct net_device *dev)
{
	BUG_ON(!test_bit(__LINK_STATE_RX_SCHED, &dev->state));
	list_del(&dev->poll_list);
	smp_mb__before_clear_bit();
	clear_bit(__LINK_STATE_RX_SCHED, &dev->state);
}

static inline void netif_tx_lock(struct net_device *dev)
{
	spin_lock(&dev->_xmit_lock);
	dev->xmit_lock_owner = smp_processor_id();
}

static inline void netif_tx_lock_bh(struct net_device *dev)
{
	spin_lock_bh(&dev->_xmit_lock);
	dev->xmit_lock_owner = smp_processor_id();
}

static inline int netif_tx_trylock(struct net_device *dev)
{
	int ok = spin_trylock(&dev->_xmit_lock);
	if (likely(ok))
		dev->xmit_lock_owner = smp_processor_id();
	return ok;
}

static inline void netif_tx_unlock(struct net_device *dev)
{
	dev->xmit_lock_owner = -1;
	spin_unlock(&dev->_xmit_lock);
}

static inline void netif_tx_unlock_bh(struct net_device *dev)
{
	dev->xmit_lock_owner = -1;
	spin_unlock_bh(&dev->_xmit_lock);
}

static inline void netif_tx_disable(struct net_device *dev)
{
	netif_tx_lock_bh(dev);
	netif_stop_queue(dev);
	netif_tx_unlock_bh(dev);
}


extern void		ether_setup(struct net_device *dev);

extern struct net_device *alloc_netdev(int sizeof_priv, const char *name,
				       void (*setup)(struct net_device *));
extern int		register_netdev(struct net_device *dev);
extern void		unregister_netdev(struct net_device *dev);
extern void		dev_mc_upload(struct net_device *dev);
extern int 		dev_mc_delete(struct net_device *dev, void *addr, int alen, int all);
extern int		dev_mc_add(struct net_device *dev, void *addr, int alen, int newonly);
extern void		dev_mc_discard(struct net_device *dev);
extern void		dev_set_promiscuity(struct net_device *dev, int inc);
extern void		dev_set_allmulti(struct net_device *dev, int inc);
extern void		netdev_state_change(struct net_device *dev);
extern void		netdev_features_change(struct net_device *dev);
extern void		dev_load(const char *name);
extern void		dev_mcast_init(void);
extern int		netdev_max_backlog;
extern int		weight_p;
extern int		netdev_set_master(struct net_device *dev, struct net_device *master);
extern int skb_checksum_help(struct sk_buff *skb, int inward);
extern struct sk_buff *skb_gso_segment(struct sk_buff *skb, int features);
#ifdef CONFIG_BUG
extern void netdev_rx_csum_fault(struct net_device *dev);
#else
static inline void netdev_rx_csum_fault(struct net_device *dev)
{
}
#endif
extern void		net_enable_timestamp(void);
extern void		net_disable_timestamp(void);

#ifdef CONFIG_PROC_FS
extern void *dev_seq_start(struct seq_file *seq, loff_t *pos);
extern void *dev_seq_next(struct seq_file *seq, void *v, loff_t *pos);
extern void dev_seq_stop(struct seq_file *seq, void *v);
#endif

extern void linkwatch_run_queue(void);

static inline int net_gso_ok(int features, int gso_type)
{
	int feature = gso_type << NETIF_F_GSO_SHIFT;
	return (features & feature) == feature;
}

static inline int skb_gso_ok(struct sk_buff *skb, int features)
{
	return net_gso_ok(features, skb_shinfo(skb)->gso_type);
}

static inline int netif_needs_gso(struct net_device *dev, struct sk_buff *skb)
{
	return skb_is_gso(skb) &&
	       (!skb_gso_ok(skb, dev->features) ||
		unlikely(skb->ip_summed != CHECKSUM_HW));
}

static inline int skb_bond_should_drop(struct sk_buff *skb)
{
	struct net_device *dev = skb->dev;
	struct net_device *master = dev->master;

	if (master &&
	    (dev->priv_flags & IFF_SLAVE_INACTIVE)) {
		if (master->priv_flags & IFF_MASTER_ALB) {
			if (skb->pkt_type != PACKET_BROADCAST &&
			    skb->pkt_type != PACKET_MULTICAST)
				return 0;
		}
		if (master->priv_flags & IFF_MASTER_8023AD &&
		    skb->protocol == __constant_htons(ETH_P_SLOW))
			return 0;

		return 1;
	}
	return 0;
}

#endif 

#endif	
