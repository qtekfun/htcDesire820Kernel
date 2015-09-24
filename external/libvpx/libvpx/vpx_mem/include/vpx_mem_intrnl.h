/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */


#ifndef __VPX_MEM_INTRNL_H__
#define __VPX_MEM_INTRNL_H__
#include "./vpx_config.h"

#ifndef CONFIG_MEM_MANAGER
# if defined(VXWORKS)
#  define CONFIG_MEM_MANAGER  1 
# else
#  define CONFIG_MEM_MANAGER  0 
# endif
#endif 

#ifndef CONFIG_MEM_TRACKER
# define CONFIG_MEM_TRACKER     1 
#endif

#ifndef CONFIG_MEM_CHECKS
# define CONFIG_MEM_CHECKS      0 
#endif

#ifndef USE_GLOBAL_FUNCTION_POINTERS
# define USE_GLOBAL_FUNCTION_POINTERS   0  
#endif

#if CONFIG_MEM_TRACKER
# include "vpx_mem_tracker.h"
# if VPX_MEM_TRACKER_VERSION_CHIEF != 2 || VPX_MEM_TRACKER_VERSION_MAJOR != 5
#  error "vpx_mem requires memory tracker version 2.5 to track memory usage"
# endif
#endif

#define ADDRESS_STORAGE_SIZE      sizeof(size_t)

#ifndef DEFAULT_ALIGNMENT
# if defined(VXWORKS)
#  define DEFAULT_ALIGNMENT        32        
# else
#  define DEFAULT_ALIGNMENT        (2 * sizeof(void*))  
# endif
#endif

#if CONFIG_MEM_TRACKER
# define TRY_BOUNDS_CHECK         1        
#else
# define TRY_BOUNDS_CHECK         0
#endif 

#if TRY_BOUNDS_CHECK
# define TRY_BOUNDS_CHECK_ON_FREE 0          
# define BOUNDS_CHECK_VALUE       0xdeadbeef 
# define BOUNDS_CHECK_PAD_SIZE    32         
#else
# define BOUNDS_CHECK_VALUE       0
# define BOUNDS_CHECK_PAD_SIZE    0
#endif 

#ifndef REMOVE_PRINTFS
# define REMOVE_PRINTFS 0
#endif

#if REMOVE_PRINTFS
# define _P(x)
#else
# define _P(x) x
#endif

#define align_addr(addr,align) (void*)(((size_t)(addr) + ((align) - 1)) & (size_t)-(align))

#endif 
