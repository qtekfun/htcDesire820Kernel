/*
 * ethtool.h: Defines for Linux ethtool.
 *
 * Copyright (C) 1998 David S. Miller (davem@redhat.com)
 * Copyright 2001 Jeff Garzik <jgarzik@pobox.com>
 * Portions Copyright 2001 Sun Microsystems (thockin@sun.com)
 * Portions Copyright 2002 Intel (eli.kupermann@intel.com,
 *                                christopher.leech@intel.com,
 *                                scott.feldman@intel.com)
 */

#ifndef _LINUX_ETHTOOL_H
#define _LINUX_ETHTOOL_H

#include <linux/types.h>

struct ethtool_cmd {
	__u32	cmd;
	__u32	supported;	
	__u32	advertising;	
	__u16	speed;		
	__u8	duplex;		
	__u8	port;		
	__u8	phy_address;
	__u8	transceiver;	
	__u8	autoneg;	
	__u32	maxtxpkt;	
	__u32	maxrxpkt;	
	__u16	speed_hi;
	__u16	reserved2;
	__u32	reserved[3];
};

static inline void ethtool_cmd_speed_set(struct ethtool_cmd *ep,
						__u32 speed)
{

	ep->speed = (__u16)speed;
	ep->speed_hi = (__u16)(speed >> 16);
}

static inline __u32 ethtool_cmd_speed(struct ethtool_cmd *ep)
{
	return (ep->speed_hi << 16) | ep->speed;
}

#define ETHTOOL_BUSINFO_LEN	32
struct ethtool_drvinfo {
	__u32	cmd;
	char	driver[32];	
	char	version[32];	
	char	fw_version[32];	
	char	bus_info[ETHTOOL_BUSINFO_LEN];	
				
	char	reserved1[32];
	char	reserved2[12];
	__u32	n_priv_flags;	
	__u32	n_stats;	
	__u32	testinfo_len;
	__u32	eedump_len;	
	__u32	regdump_len;	
};

#define SOPASS_MAX	6
struct ethtool_wolinfo {
	__u32	cmd;
	__u32	supported;
	__u32	wolopts;
	__u8	sopass[SOPASS_MAX]; 
};

struct ethtool_value {
	__u32	cmd;
	__u32	data;
};

struct ethtool_regs {
	__u32	cmd;
	__u32	version; 
	__u32	len; 
	__u8	data[0];
};

struct ethtool_eeprom {
	__u32	cmd;
	__u32	magic;
	__u32	offset; 
	__u32	len; 
	__u8	data[0];
};

struct ethtool_coalesce {
	__u32	cmd;	

	__u32	rx_coalesce_usecs;

	__u32	rx_max_coalesced_frames;

	__u32	rx_coalesce_usecs_irq;
	__u32	rx_max_coalesced_frames_irq;

	__u32	tx_coalesce_usecs;

	__u32	tx_max_coalesced_frames;

	__u32	tx_coalesce_usecs_irq;
	__u32	tx_max_coalesced_frames_irq;

	__u32	stats_block_coalesce_usecs;

	__u32	use_adaptive_rx_coalesce;
	__u32	use_adaptive_tx_coalesce;

	__u32	pkt_rate_low;
	__u32	rx_coalesce_usecs_low;
	__u32	rx_max_coalesced_frames_low;
	__u32	tx_coalesce_usecs_low;
	__u32	tx_max_coalesced_frames_low;


	__u32	pkt_rate_high;
	__u32	rx_coalesce_usecs_high;
	__u32	rx_max_coalesced_frames_high;
	__u32	tx_coalesce_usecs_high;
	__u32	tx_max_coalesced_frames_high;

	__u32	rate_sample_interval;
};

struct ethtool_ringparam {
	__u32	cmd;	

	__u32	rx_max_pending;
	__u32	rx_mini_max_pending;
	__u32	rx_jumbo_max_pending;
	__u32	tx_max_pending;

	__u32	rx_pending;
	__u32	rx_mini_pending;
	__u32	rx_jumbo_pending;
	__u32	tx_pending;
};

struct ethtool_pauseparam {
	__u32	cmd;	

	__u32	autoneg;
	__u32	rx_pause;
	__u32	tx_pause;
};

#define ETH_GSTRING_LEN		32
enum ethtool_stringset {
	ETH_SS_TEST		= 0,
	ETH_SS_STATS,
	ETH_SS_PRIV_FLAGS,
};

struct ethtool_gstrings {
	__u32	cmd;		
	__u32	string_set;	
	__u32	len;		
	__u8	data[0];
};

enum ethtool_test_flags {
	ETH_TEST_FL_OFFLINE	= (1 << 0),	
	ETH_TEST_FL_FAILED	= (1 << 1),	
};

