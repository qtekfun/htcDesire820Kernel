/*
 * netlink/cache-api.h		Caching API
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License as published by the Free Software Foundation version 2.1
 *	of the License.
 *
 * Copyright (c) 2003-2006 Thomas Graf <tgraf@suug.ch>
 */

#ifndef NETLINK_CACHE_API_H_
#define NETLINK_CACHE_API_H_

#include <netlink/netlink.h>

#ifdef __cplusplus
extern "C" {
#endif


enum {
	NL_ACT_UNSPEC,
	NL_ACT_NEW,
	NL_ACT_DEL,
	NL_ACT_GET,
	NL_ACT_SET,
	NL_ACT_CHANGE,
	__NL_ACT_MAX,
};

#define NL_ACT_MAX (__NL_ACT_MAX - 1)

#define END_OF_MSGTYPES_LIST	{ -1, -1, NULL }

struct nl_msgtype
{
	
	int			mt_id;

	
	int			mt_act;

	
	char *			mt_name;
};

struct nl_af_group
{
	
	int			ag_family;

	
	int			ag_group;
};

#define END_OF_GROUP_LIST AF_UNSPEC, 0

struct nl_parser_param
{
	int             (*pp_cb)(struct nl_object *, struct nl_parser_param *);
	void *            pp_arg;
};

struct nl_cache_ops
{
	char  *			co_name;

	int			co_hdrsize;
	int			co_protocol;
	struct nl_af_group *	co_groups;
	
	int   (*co_request_update)(struct nl_cache *, struct nl_sock *);

	int   (*co_msg_parser)(struct nl_cache_ops *, struct sockaddr_nl *,
			       struct nlmsghdr *, struct nl_parser_param *);

	struct nl_object_ops *	co_obj_ops;

	struct nl_cache_ops *co_next;
	struct nl_cache *co_major_cache;
	struct genl_ops *	co_genl;
	struct nl_msgtype	co_msgtypes[];
};


#ifdef __cplusplus
}
#endif

#endif
