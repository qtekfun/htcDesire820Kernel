// Copyright (c) 2011, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef BASE_STACKTRACE_ARM_INL_H_
#define BASE_STACKTRACE_ARM_INL_H_

#include <stdint.h>   
#include "base/basictypes.h"  
#include <gperftools/stacktrace.h>


template<bool STRICT_UNWINDING>
static void **NextStackFrame(void **old_sp) {
  void **new_sp = (void**) old_sp[-1];

  
  
  if (STRICT_UNWINDING) {
    
    
    if (new_sp <= old_sp) return NULL;
    
    if ((uintptr_t)new_sp - (uintptr_t)old_sp > 100000) return NULL;
  } else {
    
    
    if (new_sp == old_sp) return NULL;
    
    if ((new_sp > old_sp)
        && ((uintptr_t)new_sp - (uintptr_t)old_sp > 1000000)) return NULL;
  }
  if ((uintptr_t)new_sp & (sizeof(void *) - 1)) return NULL;
  return new_sp;
}

#ifdef __GNUC__
void StacktraceArmDummyFunction() __attribute__((noinline));
void StacktraceArmDummyFunction() { __asm__ volatile(""); }
#else
# error StacktraceArmDummyFunction() needs to be ported to this platform.
#endif
#endif  


int GET_STACK_TRACE_OR_FRAMES {
#ifdef __GNUC__
  void **sp = reinterpret_cast<void**>(__builtin_frame_address(0));
#else
# error reading stack point not yet supported on this platform.
#endif

  
  
  
  
  
  StacktraceArmDummyFunction();

  int n = 0;
  while (sp && n < max_depth) {
    
    
    
    
    
    void **next_sp = NextStackFrame<IS_STACK_FRAMES == 0>(sp);

    if (skip_count > 0) {
      skip_count--;
    } else {
      result[n] = *sp;

#if IS_STACK_FRAMES
      if (next_sp > sp) {
        sizes[n] = (uintptr_t)next_sp - (uintptr_t)sp;
      } else {
        
        sizes[n] = 0;
      }
#endif
      n++;
    }
    sp = next_sp;
  }
  return n;
}
