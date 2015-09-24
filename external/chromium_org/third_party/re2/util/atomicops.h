// Copyright 2006-2008 The RE2 Authors.  All Rights Reserved.
// license that can be found in the LICENSE file.

#ifndef RE2_UTIL_ATOMICOPS_H__
#define RE2_UTIL_ATOMICOPS_H__

#if defined(__i386__)

static inline void WriteMemoryBarrier() {
  int x;
  __asm__ __volatile__("xchgl (%0),%0"  
                       :: "r" (&x));
}

#elif defined(__x86_64__)

static inline void WriteMemoryBarrier() {
  __asm__ __volatile__("sfence" : : : "memory");
}

#elif defined(__ppc__)

static inline void WriteMemoryBarrier() {
  __asm__ __volatile__("eieio" : : : "memory");
}

#elif defined(__alpha__)

static inline void WriteMemoryBarrier() {
  __asm__ __volatile__("wmb" : : : "memory");
}

#else

#include "util/mutex.h"

static inline void WriteMemoryBarrier() {
  
  
  
  
  
  
  
  
  re2::Mutex mu;
  re2::MutexLock l(&mu);
}


#endif

#if defined(__alpha__)

static inline void MaybeReadMemoryBarrier() {
  __asm__ __volatile__("mb" : : : "memory");
}

#else

static inline void MaybeReadMemoryBarrier() {}

#endif 

#endif  
