/******************************************************************************
 *
 * Copyright (C) 2008 Jason Evans <jasone@FreeBSD.org>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice(s), this list of conditions and the following disclaimer
 *    unmodified other than the allowable addition of one or more
 *    copyright notices.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice(s), this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER(S) BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 *
 * cpp macro implementation of left-leaning red-black trees.
 *
 * Usage:
 *
 *   (Optional.)
 *   #define SIZEOF_PTR ...
 *   #define SIZEOF_PTR_2POW ...
 *   #define RB_NO_C99_VARARRAYS
 *
 *   (Optional, see assert(3).)
 *   #define NDEBUG
 *
 *   (Required.)
 *   #include <assert.h>
 *   #include <rb.h>
 *   ...
 *
 * All operations are done non-recursively.  Parent pointers are not used, and
 * color bits are stored in the least significant bit of right-child pointers,
 * thus making node linkage as compact as is possible for red-black trees.
 *
 * Some macros use a comparison function pointer, which is expected to have the
 * following prototype:
 *
 *   int (a_cmp *)(a_type *a_node, a_type *a_other);
 *                         ^^^^^^
 *                      or a_key
 *
 * Interpretation of comparision function return values:
 *
 *   -1 : a_node <  a_other
 *    0 : a_node == a_other
 *    1 : a_node >  a_other
 *
 * In all cases, the a_node or a_key macro argument is the first argument to the
 * comparison function, which makes it possible to write comparison functions
 * that treat the first argument specially.
 *
 ******************************************************************************/

#ifndef RB_H_
#define	RB_H_

#if 0
#include <sys/cdefs.h>
__FBSDID("$FreeBSD: head/lib/libc/stdlib/rb.h 178995 2008-05-14 18:33:13Z jasone $");
#endif

#define	rb_node(a_type)							\
struct {								\
    a_type *rbn_left;							\
    a_type *rbn_right_red;						\
}

#define	rb_tree(a_type)							\
struct {								\
    a_type *rbt_root;							\
    a_type rbt_nil;							\
}

#define	rbp_left_get(a_type, a_field, a_node)				\
    ((a_node)->a_field.rbn_left)
#define	rbp_left_set(a_type, a_field, a_node, a_left) do {		\
    (a_node)->a_field.rbn_left = a_left;				\
} while (0)

#define	rbp_right_get(a_type, a_field, a_node)				\
    ((a_type *) (((intptr_t) (a_node)->a_field.rbn_right_red)		\
      & ((ssize_t)-2)))
#define	rbp_right_set(a_type, a_field, a_node, a_right) do {		\
    (a_node)->a_field.rbn_right_red = (a_type *) (((uintptr_t) a_right)	\
      | (((uintptr_t) (a_node)->a_field.rbn_right_red) & ((size_t)1)));	\
} while (0)

#define	rbp_red_get(a_type, a_field, a_node)				\
    ((bool) (((uintptr_t) (a_node)->a_field.rbn_right_red)		\
      & ((size_t)1)))
#define	rbp_color_set(a_type, a_field, a_node, a_red) do {		\
    (a_node)->a_field.rbn_right_red = (a_type *) ((((intptr_t)		\
      (a_node)->a_field.rbn_right_red) & ((ssize_t)-2))			\
      | ((ssize_t)a_red));						\
} while (0)
#define	rbp_red_set(a_type, a_field, a_node) do {			\
    (a_node)->a_field.rbn_right_red = (a_type *) (((uintptr_t)		\
      (a_node)->a_field.rbn_right_red) | ((size_t)1));			\
} while (0)
#define	rbp_black_set(a_type, a_field, a_node) do {			\
    (a_node)->a_field.rbn_right_red = (a_type *) (((intptr_t)		\
      (a_node)->a_field.rbn_right_red) & ((ssize_t)-2));		\
} while (0)

#define	rbp_node_new(a_type, a_field, a_tree, a_node) do {		\
    rbp_left_set(a_type, a_field, (a_node), &(a_tree)->rbt_nil);	\
    rbp_right_set(a_type, a_field, (a_node), &(a_tree)->rbt_nil);	\
    rbp_red_set(a_type, a_field, (a_node));				\
} while (0)

#define	rb_new(a_type, a_field, a_tree) do {				\
    (a_tree)->rbt_root = &(a_tree)->rbt_nil;				\
    rbp_node_new(a_type, a_field, a_tree, &(a_tree)->rbt_nil);		\
    rbp_black_set(a_type, a_field, &(a_tree)->rbt_nil);			\
} while (0)

#define	rbp_black_height(a_type, a_field, a_tree, r_height) do {	\
    a_type *rbp_bh_t;							\
    for (rbp_bh_t = (a_tree)->rbt_root, (r_height) = 0;			\
      rbp_bh_t != &(a_tree)->rbt_nil;					\
      rbp_bh_t = rbp_left_get(a_type, a_field, rbp_bh_t)) {		\
	if (rbp_red_get(a_type, a_field, rbp_bh_t) == false) {		\
	    (r_height)++;						\
	}								\
    }									\
} while (0)

#define	rbp_first(a_type, a_field, a_tree, a_root, r_node) do {		\
    for ((r_node) = (a_root);						\
      rbp_left_get(a_type, a_field, (r_node)) != &(a_tree)->rbt_nil;	\
      (r_node) = rbp_left_get(a_type, a_field, (r_node))) {		\
    }									\
} while (0)

