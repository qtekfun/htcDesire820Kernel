#ifndef _LINUX_IPC_H
#define _LINUX_IPC_H

#include <linux/types.h>

#define IPC_PRIVATE ((__kernel_key_t) 0)  

struct ipc_perm
{
	__kernel_key_t	key;
	__kernel_uid_t	uid;
	__kernel_gid_t	gid;
	__kernel_uid_t	cuid;
	__kernel_gid_t	cgid;
	__kernel_mode_t	mode; 
	unsigned short	seq;
};

#include <asm/ipcbuf.h>

#define IPC_CREAT  00001000   
#define IPC_EXCL   00002000   
#define IPC_NOWAIT 00004000   

   
#define IPC_DIPC 00010000  
#define IPC_OWN  00020000  

#define IPC_RMID 0     
#define IPC_SET  1     
#define IPC_STAT 2     
#define IPC_INFO 3     

#define IPC_OLD 0	
#define IPC_64  0x0100  

#ifdef __KERNEL__

#define IPCMNI 32768  

struct kern_ipc_perm
{
	spinlock_t	lock;
	int		deleted;
	key_t		key;
	uid_t		uid;
	gid_t		gid;
	uid_t		cuid;
	gid_t		cgid;
	mode_t		mode; 
	unsigned long	seq;
	void		*security;
};

#endif 

#endif 