struct ethtool_test {
	__u32	cmd;		
	__u32	flags;		
	__u32	reserved;
	__u32	len;		
	__u64	data[0];
};

struct ethtool_stats {
	__u32	cmd;		
	__u32	n_stats;	
	__u64	data[0];
};

struct ethtool_perm_addr {
	__u32	cmd;		
	__u32	size;
	__u8	data[0];
};

enum ethtool_flags {
	ETH_FLAG_LRO		= (1 << 15),	
};

struct ethtool_rxnfc {
	__u32		cmd;
	__u32		flow_type;
	__u64		data;
};

#ifdef __KERNEL__

struct net_device;

u32 ethtool_op_get_link(struct net_device *dev);
u32 ethtool_op_get_tx_csum(struct net_device *dev);
int ethtool_op_set_tx_csum(struct net_device *dev, u32 data);
int ethtool_op_set_tx_hw_csum(struct net_device *dev, u32 data);
int ethtool_op_set_tx_ipv6_csum(struct net_device *dev, u32 data);
u32 ethtool_op_get_sg(struct net_device *dev);
int ethtool_op_set_sg(struct net_device *dev, u32 data);
u32 ethtool_op_get_tso(struct net_device *dev);
int ethtool_op_set_tso(struct net_device *dev, u32 data);
u32 ethtool_op_get_ufo(struct net_device *dev);
int ethtool_op_set_ufo(struct net_device *dev, u32 data);
u32 ethtool_op_get_flags(struct net_device *dev);
int ethtool_op_set_flags(struct net_device *dev, u32 data);

/**
 * &ethtool_ops - Alter and report network device settings
 * get_settings: Get device-specific settings
 * set_settings: Set device-specific settings
 * get_drvinfo: Report driver information
 * get_regs: Get device registers
 * get_wol: Report whether Wake-on-Lan is enabled
 * set_wol: Turn Wake-on-Lan on or off
 * get_msglevel: Report driver message level
 * set_msglevel: Set driver message level
 * nway_reset: Restart autonegotiation
 * get_link: Get link status
 * get_eeprom: Read data from the device EEPROM
 * set_eeprom: Write data to the device EEPROM
 * get_coalesce: Get interrupt coalescing parameters
 * set_coalesce: Set interrupt coalescing parameters
 * get_ringparam: Report ring sizes
 * set_ringparam: Set ring sizes
 * get_pauseparam: Report pause parameters
 * set_pauseparam: Set pause parameters
 * get_rx_csum: Report whether receive checksums are turned on or off
 * set_rx_csum: Turn receive checksum on or off
 * get_tx_csum: Report whether transmit checksums are turned on or off
 * set_tx_csum: Turn transmit checksums on or off
 * get_sg: Report whether scatter-gather is enabled
 * set_sg: Turn scatter-gather on or off
 * get_tso: Report whether TCP segmentation offload is enabled
 * set_tso: Turn TCP segmentation offload on or off
 * get_ufo: Report whether UDP fragmentation offload is enabled
 * set_ufo: Turn UDP fragmentation offload on or off
 * self_test: Run specified self-tests
 * get_strings: Return a set of strings that describe the requested objects 
 * phys_id: Identify the device
 * get_stats: Return statistics about the device
 * get_flags: get 32-bit flags bitmap
 * set_flags: set 32-bit flags bitmap
 * 
 * Description:
 *
 * get_settings:
 *	@get_settings is passed an &ethtool_cmd to fill in.  It returns
 *	an negative errno or zero.
 *
 * set_settings:
 *	@set_settings is passed an &ethtool_cmd and should attempt to set
 *	all the settings this device supports.  It may return an error value
 *	if something goes wrong (otherwise 0).
 *
 * get_eeprom:
 *	Should fill in the magic field.  Don't need to check len for zero
 *	or wraparound.  Fill in the data argument with the eeprom values
 *	from offset to offset + len.  Update len to the amount read.
 *	Returns an error or zero.
 *
 * set_eeprom:
 *	Should validate the magic field.  Don't need to check len for zero
 *	or wraparound.  Update len to the amount written.  Returns an error
 *	or zero.
 */
