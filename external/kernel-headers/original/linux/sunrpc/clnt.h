/*
 *  linux/include/linux/sunrpc/clnt.h
 *
 *  Declarations for the high-level RPC client interface
 *
 *  Copyright (C) 1995, 1996, Olaf Kirch <okir@monad.swb.de>
 */

#ifndef _LINUX_SUNRPC_CLNT_H
#define _LINUX_SUNRPC_CLNT_H

#include <linux/sunrpc/msg_prot.h>
#include <linux/sunrpc/sched.h>
#include <linux/sunrpc/xprt.h>
#include <linux/sunrpc/auth.h>
#include <linux/sunrpc/stats.h>
#include <linux/sunrpc/xdr.h>
#include <linux/sunrpc/timer.h>
#include <asm/signal.h>

struct rpc_portmap {
	__u32			pm_prog;
	__u32			pm_vers;
	__u32			pm_prot;
	__u16			pm_port;
	unsigned char		pm_binding : 1;	
	struct rpc_wait_queue	pm_bindwait;	
};

struct rpc_inode;

struct rpc_clnt {
	atomic_t		cl_count;	
	atomic_t		cl_users;	
	struct rpc_xprt *	cl_xprt;	
	struct rpc_procinfo *	cl_procinfo;	
	u32			cl_maxproc;	

	char *			cl_server;	
	char *			cl_protname;	
	struct rpc_auth *	cl_auth;	
	struct rpc_stat *	cl_stats;	
	struct rpc_iostats *	cl_metrics;	

	unsigned int		cl_softrtry : 1,
				cl_intr     : 1,
				cl_autobind : 1,
				cl_oneshot  : 1,
				cl_dead     : 1;

	struct rpc_rtt *	cl_rtt;		
	struct rpc_portmap *	cl_pmap;	

	int			cl_nodelen;	
	char 			cl_nodename[UNX_MAXNODENAME];
	char			cl_pathname[30];
	struct vfsmount *	cl_vfsmnt;
	struct dentry *		cl_dentry;	
	struct rpc_clnt *	cl_parent;	
	struct rpc_rtt		cl_rtt_default;
	struct rpc_portmap	cl_pmap_default;
	char			cl_inline_name[32];
};
#define cl_timeout		cl_xprt->timeout
#define cl_prog			cl_pmap->pm_prog
#define cl_vers			cl_pmap->pm_vers
#define cl_port			cl_pmap->pm_port
#define cl_prot			cl_pmap->pm_prot

#define RPC_MAXVERSION		4
struct rpc_program {
	char *			name;		
	u32			number;		
	unsigned int		nrvers;		
	struct rpc_version **	version;	
	struct rpc_stat *	stats;		
	char *			pipe_dir_name;	
};

struct rpc_version {
	u32			number;		
	unsigned int		nrprocs;	
	struct rpc_procinfo *	procs;		
};

struct rpc_procinfo {
	u32			p_proc;		
	kxdrproc_t		p_encode;	
	kxdrproc_t		p_decode;	
	unsigned int		p_bufsiz;	
	unsigned int		p_count;	
	unsigned int		p_timer;	
	u32			p_statidx;	
	char *			p_name;		
};

#define RPC_CONGESTED(clnt)	(RPCXPRT_CONGESTED((clnt)->cl_xprt))
#define RPC_PEERADDR(clnt)	(&(clnt)->cl_xprt->addr)

#ifdef __KERNEL__

struct rpc_clnt *rpc_create_client(struct rpc_xprt *xprt, char *servname,
				struct rpc_program *info,
				u32 version, rpc_authflavor_t authflavor);
struct rpc_clnt *rpc_new_client(struct rpc_xprt *xprt, char *servname,
				struct rpc_program *info,
				u32 version, rpc_authflavor_t authflavor);
struct rpc_clnt	*rpc_bind_new_program(struct rpc_clnt *,
				struct rpc_program *, int);
struct rpc_clnt *rpc_clone_client(struct rpc_clnt *);
int		rpc_shutdown_client(struct rpc_clnt *);
int		rpc_destroy_client(struct rpc_clnt *);
void		rpc_release_client(struct rpc_clnt *);
void		rpc_getport(struct rpc_task *, struct rpc_clnt *);
int		rpc_register(u32, u32, int, unsigned short, int *);

void		rpc_call_setup(struct rpc_task *, struct rpc_message *, int);

int		rpc_call_async(struct rpc_clnt *clnt, struct rpc_message *msg,
			       int flags, const struct rpc_call_ops *tk_ops,
			       void *calldata);
int		rpc_call_sync(struct rpc_clnt *clnt, struct rpc_message *msg,
			      int flags);
void		rpc_restart_call(struct rpc_task *);
void		rpc_clnt_sigmask(struct rpc_clnt *clnt, sigset_t *oldset);
void		rpc_clnt_sigunmask(struct rpc_clnt *clnt, sigset_t *oldset);
void		rpc_setbufsize(struct rpc_clnt *, unsigned int, unsigned int);
size_t		rpc_max_payload(struct rpc_clnt *);
void		rpc_force_rebind(struct rpc_clnt *);
int		rpc_ping(struct rpc_clnt *clnt, int flags);

int		rpc_getport_external(struct sockaddr_in *, __u32, __u32, int);

#endif 
#endif 
