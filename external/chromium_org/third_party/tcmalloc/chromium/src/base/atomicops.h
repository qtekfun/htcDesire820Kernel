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




#ifndef THREAD_ATOMICOPS_H_
#define THREAD_ATOMICOPS_H_

#include <config.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif


#include "base/arm_instruction_set_select.h"

#if defined(__MACH__) && defined(__APPLE__)
#include "base/atomicops-internals-macosx.h"
#elif defined(__GNUC__) && defined(ARMV6)
#include "base/atomicops-internals-arm-v6plus.h"
#elif defined(ARMV3)
#include "base/atomicops-internals-arm-generic.h"
#elif defined(_WIN32)
#include "base/atomicops-internals-windows.h"
#elif defined(__GNUC__) && (defined(__i386) || defined(__x86_64__))
#include "base/atomicops-internals-x86.h"
#elif defined(__linux__) && defined(__PPC__)
#include "base/atomicops-internals-linuxppc.h"
#else
#include "base/atomicops-internals-x86.h"
#endif

typedef intptr_t AtomicWord;

#ifdef AtomicWordCastType

namespace base {
namespace subtle {

inline AtomicWord NoBarrier_CompareAndSwap(volatile AtomicWord* ptr,
                                           AtomicWord old_value,
                                           AtomicWord new_value) {
  return NoBarrier_CompareAndSwap(
      reinterpret_cast<volatile AtomicWordCastType*>(ptr),
      old_value, new_value);
}

inline AtomicWord NoBarrier_AtomicExchange(volatile AtomicWord* ptr,
                                           AtomicWord new_value) {
  return NoBarrier_AtomicExchange(
      reinterpret_cast<volatile AtomicWordCastType*>(ptr), new_value);
}

inline AtomicWord NoBarrier_AtomicIncrement(volatile AtomicWord* ptr,
                                            AtomicWord increment) {
  return NoBarrier_AtomicIncrement(
      reinterpret_cast<volatile AtomicWordCastType*>(ptr), increment);
}

inline AtomicWord Barrier_AtomicIncrement(volatile AtomicWord* ptr,
                                          AtomicWord increment) {
  return Barrier_AtomicIncrement(
      reinterpret_cast<volatile AtomicWordCastType*>(ptr), increment);
}

inline AtomicWord Acquire_CompareAndSwap(volatile AtomicWord* ptr,
                                         AtomicWord old_value,
                                         AtomicWord new_value) {
  return base::subtle::Acquire_CompareAndSwap(
      reinterpret_cast<volatile AtomicWordCastType*>(ptr),
      old_value, new_value);
}

inline AtomicWord Release_CompareAndSwap(volatile AtomicWord* ptr,
                                         AtomicWord old_value,
                                         AtomicWord new_value) {
  return base::subtle::Release_CompareAndSwap(
      reinterpret_cast<volatile AtomicWordCastType*>(ptr),
      old_value, new_value);
}

inline void NoBarrier_Store(volatile AtomicWord *ptr, AtomicWord value) {
  NoBarrier_Store(
      reinterpret_cast<volatile AtomicWordCastType*>(ptr), value);
}

inline void Acquire_Store(volatile AtomicWord* ptr, AtomicWord value) {
  return base::subtle::Acquire_Store(
      reinterpret_cast<volatile AtomicWordCastType*>(ptr), value);
}

inline void Release_Store(volatile AtomicWord* ptr, AtomicWord value) {
  return base::subtle::Release_Store(
      reinterpret_cast<volatile AtomicWordCastType*>(ptr), value);
}

inline AtomicWord NoBarrier_Load(volatile const AtomicWord *ptr) {
  return NoBarrier_Load(
      reinterpret_cast<volatile const AtomicWordCastType*>(ptr));
}

inline AtomicWord Acquire_Load(volatile const AtomicWord* ptr) {
  return base::subtle::Acquire_Load(
      reinterpret_cast<volatile const AtomicWordCastType*>(ptr));
}

inline AtomicWord Release_Load(volatile const AtomicWord* ptr) {
  return base::subtle::Release_Load(
      reinterpret_cast<volatile const AtomicWordCastType*>(ptr));
}

}  
}  
#endif  


#if 0

typedef int32_t Atomic32;

