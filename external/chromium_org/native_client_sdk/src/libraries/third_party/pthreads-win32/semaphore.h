/*
 * Module: semaphore.h
 *
 * Purpose:
 *	Semaphores aren't actually part of the PThreads standard.
 *	They are defined by the POSIX Standard:
 *
 *		POSIX 1003.1b-1993	(POSIX.1b)
 *
 * --------------------------------------------------------------------------
 *
 *      Pthreads-win32 - POSIX Threads Library for Win32
 *      Copyright(C) 1998 John E. Bossom
 *      Copyright(C) 1999,2005 Pthreads-win32 contributors
 * 
 *      Contact Email: rpj@callisto.canberra.edu.au
 * 
 *      The current list of contributors is contained
 *      in the file CONTRIBUTORS included with the source
 *      code distribution. The list can also be seen at the
 *      following World Wide Web location:
 *      http://sources.redhat.com/pthreads-win32/contributors.html
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */
#if !defined( SEMAPHORE_H )
#define SEMAPHORE_H

#undef PTW32_SEMAPHORE_LEVEL

#if defined(_POSIX_SOURCE)
#define PTW32_SEMAPHORE_LEVEL 0
#endif

#if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 199309
#undef PTW32_SEMAPHORE_LEVEL
#define PTW32_SEMAPHORE_LEVEL 1
#endif

#if defined(INCLUDE_NP)
#undef PTW32_SEMAPHORE_LEVEL
#define PTW32_SEMAPHORE_LEVEL 2
#endif

#define PTW32_SEMAPHORE_LEVEL_MAX 3

#if !defined(PTW32_SEMAPHORE_LEVEL)
#define PTW32_SEMAPHORE_LEVEL PTW32_SEMAPHORE_LEVEL_MAX
#endif

#if defined(__GNUC__) && ! defined (__declspec)
# error Please upgrade your GNU compiler to one that supports __declspec.
#endif

#if !defined(PTW32_STATIC_LIB)
#  if defined(PTW32_BUILD)
#    define PTW32_DLLPORT __declspec (dllexport)
#  else
#    define PTW32_DLLPORT __declspec (dllimport)
#  endif
#else
#  define PTW32_DLLPORT
#endif


#if !defined(PTW32_CONFIG_H)
#  if defined(WINCE)
#    define NEED_ERRNO
#    define NEED_SEM
#  endif
#  if defined(__MINGW64__)
#    define HAVE_STRUCT_TIMESPEC
#    define HAVE_MODE_T
#  elif defined(_UWIN) || defined(__MINGW32__)
#    define HAVE_MODE_T
#  endif
#endif


#if PTW32_SEMAPHORE_LEVEL >= PTW32_SEMAPHORE_LEVEL_MAX
#if defined(NEED_ERRNO)
#include "need_errno.h"
#else
#include <errno.h>
#endif
#endif 

#define _POSIX_SEMAPHORES

#if defined(__cplusplus)
extern "C"
{
#endif				

#if !defined(HAVE_MODE_T)
typedef unsigned int mode_t;
#endif


typedef struct sem_t_ * sem_t;

PTW32_DLLPORT int __cdecl sem_init (sem_t * sem,
			    int pshared,
			    unsigned int value);

PTW32_DLLPORT int __cdecl sem_destroy (sem_t * sem);

PTW32_DLLPORT int __cdecl sem_trywait (sem_t * sem);

PTW32_DLLPORT int __cdecl sem_wait (sem_t * sem);

PTW32_DLLPORT int __cdecl sem_timedwait (sem_t * sem,
				 const struct timespec * abstime);

PTW32_DLLPORT int __cdecl sem_post (sem_t * sem);

PTW32_DLLPORT int __cdecl sem_post_multiple (sem_t * sem,
				     int count);

PTW32_DLLPORT int __cdecl sem_open (const char * name,
			    int oflag,
			    mode_t mode,
			    unsigned int value);

PTW32_DLLPORT int __cdecl sem_close (sem_t * sem);

PTW32_DLLPORT int __cdecl sem_unlink (const char * name);

PTW32_DLLPORT int __cdecl sem_getvalue (sem_t * sem,
				int * sval);

#if defined(__cplusplus)
}				
#endif				

#undef PTW32_SEMAPHORE_LEVEL
#undef PTW32_SEMAPHORE_LEVEL_MAX

#endif				
