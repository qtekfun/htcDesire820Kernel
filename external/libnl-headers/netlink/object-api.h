/*
 * netlink/object-api.c		Object API
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License as published by the Free Software Foundation version 2.1
 *	of the License.
 *
 * Copyright (c) 2003-2007 Thomas Graf <tgraf@suug.ch>
 */

#ifndef NETLINK_OBJECT_API_H_
#define NETLINK_OBJECT_API_H_

#include <netlink/netlink.h>
#include <netlink/utils.h>

#ifdef __cplusplus
extern "C" {
#endif


#define NLHDR_COMMON				\
	int			ce_refcnt;	\
	struct nl_object_ops *	ce_ops;		\
	struct nl_cache *	ce_cache;	\
	struct nl_list_head	ce_list;	\
	int			ce_msgtype;	\
	int			ce_flags;	\
	uint32_t		ce_mask;

#define AVAILABLE(A, B, ATTR)		(((A)->ce_mask & (B)->ce_mask) & (ATTR))

#define AVAILABLE_MISMATCH(A, B, ATTR)	(((A)->ce_mask ^ (B)->ce_mask) & (ATTR))

#define ATTR_MISMATCH(A, B, ATTR, EXPR)	(AVAILABLE_MISMATCH(A, B, ATTR) || \
					 (AVAILABLE(A, B, ATTR) && (EXPR)))

#define ATTR_DIFF(LIST, ATTR, A, B, EXPR) \
({	int diff = 0; \
	if (((LIST) & (ATTR)) && ATTR_MISMATCH(A, B, ATTR, EXPR)) \
		diff = ATTR; \
	diff; })

struct nl_object_ops
{
	char *		oo_name;

	
	size_t		oo_size;

	
	uint32_t	oo_id_attrs;

	void  (*oo_constructor)(struct nl_object *);

	void  (*oo_free_data)(struct nl_object *);

	int  (*oo_clone)(struct nl_object *, struct nl_object *);

	void (*oo_dump[NL_DUMP_MAX+1])(struct nl_object *,
				       struct nl_dump_params *);

	int   (*oo_compare)(struct nl_object *, struct nl_object *,
			    uint32_t, int);


	char *(*oo_attrs2str)(int, char *, size_t);
};


#ifdef __cplusplus
}
#endif

#endif