#define	rbp_last(a_type, a_field, a_tree, a_root, r_node) do {		\
    for ((r_node) = (a_root);						\
      rbp_right_get(a_type, a_field, (r_node)) != &(a_tree)->rbt_nil;	\
      (r_node) = rbp_right_get(a_type, a_field, (r_node))) {		\
    }									\
} while (0)

#define	rbp_next(a_type, a_field, a_cmp, a_tree, a_node, r_node) do {	\
    if (rbp_right_get(a_type, a_field, (a_node))			\
      != &(a_tree)->rbt_nil) {						\
	rbp_first(a_type, a_field, a_tree, rbp_right_get(a_type,	\
	  a_field, (a_node)), (r_node));				\
    } else {								\
	a_type *rbp_n_t = (a_tree)->rbt_root;				\
	assert(rbp_n_t != &(a_tree)->rbt_nil);				\
	(r_node) = &(a_tree)->rbt_nil;					\
	while (true) {							\
	    int rbp_n_cmp = (a_cmp)((a_node), rbp_n_t);			\
	    if (rbp_n_cmp < 0) {					\
		(r_node) = rbp_n_t;					\
		rbp_n_t = rbp_left_get(a_type, a_field, rbp_n_t);	\
	    } else if (rbp_n_cmp > 0) {					\
		rbp_n_t = rbp_right_get(a_type, a_field, rbp_n_t);	\
	    } else {							\
		break;							\
	    }								\
	    assert(rbp_n_t != &(a_tree)->rbt_nil);			\
	}								\
    }									\
} while (0)

#define	rbp_prev(a_type, a_field, a_cmp, a_tree, a_node, r_node) do {	\
    if (rbp_left_get(a_type, a_field, (a_node)) != &(a_tree)->rbt_nil) {\
	rbp_last(a_type, a_field, a_tree, rbp_left_get(a_type,		\
	  a_field, (a_node)), (r_node));				\
    } else {								\
	a_type *rbp_p_t = (a_tree)->rbt_root;				\
	assert(rbp_p_t != &(a_tree)->rbt_nil);				\
	(r_node) = &(a_tree)->rbt_nil;					\
	while (true) {							\
	    int rbp_p_cmp = (a_cmp)((a_node), rbp_p_t);			\
	    if (rbp_p_cmp < 0) {					\
		rbp_p_t = rbp_left_get(a_type, a_field, rbp_p_t);	\
	    } else if (rbp_p_cmp > 0) {					\
		(r_node) = rbp_p_t;					\
		rbp_p_t = rbp_right_get(a_type, a_field, rbp_p_t);	\
	    } else {							\
		break;							\
	    }								\
	    assert(rbp_p_t != &(a_tree)->rbt_nil);			\
	}								\
    }									\
} while (0)

#define	rb_first(a_type, a_field, a_tree, r_node) do {			\
    rbp_first(a_type, a_field, a_tree, (a_tree)->rbt_root, (r_node));	\
    if ((r_node) == &(a_tree)->rbt_nil) {				\
	(r_node) = NULL;						\
    }									\
} while (0)

#define	rb_last(a_type, a_field, a_tree, r_node) do {			\
    rbp_last(a_type, a_field, a_tree, (a_tree)->rbt_root, r_node);	\
    if ((r_node) == &(a_tree)->rbt_nil) {				\
	(r_node) = NULL;						\
    }									\
} while (0)

#define	rb_next(a_type, a_field, a_cmp, a_tree, a_node, r_node) do {	\
    rbp_next(a_type, a_field, a_cmp, a_tree, (a_node), (r_node));	\
    if ((r_node) == &(a_tree)->rbt_nil) {				\
	(r_node) = NULL;						\
    }									\
} while (0)

#define	rb_prev(a_type, a_field, a_cmp, a_tree, a_node, r_node) do {	\
    rbp_prev(a_type, a_field, a_cmp, a_tree, (a_node), (r_node));	\
    if ((r_node) == &(a_tree)->rbt_nil) {				\
	(r_node) = NULL;						\
    }									\
} while (0)

#define	rb_search(a_type, a_field, a_cmp, a_tree, a_key, r_node) do {	\
    int rbp_se_cmp;							\
    (r_node) = (a_tree)->rbt_root;					\
    while ((r_node) != &(a_tree)->rbt_nil				\
      && (rbp_se_cmp = (a_cmp)((a_key), (r_node))) != 0) {		\
	if (rbp_se_cmp < 0) {						\
	    (r_node) = rbp_left_get(a_type, a_field, (r_node));		\
	} else {							\
	    (r_node) = rbp_right_get(a_type, a_field, (r_node));	\
	}								\
    }									\
    if ((r_node) == &(a_tree)->rbt_nil) {				\
	(r_node) = NULL;						\
    }									\
} while (0)

#define	rb_nsearch(a_type, a_field, a_cmp, a_tree, a_key, r_node) do {	\
    a_type *rbp_ns_t = (a_tree)->rbt_root;				\
    (r_node) = NULL;							\
    while (rbp_ns_t != &(a_tree)->rbt_nil) {				\
	int rbp_ns_cmp = (a_cmp)((a_key), rbp_ns_t);			\
	if (rbp_ns_cmp < 0) {						\
	    (r_node) = rbp_ns_t;					\
	    rbp_ns_t = rbp_left_get(a_type, a_field, rbp_ns_t);		\
	} else if (rbp_ns_cmp > 0) {					\
	    rbp_ns_t = rbp_right_get(a_type, a_field, rbp_ns_t);	\
	} else {							\
	    (r_node) = rbp_ns_t;					\
	    break;							\
	}								\
    }									\
} while (0)

