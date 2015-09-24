/* Copyright (C) 2008 The Android Open Source Project
**
** This software is licensed under the terms of the GNU General Public
** License version 2, as published by the Free Software Foundation, and
** may be copied, distributed, and modified under those terms.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/
#ifndef _ANDROID_UTILS_SYSTEM_H
#define _ANDROID_UTILS_SYSTEM_H

#include <string.h>
#include <stdint.h>
#define __STDC_FORMAT_MACROS 1
#include <inttypes.h>  
#include "android/utils/assert.h"

void*  _android_array_alloc( size_t  itemSize, size_t  count );
void*  _android_array_alloc0( size_t  itemSize, size_t  count );
void*  _android_array_realloc( void* block, size_t  itemSize, size_t  count );


void*  android_alloc( size_t  size );

void*  android_alloc0( size_t  size );

void*  android_realloc( void*  block, size_t  size );

void   android_free( void*  block );


#define  AZERO(p)             memset((char*)(p),0,sizeof(*(p)))
#define  ANEW(p)              (p = android_alloc(sizeof(*p)))
#define  ANEW0(p)             (p = android_alloc0(sizeof(*p)))
#define  AFREE(p)             android_free(p)

#define  AMEM_ZERO(dst,size)      memset((char*)(dst), 0, (size_t)(size))
#define  AMEM_COPY(dst,src,size)  memcpy((char*)(dst),(const char*)(src),(size_t)(size))
#define  AMEM_MOVE(dst,src,size)  memmove((char*)(dst),(const char*)(src),(size_t)(size))

#define  AARRAY_NEW(p,count)          (AASSERT_LOC(), (p) = _android_array_alloc(sizeof(*p),(count)))
#define  AARRAY_NEW0(p,count)         (AASSERT_LOC(), (p) = _android_array_alloc0(sizeof(*p),(count)))
#define  AARRAY_RENEW(p,count)        (AASSERT_LOC(), (p) = _android_array_realloc((p),sizeof(*(p)),(count)))

#define  AARRAY_COPY(dst,src,count)   AMEM_COPY(dst,src,(count)*sizeof((dst)[0]))
#define  AARRAY_MOVE(dst,src,count)   AMEM_MOVE(dst,src,(count)*sizeof((dst)[0]))
#define  AARRAY_ZERO(dst,count)       AMEM_ZERO(dst,(count)*sizeof((dst)[0]))

#define  AARRAY_STATIC_LEN(a)         (sizeof((a))/sizeof((a)[0]))

#define  AINLINED  static __inline__

char*   android_strdup(const char*  src);

#define  ASTRDUP(str)  android_strdup(str)

typedef int  APosixStatus;

typedef int  ABool;

#ifndef STRINGIFY
#define  _STRINGIFY(x)  #x
#define  STRINGIFY(x)  _STRINGIFY(x)
#endif

#ifndef GLUE
#define  _GLUE(x,y)  x##y
#define  GLUE(x,y)   _GLUE(x,y)

#define  _GLUE3(x,y,z)  x##y##z
#define  GLUE3(x,y,z)    _GLUE3(x,y,z)
#endif

#ifdef _WIN32
#  undef   strsep
#  define  strsep    win32_strsep
extern char*  win32_strsep(char**  pline, const char*  delim);
#endif

#ifdef _WIN32
#  define  strcasecmp  stricmp
#endif


#ifdef _WIN32
#  define   CHECKED(ret, call)    (ret) = (call)
#else
#  define   CHECKED(ret, call)    do { (ret) = (call); } while ((ret) < 0 && errno == EINTR)
#endif

#ifdef _WIN32
typedef struct { int  dumy; }      signal_state_t;
#else
#include <signal.h>
typedef struct { sigset_t  old; }  signal_state_t;
#endif

extern  void   disable_sigalrm( signal_state_t  *state );
extern  void   restore_sigalrm( signal_state_t  *state );

#ifdef _WIN32

#define   BEGIN_NOSIGALRM  \
    {

#define   END_NOSIGALRM  \
    }

#else 

#define   BEGIN_NOSIGALRM  \
    { signal_state_t  __sigalrm_state; \
      disable_sigalrm( &__sigalrm_state );

#define   END_NOSIGALRM  \
      restore_sigalrm( &__sigalrm_state );  \
    }

#endif 


extern  void   sleep_ms( int  timeout );


#ifndef PRId64
#  define PRId64  "lld"
#endif
#ifndef PRIx64
#  define PRIx64  "llx"
#endif
#ifndef PRIu64
#  define PRIu64  "llu"
#endif


#endif 
