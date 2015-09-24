
#ifndef _LINUX_RANDOM_H
#define _LINUX_RANDOM_H

#include <linux/ioctl.h>


#define RNDGETENTCNT	_IOR( 'R', 0x00, int )

#define RNDADDTOENTCNT	_IOW( 'R', 0x01, int )

#define RNDGETPOOL	_IOR( 'R', 0x02, int [2] )

#define RNDADDENTROPY	_IOW( 'R', 0x03, int [2] )

#define RNDZAPENTCNT	_IO( 'R', 0x04 )

#define RNDCLEARPOOL	_IO( 'R', 0x06 )

struct rand_pool_info {
	int	entropy_count;
	int	buf_size;
	__u32	buf[0];
};


#ifdef __KERNEL__

extern void rand_initialize_irq(int irq);

extern void add_input_randomness(unsigned int type, unsigned int code,
				 unsigned int value);
extern void add_interrupt_randomness(int irq);

extern void get_random_bytes(void *buf, int nbytes);
void generate_random_uuid(unsigned char uuid_out[16]);

extern __u32 secure_ip_id(__u32 daddr);
extern u32 secure_ipv4_port_ephemeral(__u32 saddr, __u32 daddr, __u16 dport);
extern u32 secure_ipv6_port_ephemeral(const __u32 *saddr, const __u32 *daddr, 
				      __u16 dport);
extern __u32 secure_tcp_sequence_number(__u32 saddr, __u32 daddr,
					__u16 sport, __u16 dport);
extern __u32 secure_tcpv6_sequence_number(__u32 *saddr, __u32 *daddr,
					  __u16 sport, __u16 dport);
extern u64 secure_dccp_sequence_number(__u32 saddr, __u32 daddr,
				       __u16 sport, __u16 dport);

#ifndef MODULE
extern struct file_operations random_fops, urandom_fops;
#endif

unsigned int get_random_int(void);
unsigned long randomize_range(unsigned long start, unsigned long end, unsigned long len);

#endif 

#endif 