#define	rb_psearch(a_type, a_field, a_cmp, a_tree, a_key, r_node) do {	\
    a_type *rbp_ps_t = (a_tree)->rbt_root;				\
    (r_node) = NULL;							\
    while (rbp_ps_t != &(a_tree)->rbt_nil) {				\
	int rbp_ps_cmp = (a_cmp)((a_key), rbp_ps_t);			\
	if (rbp_ps_cmp < 0) {						\
	    rbp_ps_t = rbp_left_get(a_type, a_field, rbp_ps_t);		\
	} else if (rbp_ps_cmp > 0) {					\
	    (r_node) = rbp_ps_t;					\
	    rbp_ps_t = rbp_right_get(a_type, a_field, rbp_ps_t);	\
	} else {							\
	    (r_node) = rbp_ps_t;					\
	    break;							\
	}								\
    }									\
} while (0)

#define	rbp_rotate_left(a_type, a_field, a_node, r_node) do {		\
    (r_node) = rbp_right_get(a_type, a_field, (a_node));		\
    rbp_right_set(a_type, a_field, (a_node),				\
      rbp_left_get(a_type, a_field, (r_node)));				\
    rbp_left_set(a_type, a_field, (r_node), (a_node));			\
} while (0)

#define	rbp_rotate_right(a_type, a_field, a_node, r_node) do {		\
    (r_node) = rbp_left_get(a_type, a_field, (a_node));			\
    rbp_left_set(a_type, a_field, (a_node),				\
      rbp_right_get(a_type, a_field, (r_node)));			\
    rbp_right_set(a_type, a_field, (r_node), (a_node));			\
} while (0)

#define	rbp_lean_left(a_type, a_field, a_node, r_node) do {		\
    bool rbp_ll_red;							\
    rbp_rotate_left(a_type, a_field, (a_node), (r_node));		\
    rbp_ll_red = rbp_red_get(a_type, a_field, (a_node));		\
    rbp_color_set(a_type, a_field, (r_node), rbp_ll_red);		\
    rbp_red_set(a_type, a_field, (a_node));				\
} while (0)

#define	rbp_lean_right(a_type, a_field, a_node, r_node) do {		\
    bool rbp_lr_red;							\
    rbp_rotate_right(a_type, a_field, (a_node), (r_node));		\
    rbp_lr_red = rbp_red_get(a_type, a_field, (a_node));		\
    rbp_color_set(a_type, a_field, (r_node), rbp_lr_red);		\
    rbp_red_set(a_type, a_field, (a_node));				\
} while (0)

#define	rbp_move_red_left(a_type, a_field, a_node, r_node) do {		\
    a_type *rbp_mrl_t, *rbp_mrl_u;					\
    rbp_mrl_t = rbp_left_get(a_type, a_field, (a_node));		\
    rbp_red_set(a_type, a_field, rbp_mrl_t);				\
    rbp_mrl_t = rbp_right_get(a_type, a_field, (a_node));		\
    rbp_mrl_u = rbp_left_get(a_type, a_field, rbp_mrl_t);		\
    if (rbp_red_get(a_type, a_field, rbp_mrl_u)) {			\
	rbp_rotate_right(a_type, a_field, rbp_mrl_t, rbp_mrl_u);	\
	rbp_right_set(a_type, a_field, (a_node), rbp_mrl_u);		\
	rbp_rotate_left(a_type, a_field, (a_node), (r_node));		\
	rbp_mrl_t = rbp_right_get(a_type, a_field, (a_node));		\
	if (rbp_red_get(a_type, a_field, rbp_mrl_t)) {			\
	    rbp_black_set(a_type, a_field, rbp_mrl_t);			\
	    rbp_red_set(a_type, a_field, (a_node));			\
	    rbp_rotate_left(a_type, a_field, (a_node), rbp_mrl_t);	\
	    rbp_left_set(a_type, a_field, (r_node), rbp_mrl_t);		\
	} else {							\
	    rbp_black_set(a_type, a_field, (a_node));			\
	}								\
    } else {								\
	rbp_red_set(a_type, a_field, (a_node));				\
	rbp_rotate_left(a_type, a_field, (a_node), (r_node));		\
    }									\
} while (0)

