/*
 * netlink/netlink-types.h	Netlink Types
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License as published by the Free Software Foundation version 2.1
 *	of the License.
 *
 * Copyright (c) 2003-2006 Thomas Graf <tgraf@suug.ch>
 */

#ifndef __NETLINK_TYPES_H_
#define __NETLINK_TYPES_H_

#include <stdio.h>

enum nl_dump_type {
	NL_DUMP_LINE,		
	NL_DUMP_DETAILS,	
	NL_DUMP_STATS,		
	NL_DUMP_ENV,		
	__NL_DUMP_MAX,
};
#define NL_DUMP_MAX (__NL_DUMP_MAX - 1)

struct nl_dump_params
{
	enum nl_dump_type	dp_type;

	int			dp_prefix;

	int			dp_print_index;

	int			dp_dump_msgtype;

	void			(*dp_cb)(struct nl_dump_params *, char *);

	void			(*dp_nl_cb)(struct nl_dump_params *, int);

	void			*dp_data;

	FILE *			dp_fd;

	char *			dp_buf;

	size_t			dp_buflen;

	int			dp_pre_dump;

	int			dp_ivar;

	unsigned int		dp_line;
};

#endif
