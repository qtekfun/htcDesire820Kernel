// Copyright (c) 2011, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef TCMALLOC_LIBC_OVERRIDE_GLIBC_INL_H_
#define TCMALLOC_LIBC_OVERRIDE_GLIBC_INL_H_

#if !defined(__MALLOC_HOOK_VOLATILE)
#define MALLOC_HOOK_MAYBE_VOLATILE 
#else
#define MALLOC_HOOK_MAYBE_VOLATILE __MALLOC_HOOK_VOLATILE
#endif

#include <config.h>
#include <features.h>     
#ifdef HAVE_SYS_CDEFS_H
#include <sys/cdefs.h>    
#endif
#include <gperftools/tcmalloc.h>

#ifndef __GLIBC__
# error libc_override_glibc.h is for glibc distributions only.
#endif

#if !defined(__GNUC__) || defined(__MACH__)

# include "libc_override_redefine.h"  

#else  

# include "libc_override_gcc_and_weak.h"


#define ALIAS(tc_fn)   __attribute__ ((alias (#tc_fn)))
extern "C" {
  void* __libc_malloc(size_t size)                ALIAS(tc_malloc);
  void __libc_free(void* ptr)                     ALIAS(tc_free);
  void* __libc_realloc(void* ptr, size_t size)    ALIAS(tc_realloc);
  void* __libc_calloc(size_t n, size_t size)      ALIAS(tc_calloc);
  void __libc_cfree(void* ptr)                    ALIAS(tc_cfree);
  void* __libc_memalign(size_t align, size_t s)   ALIAS(tc_memalign);
  void* __libc_valloc(size_t size)                ALIAS(tc_valloc);
  void* __libc_pvalloc(size_t size)               ALIAS(tc_pvalloc);
  int __posix_memalign(void** r, size_t a, size_t s)  ALIAS(tc_posix_memalign);
}   
#undef ALIAS

#endif  


extern "C" {
static void* glibc_override_malloc(size_t size, const void *caller) {
  return tc_malloc(size);
}
static void* glibc_override_realloc(void *ptr, size_t size,
                                    const void *caller) {
  return tc_realloc(ptr, size);
}
static void glibc_override_free(void *ptr, const void *caller) {
  tc_free(ptr);
}
static void* glibc_override_memalign(size_t align, size_t size,
                                     const void *caller) {
  return tc_memalign(align, size);
}

#if 0
#include <malloc.h>  
void glibc_override_malloc_init_hook(void) {
  __malloc_hook = glibc_override_malloc;
  __realloc_hook = glibc_override_realloc;
  __free_hook = glibc_override_free;
  __memalign_hook = glibc_override_memalign;
}

void (* MALLOC_HOOK_MAYBE_VOLATILE __malloc_initialize_hook)(void)
    = &glibc_override_malloc_init_hook;
#endif

void* (* MALLOC_HOOK_MAYBE_VOLATILE __malloc_hook)(size_t, const void*)
    = &glibc_override_malloc;
void* (* MALLOC_HOOK_MAYBE_VOLATILE __realloc_hook)(void*, size_t, const void*)
    = &glibc_override_realloc;
void (* MALLOC_HOOK_MAYBE_VOLATILE __free_hook)(void*, const void*)
    = &glibc_override_free;
void* (* MALLOC_HOOK_MAYBE_VOLATILE __memalign_hook)(size_t,size_t, const void*)
    = &glibc_override_memalign;

}   


#endif  
