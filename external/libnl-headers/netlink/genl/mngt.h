/*
 * netlink/genl/mngt.h		Generic Netlink Management
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License as published by the Free Software Foundation version 2.1
 *	of the License.
 *
 * Copyright (c) 2003-2006 Thomas Graf <tgraf@suug.ch>
 */

#ifndef NETLINK_GENL_MNGT_H_
#define NETLINK_GENL_MNGT_H_

#include <netlink/netlink.h>
#include <netlink/attr.h>
#include <netlink/list.h>

#ifdef __cplusplus
extern "C" {
#endif

struct nl_cache_ops;

struct genl_info
{
	struct sockaddr_nl *    who;
	struct nlmsghdr *       nlh;
	struct genlmsghdr *     genlhdr;
	void *                  userhdr;
	struct nlattr **        attrs;
};

struct genl_cmd
{
	
	int			c_id;

	
	char *			c_name;

	int			c_maxattr;

	int		      (*c_msg_parser)(struct nl_cache_ops *,
					      struct genl_cmd *,
					      struct genl_info *, void *);

	struct nla_policy *	c_attr_policy;
};

struct genl_ops
{
	int			o_family;
	int			o_id;
	char *			o_name;
	struct nl_cache_ops *	o_cache_ops;
	struct genl_cmd	*	o_cmds;
	int			o_ncmds;

	
	struct nl_list_head	o_list;
};


extern int		genl_register(struct nl_cache_ops *);
extern void		genl_unregister(struct nl_cache_ops *);

#ifdef __cplusplus
}
#endif

#endif