namespace base {
namespace subtle {

typedef int64_t Atomic64;

Atomic32 NoBarrier_CompareAndSwap(volatile Atomic32* ptr,
                                  Atomic32 old_value,
                                  Atomic32 new_value);
Atomic32 NoBarrier_AtomicExchange(volatile Atomic32* ptr, Atomic32 new_value);
Atomic32 NoBarrier_AtomicIncrement(volatile Atomic32* ptr, Atomic32 increment);
Atomic32 Barrier_AtomicIncrement(volatile Atomic32* ptr,
                                 Atomic32 increment);
Atomic32 Acquire_CompareAndSwap(volatile Atomic32* ptr,
                                Atomic32 old_value,
                                Atomic32 new_value);
Atomic32 Release_CompareAndSwap(volatile Atomic32* ptr,
                                Atomic32 old_value,
                                Atomic32 new_value);
void NoBarrier_Store(volatile Atomic32* ptr, Atomic32 value);
void Acquire_Store(volatile Atomic32* ptr, Atomic32 value);
void Release_Store(volatile Atomic32* ptr, Atomic32 value);
Atomic32 NoBarrier_Load(volatile const Atomic32* ptr);
Atomic32 Acquire_Load(volatile const Atomic32* ptr);
Atomic32 Release_Load(volatile const Atomic32* ptr);

Atomic64 NoBarrier_CompareAndSwap(volatile Atomic64* ptr,
                                  Atomic64 old_value,
                                  Atomic64 new_value);
Atomic64 NoBarrier_AtomicExchange(volatile Atomic64* ptr, Atomic64 new_value);
Atomic64 NoBarrier_AtomicIncrement(volatile Atomic64* ptr, Atomic64 increment);
Atomic64 Barrier_AtomicIncrement(volatile Atomic64* ptr, Atomic64 increment);

Atomic64 Acquire_CompareAndSwap(volatile Atomic64* ptr,
                                Atomic64 old_value,
                                Atomic64 new_value);
Atomic64 Release_CompareAndSwap(volatile Atomic64* ptr,
                                Atomic64 old_value,
                                Atomic64 new_value);
void NoBarrier_Store(volatile Atomic64* ptr, Atomic64 value);
void Acquire_Store(volatile Atomic64* ptr, Atomic64 value);
void Release_Store(volatile Atomic64* ptr, Atomic64 value);
Atomic64 NoBarrier_Load(volatile const Atomic64* ptr);
Atomic64 Acquire_Load(volatile const Atomic64* ptr);
Atomic64 Release_Load(volatile const Atomic64* ptr);
}  
}  

void MemoryBarrier();

#endif  



#ifdef AtomicWordCastType
inline AtomicWord Acquire_CompareAndSwap(volatile AtomicWord* ptr,
                                         AtomicWord old_value,
                                         AtomicWord new_value) {
  return base::subtle::Acquire_CompareAndSwap(ptr, old_value, new_value);
}

inline AtomicWord Release_CompareAndSwap(volatile AtomicWord* ptr,
                                         AtomicWord old_value,
                                         AtomicWord new_value) {
  return base::subtle::Release_CompareAndSwap(ptr, old_value, new_value);
}

inline void Acquire_Store(volatile AtomicWord* ptr, AtomicWord value) {
  return base::subtle::Acquire_Store(ptr, value);
}

inline void Release_Store(volatile AtomicWord* ptr, AtomicWord value) {
  return base::subtle::Release_Store(ptr, value);
}

inline AtomicWord Acquire_Load(volatile const AtomicWord* ptr) {
  return base::subtle::Acquire_Load(ptr);
}

inline AtomicWord Release_Load(volatile const AtomicWord* ptr) {
  return base::subtle::Release_Load(ptr);
}
#endif  


inline Atomic32 Acquire_CompareAndSwap(volatile Atomic32* ptr,
                                       Atomic32 old_value,
                                       Atomic32 new_value) {
  return base::subtle::Acquire_CompareAndSwap(ptr, old_value, new_value);
}
inline Atomic32 Release_CompareAndSwap(volatile Atomic32* ptr,
                                       Atomic32 old_value,
                                       Atomic32 new_value) {
  return base::subtle::Release_CompareAndSwap(ptr, old_value, new_value);
}
inline void Acquire_Store(volatile Atomic32* ptr, Atomic32 value) {
  base::subtle::Acquire_Store(ptr, value);
}
inline void Release_Store(volatile Atomic32* ptr, Atomic32 value) {
  return base::subtle::Release_Store(ptr, value);
}
inline Atomic32 Acquire_Load(volatile const Atomic32* ptr) {
  return base::subtle::Acquire_Load(ptr);
}
inline Atomic32 Release_Load(volatile const Atomic32* ptr) {
  return base::subtle::Release_Load(ptr);
}

#ifdef BASE_HAS_ATOMIC64


inline base::subtle::Atomic64 Acquire_CompareAndSwap(
    volatile base::subtle::Atomic64* ptr,
    base::subtle::Atomic64 old_value, base::subtle::Atomic64 new_value) {
  return base::subtle::Acquire_CompareAndSwap(ptr, old_value, new_value);
}
inline base::subtle::Atomic64 Release_CompareAndSwap(
    volatile base::subtle::Atomic64* ptr,
    base::subtle::Atomic64 old_value, base::subtle::Atomic64 new_value) {
  return base::subtle::Release_CompareAndSwap(ptr, old_value, new_value);
}
inline void Acquire_Store(
    volatile base::subtle::Atomic64* ptr, base::subtle::Atomic64 value) {
  base::subtle::Acquire_Store(ptr, value);
}
inline void Release_Store(
    volatile base::subtle::Atomic64* ptr, base::subtle::Atomic64 value) {
  return base::subtle::Release_Store(ptr, value);
}
inline base::subtle::Atomic64 Acquire_Load(
    volatile const base::subtle::Atomic64* ptr) {
  return base::subtle::Acquire_Load(ptr);
}
inline base::subtle::Atomic64 Release_Load(
    volatile const base::subtle::Atomic64* ptr) {
  return base::subtle::Release_Load(ptr);
}

#endif  

#endif  
