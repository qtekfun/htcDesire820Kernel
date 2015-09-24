// Copyright (c) 2007, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef BASE_STACKTRACE_POWERPC_INL_H_
#define BASE_STACKTRACE_POWERPC_INL_H_

#include <stdint.h>   
#include <stdlib.h>   
#include <gperftools/stacktrace.h>

template<bool STRICT_UNWINDING>
static void **NextStackFrame(void **old_sp) {
  void **new_sp = (void **) *old_sp;

  
  
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

void StacktracePowerPCDummyFunction() __attribute__((noinline));
void StacktracePowerPCDummyFunction() { __asm__ volatile(""); }
#endif  


int GET_STACK_TRACE_OR_FRAMES {
  void **sp;
  
  
  
  
  
#ifdef __APPLE__
  __asm__ volatile ("mr %0,r1" : "=r" (sp));
#else
  __asm__ volatile ("mr %0,1" : "=r" (sp));
#endif

  
  
  
  
  
  
  
  StacktracePowerPCDummyFunction();

#if IS_STACK_FRAMES
  
  
  
  
#else
  
  skip_count++;
#endif

  int n = 0;
  while (sp && n < max_depth) {
    
    
    
    
    
    void **next_sp = NextStackFrame<!IS_STACK_FRAMES>(sp);

    if (skip_count > 0) {
      skip_count--;
    } else {
      
      
      
      
#if defined(_CALL_AIX) || defined(_CALL_DARWIN)
      result[n] = *(sp+2);
#elif defined(_CALL_SYSV)
      result[n] = *(sp+1);
#elif defined(__APPLE__) || (defined(__linux) && defined(__PPC64__))
      
      result[n] = *(sp+2);
#elif defined(__linux)
      
      result[n] = *(sp+1);
#else
#error Need to specify the PPC ABI for your archiecture.
#endif

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