#define	rbp_move_red_right(a_type, a_field, a_node, r_node) do {	\
    a_type *rbp_mrr_t;							\
    rbp_mrr_t = rbp_left_get(a_type, a_field, (a_node));		\
    if (rbp_red_get(a_type, a_field, rbp_mrr_t)) {			\
	a_type *rbp_mrr_u, *rbp_mrr_v;					\
	rbp_mrr_u = rbp_right_get(a_type, a_field, rbp_mrr_t);		\
	rbp_mrr_v = rbp_left_get(a_type, a_field, rbp_mrr_u);		\
	if (rbp_red_get(a_type, a_field, rbp_mrr_v)) {			\
	    rbp_color_set(a_type, a_field, rbp_mrr_u,			\
	      rbp_red_get(a_type, a_field, (a_node)));			\
	    rbp_black_set(a_type, a_field, rbp_mrr_v);			\
	    rbp_rotate_left(a_type, a_field, rbp_mrr_t, rbp_mrr_u);	\
	    rbp_left_set(a_type, a_field, (a_node), rbp_mrr_u);		\
	    rbp_rotate_right(a_type, a_field, (a_node), (r_node));	\
	    rbp_rotate_left(a_type, a_field, (a_node), rbp_mrr_t);	\
	    rbp_right_set(a_type, a_field, (r_node), rbp_mrr_t);	\
	} else {							\
	    rbp_color_set(a_type, a_field, rbp_mrr_t,			\
	      rbp_red_get(a_type, a_field, (a_node)));			\
	    rbp_red_set(a_type, a_field, rbp_mrr_u);			\
	    rbp_rotate_right(a_type, a_field, (a_node), (r_node));	\
	    rbp_rotate_left(a_type, a_field, (a_node), rbp_mrr_t);	\
	    rbp_right_set(a_type, a_field, (r_node), rbp_mrr_t);	\
	}								\
	rbp_red_set(a_type, a_field, (a_node));				\
    } else {								\
	rbp_red_set(a_type, a_field, rbp_mrr_t);			\
	rbp_mrr_t = rbp_left_get(a_type, a_field, rbp_mrr_t);		\
	if (rbp_red_get(a_type, a_field, rbp_mrr_t)) {			\
	    rbp_black_set(a_type, a_field, rbp_mrr_t);			\
	    rbp_rotate_right(a_type, a_field, (a_node), (r_node));	\
	    rbp_rotate_left(a_type, a_field, (a_node), rbp_mrr_t);	\
	    rbp_right_set(a_type, a_field, (r_node), rbp_mrr_t);	\
	} else {							\
	    rbp_rotate_left(a_type, a_field, (a_node), (r_node));	\
	}								\
    }									\
} while (0)

#define	rb_insert(a_type, a_field, a_cmp, a_tree, a_node) do {		\
    a_type rbp_i_s;							\
    a_type *rbp_i_g, *rbp_i_p, *rbp_i_c, *rbp_i_t, *rbp_i_u;		\
    int rbp_i_cmp = 0;							\
    rbp_i_g = &(a_tree)->rbt_nil;					\
    rbp_left_set(a_type, a_field, &rbp_i_s, (a_tree)->rbt_root);	\
    rbp_right_set(a_type, a_field, &rbp_i_s, &(a_tree)->rbt_nil);	\
    rbp_black_set(a_type, a_field, &rbp_i_s);				\
    rbp_i_p = &rbp_i_s;							\
    rbp_i_c = (a_tree)->rbt_root;					\
    \
    \
    \
    \
    while (rbp_i_c != &(a_tree)->rbt_nil) {				\
	rbp_i_t = rbp_left_get(a_type, a_field, rbp_i_c);		\
	rbp_i_u = rbp_left_get(a_type, a_field, rbp_i_t);		\
	if (rbp_red_get(a_type, a_field, rbp_i_t)			\
	  && rbp_red_get(a_type, a_field, rbp_i_u)) {			\
	    \
	    \
	    \
	    \
	    \
	    rbp_rotate_right(a_type, a_field, rbp_i_c, rbp_i_t);	\
	    \
	    rbp_i_u = rbp_left_get(a_type, a_field, rbp_i_t);		\
	    rbp_black_set(a_type, a_field, rbp_i_u);			\
	    if (rbp_left_get(a_type, a_field, rbp_i_p) == rbp_i_c) {	\
		rbp_left_set(a_type, a_field, rbp_i_p, rbp_i_t);	\
		rbp_i_c = rbp_i_t;					\
	    } else {							\
		\
		\
		\
		assert(rbp_right_get(a_type, a_field, rbp_i_p)		\
		  == rbp_i_c);						\
		rbp_right_set(a_type, a_field, rbp_i_p, rbp_i_t);	\
		rbp_lean_left(a_type, a_field, rbp_i_p, rbp_i_u);	\
		if (rbp_left_get(a_type, a_field, rbp_i_g) == rbp_i_p) {\
		    rbp_left_set(a_type, a_field, rbp_i_g, rbp_i_u);	\
		} else {						\
		    assert(rbp_right_get(a_type, a_field, rbp_i_g)	\
		      == rbp_i_p);					\
		    rbp_right_set(a_type, a_field, rbp_i_g, rbp_i_u);	\
		}							\
		rbp_i_p = rbp_i_u;					\
		rbp_i_cmp = (a_cmp)((a_node), rbp_i_p);			\
		if (rbp_i_cmp < 0) {					\
		    rbp_i_c = rbp_left_get(a_type, a_field, rbp_i_p);	\
		} else {						\
		    assert(rbp_i_cmp > 0);				\
		    rbp_i_c = rbp_right_get(a_type, a_field, rbp_i_p);	\
		}							\
		continue;						\
	    }								\
	}								\
	rbp_i_g = rbp_i_p;						\
	rbp_i_p = rbp_i_c;						\
	rbp_i_cmp = (a_cmp)((a_node), rbp_i_c);				\
	if (rbp_i_cmp < 0) {						\
	    rbp_i_c = rbp_left_get(a_type, a_field, rbp_i_c);		\
	} else {							\
	    assert(rbp_i_cmp > 0);					\
	    rbp_i_c = rbp_right_get(a_type, a_field, rbp_i_c);		\
	}								\
    }									\
    \
    rbp_node_new(a_type, a_field, a_tree, (a_node));			\
    if (rbp_i_cmp > 0) {						\
	rbp_right_set(a_type, a_field, rbp_i_p, (a_node));		\
	rbp_lean_left(a_type, a_field, rbp_i_p, rbp_i_t);		\
	if (rbp_left_get(a_type, a_field, rbp_i_g) == rbp_i_p) {	\
	    rbp_left_set(a_type, a_field, rbp_i_g, rbp_i_t);		\
	} else if (rbp_right_get(a_type, a_field, rbp_i_g) == rbp_i_p) {\
	    rbp_right_set(a_type, a_field, rbp_i_g, rbp_i_t);		\
	}								\
    } else {								\
	rbp_left_set(a_type, a_field, rbp_i_p, (a_node));		\
    }									\
    \
    (a_tree)->rbt_root = rbp_left_get(a_type, a_field, &rbp_i_s);	\
    rbp_black_set(a_type, a_field, (a_tree)->rbt_root);			\
} while (0)

