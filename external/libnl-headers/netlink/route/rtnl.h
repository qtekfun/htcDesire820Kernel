/*
 * netlink/route/rtnl.h		Routing Netlink
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License as published by the Free Software Foundation version 2.1
 *	of the License.
 *
 * Copyright (c) 2003-2008 Thomas Graf <tgraf@suug.ch>
 */

#ifndef NETLINK_RTNL_H_
#define NETLINK_RTNL_H_

#include <netlink/netlink.h>

#ifdef __cplusplus
extern "C" {
#endif


#define RTNL_REALM_MASK (0xFFFF)

#define RTNL_REALM_FROM(realm) ((realm) >> 16)

#define RTNL_REALM_TO(realm) ((realm) & RTNL_REALM_MASK)

#define RTNL_MAKE_REALM(from, to) \
	((RTNL_REALM_TO(from) << 16) & RTNL_REALM_TO(to))



extern int		nl_rtgen_request(struct nl_sock *, int, int, int);

extern char *		nl_rtntype2str(int, char *, size_t);
extern int		nl_str2rtntype(const char *);

extern char *		rtnl_scope2str(int, char *, size_t);
extern int		rtnl_str2scope(const char *);

extern char *		rtnl_realms2str(uint32_t, char *, size_t);

#ifdef __cplusplus
}
#endif

#endif
