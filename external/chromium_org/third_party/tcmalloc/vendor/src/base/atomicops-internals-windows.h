/* Copyright (c) 2006, Google Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ---
 * Author: Sanjay Ghemawat
 */


#ifndef BASE_ATOMICOPS_INTERNALS_WINDOWS_H_
#define BASE_ATOMICOPS_INTERNALS_WINDOWS_H_

#include <stdio.h>
#include <stdlib.h>
#include "base/basictypes.h"  

typedef int32 Atomic32;

#if defined(_WIN64)
#define BASE_HAS_ATOMIC64 1  
#endif

namespace base {
namespace subtle {

typedef int64 Atomic64;


extern "C" {

#if defined(__MINGW32__)
inline LONG FastInterlockedCompareExchange(volatile LONG* ptr,
                                           LONG newval, LONG oldval) {
  return ::InterlockedCompareExchange(const_cast<LONG*>(ptr), newval, oldval);
}
inline LONG FastInterlockedExchange(volatile LONG* ptr, LONG newval) {
  return ::InterlockedExchange(const_cast<LONG*>(ptr), newval);
}
inline LONG FastInterlockedExchangeAdd(volatile LONG* ptr, LONG increment) {
  return ::InterlockedExchangeAdd(const_cast<LONG*>(ptr), increment);
}

#elif _MSC_VER >= 1400   
LONG _InterlockedCompareExchange(volatile LONG* ptr, LONG newval, LONG oldval);
#pragma intrinsic(_InterlockedCompareExchange)
inline LONG FastInterlockedCompareExchange(volatile LONG* ptr,
                                           LONG newval, LONG oldval) {
  return _InterlockedCompareExchange(ptr, newval, oldval);
}

LONG _InterlockedExchange(volatile LONG* ptr, LONG newval);
#pragma intrinsic(_InterlockedExchange)
inline LONG FastInterlockedExchange(volatile LONG* ptr, LONG newval) {
  return _InterlockedExchange(ptr, newval);
}

LONG _InterlockedExchangeAdd(volatile LONG* ptr, LONG increment);
#pragma intrinsic(_InterlockedExchangeAdd)
inline LONG FastInterlockedExchangeAdd(volatile LONG* ptr, LONG increment) {
  return _InterlockedExchangeAdd(ptr, increment);
}

#else
inline LONG FastInterlockedCompareExchange(volatile LONG* ptr,
                                           LONG newval, LONG oldval) {
  return ::InterlockedCompareExchange(ptr, newval, oldval);
}
inline LONG FastInterlockedExchange(volatile LONG* ptr, LONG newval) {
  return ::InterlockedExchange(ptr, newval);
}
inline LONG FastInterlockedExchangeAdd(volatile LONG* ptr, LONG increment) {
  return ::InterlockedExchangeAdd(ptr, increment);
}

#endif  
}  

inline Atomic32 NoBarrier_CompareAndSwap(volatile Atomic32* ptr,
                                         Atomic32 old_value,
                                         Atomic32 new_value) {
  LONG result = FastInterlockedCompareExchange(
      reinterpret_cast<volatile LONG*>(ptr),
      static_cast<LONG>(new_value),
      static_cast<LONG>(old_value));
  return static_cast<Atomic32>(result);
}

inline Atomic32 NoBarrier_AtomicExchange(volatile Atomic32* ptr,
                                         Atomic32 new_value) {
  LONG result = FastInterlockedExchange(
      reinterpret_cast<volatile LONG*>(ptr),
      static_cast<LONG>(new_value));
  return static_cast<Atomic32>(result);
}

inline Atomic32 Barrier_AtomicIncrement(volatile Atomic32* ptr,
                                        Atomic32 increment) {
  return FastInterlockedExchangeAdd(
      reinterpret_cast<volatile LONG*>(ptr),
      static_cast<LONG>(increment)) + increment;
}

inline Atomic32 NoBarrier_AtomicIncrement(volatile Atomic32* ptr,
                                          Atomic32 increment) {
  return Barrier_AtomicIncrement(ptr, increment);
}

}  
}  