struct ethtool_ops {
	int	(*get_settings)(struct net_device *, struct ethtool_cmd *);
	int	(*set_settings)(struct net_device *, struct ethtool_cmd *);
	void	(*get_drvinfo)(struct net_device *, struct ethtool_drvinfo *);
	int	(*get_regs_len)(struct net_device *);
	void	(*get_regs)(struct net_device *, struct ethtool_regs *, void *);
	void	(*get_wol)(struct net_device *, struct ethtool_wolinfo *);
	int	(*set_wol)(struct net_device *, struct ethtool_wolinfo *);
	u32	(*get_msglevel)(struct net_device *);
	void	(*set_msglevel)(struct net_device *, u32);
	int	(*nway_reset)(struct net_device *);
	u32	(*get_link)(struct net_device *);
	int	(*get_eeprom_len)(struct net_device *);
	int	(*get_eeprom)(struct net_device *, struct ethtool_eeprom *, u8 *);
	int	(*set_eeprom)(struct net_device *, struct ethtool_eeprom *, u8 *);
	int	(*get_coalesce)(struct net_device *, struct ethtool_coalesce *);
	int	(*set_coalesce)(struct net_device *, struct ethtool_coalesce *);
	void	(*get_ringparam)(struct net_device *, struct ethtool_ringparam *);
	int	(*set_ringparam)(struct net_device *, struct ethtool_ringparam *);
	void	(*get_pauseparam)(struct net_device *, struct ethtool_pauseparam*);
	int	(*set_pauseparam)(struct net_device *, struct ethtool_pauseparam*);
	u32	(*get_rx_csum)(struct net_device *);
	int	(*set_rx_csum)(struct net_device *, u32);
	u32	(*get_tx_csum)(struct net_device *);
	int	(*set_tx_csum)(struct net_device *, u32);
	u32	(*get_sg)(struct net_device *);
	int	(*set_sg)(struct net_device *, u32);
	u32	(*get_tso)(struct net_device *);
	int	(*set_tso)(struct net_device *, u32);
	void	(*self_test)(struct net_device *, struct ethtool_test *, u64 *);
	void	(*get_strings)(struct net_device *, u32 stringset, u8 *);
	int	(*phys_id)(struct net_device *, u32);
	void	(*get_ethtool_stats)(struct net_device *, struct ethtool_stats *, u64 *);
	int	(*begin)(struct net_device *);
	void	(*complete)(struct net_device *);
	u32     (*get_ufo)(struct net_device *);
	int     (*set_ufo)(struct net_device *, u32);
	u32     (*get_flags)(struct net_device *);
	int     (*set_flags)(struct net_device *, u32);
	u32     (*get_priv_flags)(struct net_device *);
	int     (*set_priv_flags)(struct net_device *, u32);
	int	(*get_sset_count)(struct net_device *, int);

	
	int	(*self_test_count)(struct net_device *);
	int	(*get_stats_count)(struct net_device *);
	int	(*get_rxhash)(struct net_device *, struct ethtool_rxnfc *);
	int	(*set_rxhash)(struct net_device *, struct ethtool_rxnfc *);
};
#endif 

#define ETHTOOL_GSET		0x00000001 
#define ETHTOOL_SSET		0x00000002 
#define ETHTOOL_GDRVINFO	0x00000003 
#define ETHTOOL_GREGS		0x00000004 
#define ETHTOOL_GWOL		0x00000005 
#define ETHTOOL_SWOL		0x00000006 
#define ETHTOOL_GMSGLVL		0x00000007 
#define ETHTOOL_SMSGLVL		0x00000008 
#define ETHTOOL_NWAY_RST	0x00000009 
#define ETHTOOL_GLINK		0x0000000a 
#define ETHTOOL_GEEPROM		0x0000000b 
#define ETHTOOL_SEEPROM		0x0000000c 
#define ETHTOOL_GCOALESCE	0x0000000e 
#define ETHTOOL_SCOALESCE	0x0000000f 
#define ETHTOOL_GRINGPARAM	0x00000010 
#define ETHTOOL_SRINGPARAM	0x00000011 
#define ETHTOOL_GPAUSEPARAM	0x00000012 
#define ETHTOOL_SPAUSEPARAM	0x00000013 
#define ETHTOOL_GRXCSUM		0x00000014 
#define ETHTOOL_SRXCSUM		0x00000015 
#define ETHTOOL_GTXCSUM		0x00000016 
#define ETHTOOL_STXCSUM		0x00000017 
#define ETHTOOL_GSG		0x00000018 
#define ETHTOOL_SSG		0x00000019 
#define ETHTOOL_TEST		0x0000001a 
#define ETHTOOL_GSTRINGS	0x0000001b 
#define ETHTOOL_PHYS_ID		0x0000001c 
#define ETHTOOL_GSTATS		0x0000001d 
#define ETHTOOL_GTSO		0x0000001e 
#define ETHTOOL_STSO		0x0000001f 
#define ETHTOOL_GPERMADDR	0x00000020 
#define ETHTOOL_GUFO		0x00000021 
#define ETHTOOL_SUFO		0x00000022 
#define ETHTOOL_GGSO		0x00000023 
#define ETHTOOL_SGSO		0x00000024 
#define ETHTOOL_GFLAGS		0x00000025 
#define ETHTOOL_SFLAGS		0x00000026 
#define ETHTOOL_GPFLAGS		0x00000027 
#define ETHTOOL_SPFLAGS		0x00000028 

