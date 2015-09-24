// Copyright (c) 2007, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#include <config.h>

#ifndef _XOPEN_SOURCE
# define _XOPEN_SOURCE 600  
#endif
#include <stdlib.h>         
#if defined(HAVE_MALLOC_H) && !defined(__FreeBSD__)
#include <malloc.h>         
#endif

#ifndef __THROW    
# define __THROW   
#endif

#if !HAVE_CFREE_SYMBOL
extern "C" void cfree(void* ptr) __THROW;
#endif
#if !HAVE_POSIX_MEMALIGN_SYMBOL
extern "C" int posix_memalign(void** ptr, size_t align, size_t size) __THROW;
#endif
#if !HAVE_MEMALIGN_SYMBOL
extern "C" void* memalign(size_t __alignment, size_t __size) __THROW;
#endif
#if !HAVE_VALLOC_SYMBOL
extern "C" void* valloc(size_t __size) __THROW;
#endif
#if !HAVE_PVALLOC_SYMBOL
extern "C" void* pvalloc(size_t __size) __THROW;
#endif
