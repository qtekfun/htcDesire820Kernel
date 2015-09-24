/*-
 * Copyright (C) 2006-2008 Jason Evans <jasone@FreeBSD.org>.
 * All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice(s), this list of conditions and the following disclaimer as
 *    the first lines of this file unmodified other than the possible
 *    addition of one or more copyright notices.
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
 */

#ifndef _JEMALLOC_H_
#define _JEMALLOC_H_

#ifdef _MSC_VER
#include <crtdefs.h>
#else
#include <stddef.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char jemalloc_bool;

extern const char	*_malloc_options;

typedef struct {
	jemalloc_bool	opt_abort;	
	jemalloc_bool	opt_junk;	
	jemalloc_bool	opt_utrace;	
	jemalloc_bool	opt_sysv;	
	jemalloc_bool	opt_xmalloc;	
	jemalloc_bool	opt_zero;	
	size_t	narenas;	
	size_t	balance_threshold; 
	size_t	quantum;	
	size_t	small_max;	
	size_t	large_max;	
	size_t	chunksize;	
	size_t	dirty_max;	
	size_t	reserve_min;	
	size_t	reserve_max;	

	size_t	mapped;		
	size_t	committed;	
	size_t	allocated;	
	size_t	dirty;		
	size_t	reserve_cur;	
} jemalloc_stats_t;

#ifndef MOZ_MEMORY_DARWIN
void	*malloc(size_t size);
void	*valloc(size_t size);
void	*calloc(size_t num, size_t size);
void	*realloc(void *ptr, size_t size);
void	free(void *ptr);
#endif

int	posix_memalign(void **memptr, size_t alignment, size_t size);
void	*memalign(size_t alignment, size_t size);
size_t	malloc_usable_size(const void *ptr);
void	jemalloc_stats(jemalloc_stats_t *stats);

void	*xmalloc(size_t size);
void	*xcalloc(size_t num, size_t size);
void	*xrealloc(void *ptr, size_t size);
void	*xmemalign(size_t alignment, size_t size);


typedef enum {
	RESERVE_CND_LOW,
	RESERVE_CND_CRIT,
	RESERVE_CND_FAIL
} reserve_cnd_t;

typedef void reserve_cb_t(void *ctx, reserve_cnd_t cnd, size_t size);

jemalloc_bool	reserve_cb_register(reserve_cb_t *cb, void *ctx);

jemalloc_bool	reserve_cb_unregister(reserve_cb_t *cb, void *ctx);

size_t	reserve_cur_get(void);

size_t	reserve_min_get(void);

jemalloc_bool	reserve_min_set(size_t min);

#ifdef __cplusplus
} 
#endif

#endif 