#define	ETHTOOL_GRXFH		0x00000029 
#define	ETHTOOL_SRXFH		0x0000002a 
#define ETHTOOL_GGRO		0x0000002b 
#define ETHTOOL_SGRO		0x0000002c 

#define SPARC_ETH_GSET		ETHTOOL_GSET
#define SPARC_ETH_SSET		ETHTOOL_SSET

#define SUPPORTED_10baseT_Half		(1 << 0)
#define SUPPORTED_10baseT_Full		(1 << 1)
#define SUPPORTED_100baseT_Half		(1 << 2)
#define SUPPORTED_100baseT_Full		(1 << 3)
#define SUPPORTED_1000baseT_Half	(1 << 4)
#define SUPPORTED_1000baseT_Full	(1 << 5)
#define SUPPORTED_Autoneg		(1 << 6)
#define SUPPORTED_TP			(1 << 7)
#define SUPPORTED_AUI			(1 << 8)
#define SUPPORTED_MII			(1 << 9)
#define SUPPORTED_FIBRE			(1 << 10)
#define SUPPORTED_BNC			(1 << 11)
#define SUPPORTED_10000baseT_Full	(1 << 12)
#define SUPPORTED_Pause			(1 << 13)
#define SUPPORTED_Asym_Pause		(1 << 14)
#define SUPPORTED_2500baseX_Full	(1 << 15)

#define ADVERTISED_10baseT_Half		(1 << 0)
#define ADVERTISED_10baseT_Full		(1 << 1)
#define ADVERTISED_100baseT_Half	(1 << 2)
#define ADVERTISED_100baseT_Full	(1 << 3)
#define ADVERTISED_1000baseT_Half	(1 << 4)
#define ADVERTISED_1000baseT_Full	(1 << 5)
#define ADVERTISED_Autoneg		(1 << 6)
#define ADVERTISED_TP			(1 << 7)
#define ADVERTISED_AUI			(1 << 8)
#define ADVERTISED_MII			(1 << 9)
#define ADVERTISED_FIBRE		(1 << 10)
#define ADVERTISED_BNC			(1 << 11)
#define ADVERTISED_10000baseT_Full	(1 << 12)
#define ADVERTISED_Pause		(1 << 13)
#define ADVERTISED_Asym_Pause		(1 << 14)
#define ADVERTISED_2500baseX_Full	(1 << 15)


#define SPEED_10		10
#define SPEED_100		100
#define SPEED_1000		1000
#define SPEED_2500		2500
#define SPEED_10000		10000

#define DUPLEX_HALF		0x00
#define DUPLEX_FULL		0x01

#define PORT_TP			0x00
#define PORT_AUI		0x01
#define PORT_MII		0x02
#define PORT_FIBRE		0x03
#define PORT_BNC		0x04

#define XCVR_INTERNAL		0x00
#define XCVR_EXTERNAL		0x01
#define XCVR_DUMMY1		0x02
#define XCVR_DUMMY2		0x03
#define XCVR_DUMMY3		0x04

#define AUTONEG_DISABLE		0x00
#define AUTONEG_ENABLE		0x01

#define WAKE_PHY		(1 << 0)
#define WAKE_UCAST		(1 << 1)
#define WAKE_MCAST		(1 << 2)
#define WAKE_BCAST		(1 << 3)
#define WAKE_ARP		(1 << 4)
#define WAKE_MAGIC		(1 << 5)
#define WAKE_MAGICSECURE	(1 << 6) 

#define	TCP_V4_FLOW	0x01
#define	UDP_V4_FLOW	0x02
#define	SCTP_V4_FLOW	0x03
#define	AH_ESP_V4_FLOW	0x04
#define	TCP_V6_FLOW	0x05
#define	UDP_V6_FLOW	0x06
#define	SCTP_V6_FLOW	0x07
#define	AH_ESP_V6_FLOW	0x08

#define	RXH_DEV_PORT	(1 << 0)
#define	RXH_L2DA	(1 << 1)
#define	RXH_VLAN	(1 << 2)
#define	RXH_L3_PROTO	(1 << 3)
#define	RXH_IP_SRC	(1 << 4)
#define	RXH_IP_DST	(1 << 5)
#define	RXH_L4_B_0_1	(1 << 6) 
#define	RXH_L4_B_2_3	(1 << 7) 
#define	RXH_DISCARD	(1 << 31)


#endif 