#define	rb_remove(a_type, a_field, a_cmp, a_tree, a_node) do {		\
    a_type rbp_r_s;							\
    a_type *rbp_r_p, *rbp_r_c, *rbp_r_xp, *rbp_r_t, *rbp_r_u;		\
    int rbp_r_cmp;							\
    rbp_left_set(a_type, a_field, &rbp_r_s, (a_tree)->rbt_root);	\
    rbp_right_set(a_type, a_field, &rbp_r_s, &(a_tree)->rbt_nil);	\
    rbp_black_set(a_type, a_field, &rbp_r_s);				\
    rbp_r_p = &rbp_r_s;							\
    rbp_r_c = (a_tree)->rbt_root;					\
    rbp_r_xp = &(a_tree)->rbt_nil;					\
    \
    \
    \
    \
    \
    rbp_r_cmp = (a_cmp)((a_node), rbp_r_c);				\
    if (rbp_r_cmp < 0) {						\
	rbp_r_t = rbp_left_get(a_type, a_field, rbp_r_c);		\
	rbp_r_u = rbp_left_get(a_type, a_field, rbp_r_t);		\
	if (rbp_red_get(a_type, a_field, rbp_r_t) == false		\
	  && rbp_red_get(a_type, a_field, rbp_r_u) == false) {		\
	    \
	    rbp_move_red_left(a_type, a_field, rbp_r_c, rbp_r_t);	\
	    rbp_black_set(a_type, a_field, rbp_r_t);			\
	    rbp_left_set(a_type, a_field, rbp_r_p, rbp_r_t);		\
	    rbp_r_c = rbp_r_t;						\
	} else {							\
	    \
	    rbp_r_p = rbp_r_c;						\
	    rbp_r_c = rbp_left_get(a_type, a_field, rbp_r_c);		\
	}								\
    } else {								\
	if (rbp_r_cmp == 0) {						\
	    assert((a_node) == rbp_r_c);				\
	    if (rbp_right_get(a_type, a_field, rbp_r_c)			\
	      == &(a_tree)->rbt_nil) {					\
		\
		if (rbp_left_get(a_type, a_field, rbp_r_c)		\
		  != &(a_tree)->rbt_nil) {				\
		    rbp_lean_right(a_type, a_field, rbp_r_c, rbp_r_t);	\
		    rbp_right_set(a_type, a_field, rbp_r_t,		\
		      &(a_tree)->rbt_nil);				\
		} else {						\
		    rbp_r_t = &(a_tree)->rbt_nil;			\
		}							\
		rbp_left_set(a_type, a_field, rbp_r_p, rbp_r_t);	\
	    } else {							\
		\
		\
		\
		\
		rbp_r_xp = rbp_r_p;					\
		rbp_r_cmp = 1; \
	    }								\
	}								\
	if (rbp_r_cmp == 1) {						\
	    if (rbp_red_get(a_type, a_field, rbp_left_get(a_type,	\
	      a_field, rbp_right_get(a_type, a_field, rbp_r_c)))	\
	      == false) {						\
		rbp_r_t = rbp_left_get(a_type, a_field, rbp_r_c);	\
		if (rbp_red_get(a_type, a_field, rbp_r_t)) {		\
		    \
		    rbp_move_red_right(a_type, a_field, rbp_r_c,	\
		      rbp_r_t);						\
		} else {						\
		    \
		    rbp_red_set(a_type, a_field, rbp_r_c);		\
		    rbp_r_u = rbp_left_get(a_type, a_field, rbp_r_t);	\
		    if (rbp_red_get(a_type, a_field, rbp_r_u)) {	\
			rbp_black_set(a_type, a_field, rbp_r_u);	\
			rbp_rotate_right(a_type, a_field, rbp_r_c,	\
			  rbp_r_t);					\
			rbp_rotate_left(a_type, a_field, rbp_r_c,	\
			  rbp_r_u);					\
			rbp_right_set(a_type, a_field, rbp_r_t,		\
			  rbp_r_u);					\
		    } else {						\
			rbp_red_set(a_type, a_field, rbp_r_t);		\
			rbp_rotate_left(a_type, a_field, rbp_r_c,	\
			  rbp_r_t);					\
		    }							\
		}							\
		rbp_left_set(a_type, a_field, rbp_r_p, rbp_r_t);	\
		rbp_r_c = rbp_r_t;					\
	    } else {							\
		\
		rbp_r_p = rbp_r_c;					\
		rbp_r_c = rbp_right_get(a_type, a_field, rbp_r_c);	\
	    }								\
	}								\
    }									\
    if (rbp_r_cmp != 0) {						\
	while (true) {							\
	    assert(rbp_r_p != &(a_tree)->rbt_nil);			\
	    rbp_r_cmp = (a_cmp)((a_node), rbp_r_c);			\
	    if (rbp_r_cmp < 0) {					\
		rbp_r_t = rbp_left_get(a_type, a_field, rbp_r_c);	\
		if (rbp_r_t == &(a_tree)->rbt_nil) {			\
		    \
		    \
		    \
		    if (rbp_left_get(a_type, a_field, rbp_r_xp)		\
		      == (a_node)) {					\
			rbp_left_set(a_type, a_field, rbp_r_xp,		\
			  rbp_r_c);					\
		    } else {						\
			assert(rbp_right_get(a_type, a_field,		\
			  rbp_r_xp) == (a_node));			\
			rbp_right_set(a_type, a_field, rbp_r_xp,	\
			  rbp_r_c);					\
		    }							\
		    rbp_left_set(a_type, a_field, rbp_r_c,		\
		      rbp_left_get(a_type, a_field, (a_node)));		\
		    rbp_right_set(a_type, a_field, rbp_r_c,		\
		      rbp_right_get(a_type, a_field, (a_node)));	\
		    rbp_color_set(a_type, a_field, rbp_r_c,		\
		      rbp_red_get(a_type, a_field, (a_node)));		\
		    if (rbp_left_get(a_type, a_field, rbp_r_p)		\
		      == rbp_r_c) {					\
			rbp_left_set(a_type, a_field, rbp_r_p,		\
			  &(a_tree)->rbt_nil);				\
		    } else {						\
			assert(rbp_right_get(a_type, a_field, rbp_r_p)	\
			  == rbp_r_c);					\
			rbp_right_set(a_type, a_field, rbp_r_p,		\
			  &(a_tree)->rbt_nil);				\
		    }							\
		    break;						\
		}							\
		rbp_r_u = rbp_left_get(a_type, a_field, rbp_r_t);	\
		if (rbp_red_get(a_type, a_field, rbp_r_t) == false	\
		  && rbp_red_get(a_type, a_field, rbp_r_u) == false) {	\
		    rbp_move_red_left(a_type, a_field, rbp_r_c,		\
		      rbp_r_t);						\
		    if (rbp_left_get(a_type, a_field, rbp_r_p)		\
		      == rbp_r_c) {					\
			rbp_left_set(a_type, a_field, rbp_r_p, rbp_r_t);\
		    } else {						\
			rbp_right_set(a_type, a_field, rbp_r_p,		\
			  rbp_r_t);					\
		    }							\
		    rbp_r_c = rbp_r_t;					\
		} else {						\
		    rbp_r_p = rbp_r_c;					\
		    rbp_r_c = rbp_left_get(a_type, a_field, rbp_r_c);	\
		}							\
	    } else {							\
		\
		\
		if (rbp_r_cmp == 0) {					\
		    assert((a_node) == rbp_r_c);			\
		    if (rbp_right_get(a_type, a_field, rbp_r_c)		\
		      == &(a_tree)->rbt_nil) {				\
			\
			if (rbp_left_get(a_type, a_field, rbp_r_c)	\
			  != &(a_tree)->rbt_nil) {			\
			    rbp_lean_right(a_type, a_field, rbp_r_c,	\
			      rbp_r_t);					\
			    rbp_right_set(a_type, a_field, rbp_r_t,	\
			      &(a_tree)->rbt_nil);			\
			} else {					\
			    rbp_r_t = &(a_tree)->rbt_nil;		\
			}						\
			if (rbp_left_get(a_type, a_field, rbp_r_p)	\
			  == rbp_r_c) {					\
			    rbp_left_set(a_type, a_field, rbp_r_p,	\
			      rbp_r_t);					\
			} else {					\
			    rbp_right_set(a_type, a_field, rbp_r_p,	\
			      rbp_r_t);					\
			}						\
			break;						\
		    } else {						\
			\
			\
			\
			\
			\
			rbp_r_xp = rbp_r_p;				\
		    }							\
		}							\
		rbp_r_t = rbp_right_get(a_type, a_field, rbp_r_c);	\
		rbp_r_u = rbp_left_get(a_type, a_field, rbp_r_t);	\
		if (rbp_red_get(a_type, a_field, rbp_r_u) == false) {	\
		    rbp_move_red_right(a_type, a_field, rbp_r_c,	\
		      rbp_r_t);						\
		    if (rbp_left_get(a_type, a_field, rbp_r_p)		\
		      == rbp_r_c) {					\
			rbp_left_set(a_type, a_field, rbp_r_p, rbp_r_t);\
		    } else {						\
			rbp_right_set(a_type, a_field, rbp_r_p,		\
			  rbp_r_t);					\
		    }							\
		    rbp_r_c = rbp_r_t;					\
		} else {						\
		    rbp_r_p = rbp_r_c;					\
		    rbp_r_c = rbp_right_get(a_type, a_field, rbp_r_c);	\
		}							\
	    }								\
	}								\
    }									\
    \
    (a_tree)->rbt_root = rbp_left_get(a_type, a_field, &rbp_r_s);	\
} while (0)


