#ifndef _NF_CONNTRACK_FTP_H
#define _NF_CONNTRACK_FTP_H

enum ip_ct_ftp_type
{
	
	IP_CT_FTP_PORT,
	
	IP_CT_FTP_PASV,
	
	IP_CT_FTP_EPRT,
	
	IP_CT_FTP_EPSV,
};

#ifdef __KERNEL__

#define FTP_PORT	21

#define NUM_SEQ_TO_REMEMBER 2
struct ip_ct_ftp_master {
	
	u_int32_t seq_aft_nl[IP_CT_DIR_MAX][NUM_SEQ_TO_REMEMBER];
	
	int seq_aft_nl_num[IP_CT_DIR_MAX];
};

struct ip_conntrack_expect;

extern unsigned int (*ip_nat_ftp_hook)(struct sk_buff **pskb,
				       enum ip_conntrack_info ctinfo,
				       enum ip_ct_ftp_type type,
				       unsigned int matchoff,
				       unsigned int matchlen,
				       struct ip_conntrack_expect *exp,
				       u32 *seq);
#endif 

#endif 