#if !(defined(_MSC_VER) && _MSC_VER >= 1400)
inline void MemoryBarrier() {
  Atomic32 value = 0;
  base::subtle::NoBarrier_AtomicExchange(&value, 0);
                        
}
#endif

namespace base {
namespace subtle {

inline void MemoryBarrier() {
  ::MemoryBarrier();
}

inline Atomic32 Acquire_CompareAndSwap(volatile Atomic32* ptr,
                                       Atomic32 old_value,
                                       Atomic32 new_value) {
  return NoBarrier_CompareAndSwap(ptr, old_value, new_value);
}

inline Atomic32 Release_CompareAndSwap(volatile Atomic32* ptr,
                                       Atomic32 old_value,
                                       Atomic32 new_value) {
  return NoBarrier_CompareAndSwap(ptr, old_value, new_value);
}

inline void NoBarrier_Store(volatile Atomic32* ptr, Atomic32 value) {
  *ptr = value;
}

inline void Acquire_Store(volatile Atomic32* ptr, Atomic32 value) {
  NoBarrier_AtomicExchange(ptr, value);
              
}

inline void Release_Store(volatile Atomic32* ptr, Atomic32 value) {
  *ptr = value; 
  
}

inline Atomic32 NoBarrier_Load(volatile const Atomic32* ptr) {
  return *ptr;
}

inline Atomic32 Acquire_Load(volatile const Atomic32* ptr) {
  Atomic32 value = *ptr;
  return value;
}

inline Atomic32 Release_Load(volatile const Atomic32* ptr) {
  MemoryBarrier();
  return *ptr;
}


#if defined(_WIN64) || defined(__MINGW64__)


COMPILE_ASSERT(sizeof(Atomic64) == sizeof(PVOID), atomic_word_is_atomic);


extern "C" {
#if defined(__MINGW64__)
inline PVOID FastInterlockedCompareExchangePointer(volatile PVOID* ptr,
                                                   PVOID newval, PVOID oldval) {
  return ::InterlockedCompareExchangePointer(const_cast<PVOID*>(ptr),
                                             newval, oldval);
}
inline PVOID FastInterlockedExchangePointer(volatile PVOID* ptr, PVOID newval) {
  return ::InterlockedExchangePointer(const_cast<PVOID*>(ptr), newval);
}
inline LONGLONG FastInterlockedExchangeAdd64(volatile LONGLONG* ptr,
                                             LONGLONG increment) {
  return ::InterlockedExchangeAdd64(const_cast<LONGLONG*>(ptr), increment);
}

#elif _MSC_VER >= 1400   
PVOID _InterlockedCompareExchangePointer(volatile PVOID* ptr,
                                         PVOID newval, PVOID oldval);
#pragma intrinsic(_InterlockedCompareExchangePointer)
inline PVOID FastInterlockedCompareExchangePointer(volatile PVOID* ptr,
                                                   PVOID newval, PVOID oldval) {
  return _InterlockedCompareExchangePointer(const_cast<PVOID*>(ptr),
                                            newval, oldval);
}

PVOID _InterlockedExchangePointer(volatile PVOID* ptr, PVOID newval);
#pragma intrinsic(_InterlockedExchangePointer)
inline PVOID FastInterlockedExchangePointer(volatile PVOID* ptr, PVOID newval) {
  return _InterlockedExchangePointer(const_cast<PVOID*>(ptr), newval);
}

LONGLONG _InterlockedExchangeAdd64(volatile LONGLONG* ptr, LONGLONG increment);
#pragma intrinsic(_InterlockedExchangeAdd64)
inline LONGLONG FastInterlockedExchangeAdd64(volatile LONGLONG* ptr,
                                             LONGLONG increment) {
  return _InterlockedExchangeAdd64(const_cast<LONGLONG*>(ptr), increment);
}

#else
inline PVOID FastInterlockedCompareExchangePointer(volatile PVOID* ptr,
                                                   PVOID newval, PVOID oldval) {
  return ::InterlockedCompareExchangePointer(ptr, newval, oldval);
}
inline PVOID FastInterlockedExchangePointer(volatile PVOID* ptr, PVOID newval) {
  return ::InterlockedExchangePointer(ptr, newval);
}
inline LONGLONG FastInterlockedExchangeAdd64(volatile LONGLONG* ptr,
                                         LONGLONG increment) {
  return ::InterlockedExchangeAdd64(ptr, increment);
}

#endif  
}  

inline Atomic64 NoBarrier_CompareAndSwap(volatile Atomic64* ptr,
                                         Atomic64 old_value,
                                         Atomic64 new_value) {
  PVOID result = FastInterlockedCompareExchangePointer(
    reinterpret_cast<volatile PVOID*>(ptr),
    reinterpret_cast<PVOID>(new_value), reinterpret_cast<PVOID>(old_value));
  return reinterpret_cast<Atomic64>(result);
}

inline Atomic64 NoBarrier_AtomicExchange(volatile Atomic64* ptr,
                                         Atomic64 new_value) {
  PVOID result = FastInterlockedExchangePointer(
    reinterpret_cast<volatile PVOID*>(ptr),
    reinterpret_cast<PVOID>(new_value));
  return reinterpret_cast<Atomic64>(result);
}

inline Atomic64 Barrier_AtomicIncrement(volatile Atomic64* ptr,
                                        Atomic64 increment) {
  return FastInterlockedExchangeAdd64(
      reinterpret_cast<volatile LONGLONG*>(ptr),
      static_cast<LONGLONG>(increment)) + increment;
}

inline Atomic64 NoBarrier_AtomicIncrement(volatile Atomic64* ptr,
                                          Atomic64 increment) {
  return Barrier_AtomicIncrement(ptr, increment);
}

inline void NoBarrier_Store(volatile Atomic64* ptr, Atomic64 value) {
  *ptr = value;
}

inline void Acquire_Store(volatile Atomic64* ptr, Atomic64 value) {
  NoBarrier_AtomicExchange(ptr, value);
              
}

inline void Release_Store(volatile Atomic64* ptr, Atomic64 value) {
  *ptr = value; 

  
  
  
  
  
  
}

inline Atomic64 NoBarrier_Load(volatile const Atomic64* ptr) {
  return *ptr;
}

inline Atomic64 Acquire_Load(volatile const Atomic64* ptr) {
  Atomic64 value = *ptr;
  return value;
}

inline Atomic64 Release_Load(volatile const Atomic64* ptr) {
  MemoryBarrier();
  return *ptr;
}

#else  



inline void NotImplementedFatalError(const char *function_name) {
  fprintf(stderr, "64-bit %s() not implemented on this platform\n",
          function_name);
  abort();
}

inline Atomic64 NoBarrier_CompareAndSwap(volatile Atomic64* ptr,
                                         Atomic64 old_value,
                                         Atomic64 new_value) {
#if 0 
  Atomic64 prev;
  __asm__ __volatile__("movl (%3), %%ebx\n\t"    
                       "movl 4(%3), %%ecx\n\t"   
                       "lock; cmpxchg8b %1\n\t"  
                       : "=A" (prev)             
                       : "m" (*ptr),             
                         "0" (old_value),        
                         "r" (&new_value)        
                       : "memory", "%ebx", "%ecx");
  return prev;
#else
  NotImplementedFatalError("NoBarrier_CompareAndSwap");
  return 0;
#endif
}

inline Atomic64 NoBarrier_AtomicExchange(volatile Atomic64* ptr,
                                         Atomic64 new_value) {
#if 0 
  __asm__ __volatile__(
                       "movl (%2), %%ebx\n\t"    
                       "movl 4(%2), %%ecx\n\t"   
                       "0:\n\t"
                       "movl %1, %%eax\n\t"      
                       "movl 4%1, %%edx\n\t"     
                       "lock; cmpxchg8b %1\n\t"  
                       "jnz 0b\n\t"              
                       : "=A" (new_value)
                       : "m" (*ptr),
                         "r" (&new_value)
                       : "memory", "%ebx", "%ecx");
  return new_value;  
#else
  NotImplementedFatalError("NoBarrier_AtomicExchange");
  return 0;
#endif
}

inline Atomic64 NoBarrier_AtomicIncrement(volatile Atomic64* ptr,
                                          Atomic64 increment) {
#if 0 
  Atomic64 temp = increment;
  __asm__ __volatile__(
                       "0:\n\t"
                       "movl (%3), %%ebx\n\t"    
                       "movl 4(%3), %%ecx\n\t"   
                       "movl (%2), %%eax\n\t"    
                       "movl 4(%2), %%edx\n\t"   
                       "add %%eax, %%ebx\n\t"    
                       "adc %%edx, %%ecx\n\t"    
                       "lock; cmpxchg8b (%2)\n\t"
                       "jnz 0b\n\t"              
                       : "=A"(temp), "+m"(*ptr)
                       : "D" (ptr), "S" (&increment)
                       : "memory", "%ebx", "%ecx");
  
  return temp + increment;
#else
  NotImplementedFatalError("NoBarrier_AtomicIncrement");
  return 0;
#endif
}

inline Atomic64 Barrier_AtomicIncrement(volatile Atomic64* ptr,
                                        Atomic64 increment) {
#if 0 
  Atomic64 new_val = NoBarrier_AtomicIncrement(ptr, increment);
  if (AtomicOps_Internalx86CPUFeatures.has_amd_lock_mb_bug) {
    __asm__ __volatile__("lfence" : : : "memory");
  }
  return new_val;
#else
  NotImplementedFatalError("Barrier_AtomicIncrement");
  return 0;
#endif
}

inline void NoBarrier_Store(volatile Atomic64* ptr, Atomic64 value) {
#if 0 
  __asm {
    mov mm0, value;  
    mov ptr, mm0;
    emms;            
  }
#else
  NotImplementedFatalError("NoBarrier_Store");
#endif
}

inline void Acquire_Store(volatile Atomic64* ptr, Atomic64 value) {
  NoBarrier_AtomicExchange(ptr, value);
              
}

inline void Release_Store(volatile Atomic64* ptr, Atomic64 value) {
  NoBarrier_Store(ptr, value);
}

inline Atomic64 NoBarrier_Load(volatile const Atomic64* ptr) {
#if 0 
  Atomic64 value;
  __asm {
    mov mm0, ptr;    
    mov value, mm0;
    emms;            
  }
  return value;
#else
  NotImplementedFatalError("NoBarrier_Store");
  return 0;
#endif
}

inline Atomic64 Acquire_Load(volatile const Atomic64* ptr) {
  Atomic64 value = NoBarrier_Load(ptr);
  return value;
}

inline Atomic64 Release_Load(volatile const Atomic64* ptr) {
  MemoryBarrier();
  return NoBarrier_Load(ptr);
}

#endif  


inline Atomic64 Acquire_CompareAndSwap(volatile Atomic64* ptr,
                                       Atomic64 old_value,
                                       Atomic64 new_value) {
  return NoBarrier_CompareAndSwap(ptr, old_value, new_value);
}

inline Atomic64 Release_CompareAndSwap(volatile Atomic64* ptr,
                                       Atomic64 old_value,
                                       Atomic64 new_value) {
  return NoBarrier_CompareAndSwap(ptr, old_value, new_value);
}

}  
}  

#endif  