#define	rb_wrap(a_attr, a_prefix, a_tree_type, a_type, a_field, a_cmp)	\
a_attr void								\
a_prefix##new(a_tree_type *tree) {					\
    rb_new(a_type, a_field, tree);					\
}									\
a_attr a_type *								\
a_prefix##first(a_tree_type *tree) {					\
    a_type *ret;							\
    rb_first(a_type, a_field, tree, ret);				\
    return (ret);							\
}									\
a_attr a_type *								\
a_prefix##last(a_tree_type *tree) {					\
    a_type *ret;							\
    rb_last(a_type, a_field, tree, ret);				\
    return (ret);							\
}									\
a_attr a_type *								\
a_prefix##next(a_tree_type *tree, a_type *node) {			\
    a_type *ret;							\
    rb_next(a_type, a_field, a_cmp, tree, node, ret);			\
    return (ret);							\
}									\
a_attr a_type *								\
a_prefix##prev(a_tree_type *tree, a_type *node) {			\
    a_type *ret;							\
    rb_prev(a_type, a_field, a_cmp, tree, node, ret);			\
    return (ret);							\
}									\
a_attr a_type *								\
a_prefix##search(a_tree_type *tree, a_type *key) {			\
    a_type *ret;							\
    rb_search(a_type, a_field, a_cmp, tree, key, ret);			\
    return (ret);							\
}									\
a_attr a_type *								\
a_prefix##nsearch(a_tree_type *tree, a_type *key) {			\
    a_type *ret;							\
    rb_nsearch(a_type, a_field, a_cmp, tree, key, ret);			\
    return (ret);							\
}									\
a_attr a_type *								\
a_prefix##psearch(a_tree_type *tree, a_type *key) {			\
    a_type *ret;							\
    rb_psearch(a_type, a_field, a_cmp, tree, key, ret);			\
    return (ret);							\
}									\
a_attr void								\
a_prefix##insert(a_tree_type *tree, a_type *node) {			\
    rb_insert(a_type, a_field, a_cmp, tree, node);			\
}									\
a_attr void								\
a_prefix##remove(a_tree_type *tree, a_type *node) {			\
    rb_remove(a_type, a_field, a_cmp, tree, node);			\
}


