// Copyright (c) 2004, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_BASE_CYCLECLOCK_H_
#define GOOGLE_BASE_CYCLECLOCK_H_

#include "base/basictypes.h"   
#include "base/arm_instruction_set_select.h"
#if defined(__arm__) || defined(__mips__)
# include "base/sysinfo.h"
#endif
#if defined(__MACH__) && defined(__APPLE__)
# include <mach/mach_time.h>
#endif
#if defined(_MSC_VER) && !defined(_M_IX86)
extern "C" uint64 __rdtsc();
#pragma intrinsic(__rdtsc)
#endif
#if defined(ARMV3) || defined(__mips__)
#include <sys/time.h>
#endif

struct CycleClock {
  
  static inline int64 Now() {
#if defined(__MACH__) && defined(__APPLE__)
    
    
    
    
    
    
    
    
    
    return mach_absolute_time();
#elif defined(__i386__)
    int64 ret;
    __asm__ volatile ("rdtsc" : "=A" (ret) );
    return ret;
#elif defined(__x86_64__) || defined(__amd64__)
    uint64 low, high;
    __asm__ volatile ("rdtsc" : "=a" (low), "=d" (high));
    return (high << 32) | low;
#elif defined(__powerpc__) || defined(__ppc__)
    
    int64 tbl, tbu0, tbu1;
    asm("mftbu %0" : "=r" (tbu0));
    asm("mftb  %0" : "=r" (tbl));
    asm("mftbu %0" : "=r" (tbu1));
    tbl &= -static_cast<int64>(tbu0 == tbu1);
    
    return (tbu1 << 32) | tbl;
#elif defined(__sparc__)
    int64 tick;
    asm(".byte 0x83, 0x41, 0x00, 0x00");
    asm("mov   %%g1, %0" : "=r" (tick));
    return tick;
#elif defined(__ia64__)
    int64 itc;
    asm("mov %0 = ar.itc" : "=r" (itc));
    return itc;
#elif defined(_MSC_VER) && defined(_M_IX86)
    
    
    
    
    _asm rdtsc
#elif defined(_MSC_VER)
    return __rdtsc();
#elif defined(ARMV3)
#if defined(ARMV6)  
    uint32 pmccntr;
    uint32 pmuseren;
    uint32 pmcntenset;
    
    asm volatile ("mrc p15, 0, %0, c9, c14, 0" : "=r" (pmuseren));
    if (pmuseren & 1) {  
      asm volatile ("mrc p15, 0, %0, c9, c12, 1" : "=r" (pmcntenset));
      if (pmcntenset & 0x80000000ul) {  
        asm volatile ("mrc p15, 0, %0, c9, c13, 0" : "=r" (pmccntr));
        
        return static_cast<int64>(pmccntr) * 64;  
      }
    }
#endif
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return static_cast<int64>((tv.tv_sec + tv.tv_usec * 0.000001)
                              * CyclesPerSecond());
#elif defined(__mips__)
    
    
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return static_cast<int64>((tv.tv_sec + tv.tv_usec * 0.000001)
                              * CyclesPerSecond());
#else
#error You need to define CycleTimer for your O/S and CPU
#endif
  }
};


#endif  
