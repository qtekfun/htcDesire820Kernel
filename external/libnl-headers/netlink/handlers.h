/*
 * netlink/handlers.c	default netlink message handlers
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License as published by the Free Software Foundation version 2.1
 *	of the License.
 *
 * Copyright (c) 2003-2006 Thomas Graf <tgraf@suug.ch>
 */

#ifndef NETLINK_HANDLERS_H_
#define NETLINK_HANDLERS_H_

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <netlink/netlink-compat.h>

#include <linux/netlink.h>

#ifdef __cplusplus
extern "C" {
#endif

struct nl_cb;
struct nl_sock;
struct nl_msg;
struct ucred;


typedef int (*nl_recvmsg_msg_cb_t)(struct nl_msg *msg, void *arg);

typedef int (*nl_recvmsg_err_cb_t)(struct sockaddr_nl *nla,
				   struct nlmsgerr *nlerr, void *arg);


enum nl_cb_action {
	
	NL_OK,
	
	NL_SKIP,
	
	NL_STOP,
};

enum nl_cb_kind {
	
	NL_CB_DEFAULT,
	
	NL_CB_VERBOSE,
	
	NL_CB_DEBUG,
	
	NL_CB_CUSTOM,
	__NL_CB_KIND_MAX,
};

#define NL_CB_KIND_MAX (__NL_CB_KIND_MAX - 1)

enum nl_cb_type {
	
	NL_CB_VALID,
	
	NL_CB_FINISH,
	
	NL_CB_OVERRUN,
	
	NL_CB_SKIPPED,
	
	NL_CB_ACK,
	
	NL_CB_MSG_IN,
	
	NL_CB_MSG_OUT,
	
	NL_CB_INVALID,
	
	NL_CB_SEQ_CHECK,
	
	NL_CB_SEND_ACK,
	__NL_CB_TYPE_MAX,
};

#define NL_CB_TYPE_MAX (__NL_CB_TYPE_MAX - 1)

extern struct nl_cb *	nl_cb_alloc(enum nl_cb_kind);
extern struct nl_cb *	nl_cb_clone(struct nl_cb *);
extern struct nl_cb *	nl_cb_get(struct nl_cb *);
extern void		nl_cb_put(struct nl_cb *);

extern int  nl_cb_set(struct nl_cb *, enum nl_cb_type, enum nl_cb_kind,
		      nl_recvmsg_msg_cb_t, void *);
extern int  nl_cb_set_all(struct nl_cb *, enum nl_cb_kind,
			  nl_recvmsg_msg_cb_t, void *);
extern int  nl_cb_err(struct nl_cb *, enum nl_cb_kind, nl_recvmsg_err_cb_t,
		      void *);

extern void nl_cb_overwrite_recvmsgs(struct nl_cb *,
				     int (*func)(struct nl_sock *,
						 struct nl_cb *));
extern void nl_cb_overwrite_recv(struct nl_cb *,
				 int (*func)(struct nl_sock *,
					     struct sockaddr_nl *,
					     unsigned char **,
					     struct ucred **));
extern void nl_cb_overwrite_send(struct nl_cb *,
				 int (*func)(struct nl_sock *,
					     struct nl_msg *));

#ifdef __cplusplus
}
#endif

#endif