#ifdef RB_NO_C99_VARARRAYS
#  define rbp_compute_f_height(a_type, a_field, a_tree)
#  define rbp_f_height	(3 * ((SIZEOF_PTR<<3) - (SIZEOF_PTR_2POW+1)))
#  define rbp_compute_fr_height(a_type, a_field, a_tree)
#  define rbp_fr_height	(3 * ((SIZEOF_PTR<<3) - (SIZEOF_PTR_2POW+1)))
#else
#  define rbp_compute_f_height(a_type, a_field, a_tree)			\
    \
    unsigned rbp_f_height;						\
    rbp_black_height(a_type, a_field, a_tree, rbp_f_height);		\
    rbp_f_height *= 3;
#  define rbp_compute_fr_height(a_type, a_field, a_tree)		\
    \
    unsigned rbp_fr_height;						\
    rbp_black_height(a_type, a_field, a_tree, rbp_fr_height);		\
    rbp_fr_height *= 3;
#endif

#define	rb_foreach_begin(a_type, a_field, a_tree, a_var) {		\
    rbp_compute_f_height(a_type, a_field, a_tree)			\
    {									\
	\
	a_type *rbp_f_path[rbp_f_height];				\
	a_type *rbp_f_node;						\
	bool rbp_f_synced = false;					\
	unsigned rbp_f_depth = 0;					\
	if ((a_tree)->rbt_root != &(a_tree)->rbt_nil) {			\
	    rbp_f_path[rbp_f_depth] = (a_tree)->rbt_root;		\
	    rbp_f_depth++;						\
	    while ((rbp_f_node = rbp_left_get(a_type, a_field,		\
	      rbp_f_path[rbp_f_depth-1])) != &(a_tree)->rbt_nil) {	\
		rbp_f_path[rbp_f_depth] = rbp_f_node;			\
		rbp_f_depth++;						\
	    }								\
	}								\
	\
	while (rbp_f_depth > 0) {					\
	    (a_var) = rbp_f_path[rbp_f_depth-1];

#define	rb_foreach_next(a_type, a_field, a_cmp, a_tree, a_node)		\
	    \
	    rbp_f_depth = 0;						\
	    if (a_node != NULL) {					\
		if ((a_tree)->rbt_root != &(a_tree)->rbt_nil) {		\
		    rbp_f_path[rbp_f_depth] = (a_tree)->rbt_root;	\
		    rbp_f_depth++;					\
		    rbp_f_node = rbp_f_path[0];				\
		    while (true) {					\
			int rbp_f_cmp = (a_cmp)((a_node),		\
			  rbp_f_path[rbp_f_depth-1]);			\
			if (rbp_f_cmp < 0) {				\
			    rbp_f_node = rbp_left_get(a_type, a_field,	\
			      rbp_f_path[rbp_f_depth-1]);		\
			} else if (rbp_f_cmp > 0) {			\
			    rbp_f_node = rbp_right_get(a_type, a_field,	\
			      rbp_f_path[rbp_f_depth-1]);		\
			} else {					\
			    break;					\
			}						\
			assert(rbp_f_node != &(a_tree)->rbt_nil);	\
			rbp_f_path[rbp_f_depth] = rbp_f_node;		\
			rbp_f_depth++;					\
		    }							\
		}							\
	    }								\
	    rbp_f_synced = true;

#define	rb_foreach_end(a_type, a_field, a_tree, a_var)			\
	    if (rbp_f_synced) {						\
		rbp_f_synced = false;					\
		continue;						\
	    }								\
	    \
	    if ((rbp_f_node = rbp_right_get(a_type, a_field,		\
	      rbp_f_path[rbp_f_depth-1])) != &(a_tree)->rbt_nil) {	\
	        \
		\
		rbp_f_path[rbp_f_depth] = rbp_f_node;			\
		rbp_f_depth++;						\
		while ((rbp_f_node = rbp_left_get(a_type, a_field,	\
		  rbp_f_path[rbp_f_depth-1])) != &(a_tree)->rbt_nil) {	\
		    rbp_f_path[rbp_f_depth] = rbp_f_node;		\
		    rbp_f_depth++;					\
		}							\
	    } else {							\
		\
		\
		\
		for (rbp_f_depth--; rbp_f_depth > 0; rbp_f_depth--) {	\
		    if (rbp_left_get(a_type, a_field,			\
		      rbp_f_path[rbp_f_depth-1])			\
		      == rbp_f_path[rbp_f_depth]) {			\
			break;						\
		    }							\
		}							\
	    }								\
	}								\
    }									\
}

#define	rb_foreach_reverse_begin(a_type, a_field, a_tree, a_var) {	\
    rbp_compute_fr_height(a_type, a_field, a_tree)			\
    {									\
	\
	a_type *rbp_fr_path[rbp_fr_height];				\
	a_type *rbp_fr_node;						\
	bool rbp_fr_synced = false;					\
	unsigned rbp_fr_depth = 0;					\
	if ((a_tree)->rbt_root != &(a_tree)->rbt_nil) {			\
	    rbp_fr_path[rbp_fr_depth] = (a_tree)->rbt_root;		\
	    rbp_fr_depth++;						\
	    while ((rbp_fr_node = rbp_right_get(a_type, a_field,	\
	      rbp_fr_path[rbp_fr_depth-1])) != &(a_tree)->rbt_nil) {	\
		rbp_fr_path[rbp_fr_depth] = rbp_fr_node;		\
		rbp_fr_depth++;						\
	    }								\
	}								\
	\
	while (rbp_fr_depth > 0) {					\
	    (a_var) = rbp_fr_path[rbp_fr_depth-1];

#define	rb_foreach_reverse_prev(a_type, a_field, a_cmp, a_tree, a_node)	\
	    \
	    rbp_fr_depth = 0;						\
	    if (a_node != NULL) {					\
		if ((a_tree)->rbt_root != &(a_tree)->rbt_nil) {		\
		    rbp_fr_path[rbp_fr_depth] = (a_tree)->rbt_root;	\
		    rbp_fr_depth++;					\
		    rbp_fr_node = rbp_fr_path[0];			\
		    while (true) {					\
			int rbp_fr_cmp = (a_cmp)((a_node),		\
			  rbp_fr_path[rbp_fr_depth-1]);			\
			if (rbp_fr_cmp < 0) {				\
			    rbp_fr_node = rbp_left_get(a_type, a_field,	\
			      rbp_fr_path[rbp_fr_depth-1]);		\
			} else if (rbp_fr_cmp > 0) {			\
			    rbp_fr_node = rbp_right_get(a_type, a_field,\
			      rbp_fr_path[rbp_fr_depth-1]);		\
			} else {					\
			    break;					\
			}						\
			assert(rbp_fr_node != &(a_tree)->rbt_nil);	\
			rbp_fr_path[rbp_fr_depth] = rbp_fr_node;	\
			rbp_fr_depth++;					\
		    }							\
		}							\
	    }								\
	    rbp_fr_synced = true;

#define	rb_foreach_reverse_end(a_type, a_field, a_tree, a_var)		\
	    if (rbp_fr_synced) {					\
		rbp_fr_synced = false;					\
		continue;						\
	    }								\
	    if (rbp_fr_depth == 0) {					\
		\
		\
		break;							\
	    }								\
	    \
	    if ((rbp_fr_node = rbp_left_get(a_type, a_field,		\
	      rbp_fr_path[rbp_fr_depth-1])) != &(a_tree)->rbt_nil) {	\
	        \
		\
		rbp_fr_path[rbp_fr_depth] = rbp_fr_node;		\
		rbp_fr_depth++;						\
		while ((rbp_fr_node = rbp_right_get(a_type, a_field,	\
		  rbp_fr_path[rbp_fr_depth-1])) != &(a_tree)->rbt_nil) {\
		    rbp_fr_path[rbp_fr_depth] = rbp_fr_node;		\
		    rbp_fr_depth++;					\
		}							\
	    } else {							\
		\
		\
		\
		for (rbp_fr_depth--; rbp_fr_depth > 0; rbp_fr_depth--) {\
		    if (rbp_right_get(a_type, a_field,			\
		      rbp_fr_path[rbp_fr_depth-1])			\
		      == rbp_fr_path[rbp_fr_depth]) {			\
			break;						\
		    }							\
		}							\
	    }								\
	}								\
    }									\
}

#endif 

