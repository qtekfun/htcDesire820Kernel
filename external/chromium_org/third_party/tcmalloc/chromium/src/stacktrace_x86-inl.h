// Copyright (c) 2005, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef BASE_STACKTRACE_X86_INL_H_
#define BASE_STACKTRACE_X86_INL_H_

#include "config.h"
#include <stdlib.h>   
#include <assert.h>
#if defined(HAVE_SYS_UCONTEXT_H)
#include <sys/ucontext.h>
#elif defined(HAVE_UCONTEXT_H)
#include <ucontext.h>  
#elif defined(HAVE_CYGWIN_SIGNAL_H)
# ifdef HAVE_PTHREAD
# include <pthread.h>
# endif
#include <cygwin/signal.h>
typedef ucontext ucontext_t;
#endif
#ifdef HAVE_STDINT_H
#include <stdint.h>   
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_MMAP
#include <sys/mman.h> 
#include "base/vdso_support.h"
#endif

#include "gperftools/stacktrace.h"
#if defined(KEEP_SHADOW_STACKS)
#include "linux_shadow_stacks.h"
#endif  

#if defined(__linux__) && defined(__i386__) && defined(__ELF__) && defined(HAVE_MMAP)
static const int kMaxBytes = 10;


static int CountPushInstructions(const unsigned char *const addr) {
  int result = 0;
  for (int i = 0; i < kMaxBytes; ++i) {
    if (addr[i] == 0x89) {
      
      if (addr[i + 1] == 0xE5) {
        
        return 0;
      }
      ++i;  
    } else if (addr[i] == 0x0F &&
               (addr[i + 1] == 0x34 || addr[i + 1] == 0x05)) {
      
      return result;
    } else if ((addr[i] & 0xF0) == 0x50) {
      
      ++result;
    } else if (addr[i] == 0xCD && addr[i + 1] == 0x80) {
      
      assert(result == 0);
      return 0;
    } else {
      
      assert(0 == "unexpected instruction in __kernel_vsyscall");
      return 0;
    }
  }
  
  
  assert(0 == "did not find SYSENTER or SYSCALL in __kernel_vsyscall");
  return 0;
}
#endif

template<bool STRICT_UNWINDING, bool WITH_CONTEXT>
static void **NextStackFrame(void **old_sp, const void *uc) {
  void **new_sp = (void **) *old_sp;

#if defined(__linux__) && defined(__i386__) && defined(HAVE_VDSO_SUPPORT)
  if (WITH_CONTEXT && uc != NULL) {
    
    
    
    static int num_push_instructions = -1;  
    
    
    static const unsigned char *kernel_rt_sigreturn_address = NULL;
    static const unsigned char *kernel_vsyscall_address = NULL;
    if (num_push_instructions == -1) {
      base::VDSOSupport vdso;
      if (vdso.IsPresent()) {
        base::VDSOSupport::SymbolInfo rt_sigreturn_symbol_info;
        base::VDSOSupport::SymbolInfo vsyscall_symbol_info;
        if (!vdso.LookupSymbol("__kernel_rt_sigreturn", "LINUX_2.5",
                               STT_FUNC, &rt_sigreturn_symbol_info) ||
            !vdso.LookupSymbol("__kernel_vsyscall", "LINUX_2.5",
                               STT_FUNC, &vsyscall_symbol_info) ||
            rt_sigreturn_symbol_info.address == NULL ||
            vsyscall_symbol_info.address == NULL) {
          
          
          assert(0 == "VDSO is present, but doesn't have expected symbols");
          num_push_instructions = 0;
        } else {
          kernel_rt_sigreturn_address =
              reinterpret_cast<const unsigned char *>(
                  rt_sigreturn_symbol_info.address);
          kernel_vsyscall_address =
              reinterpret_cast<const unsigned char *>(
                  vsyscall_symbol_info.address);
          num_push_instructions =
              CountPushInstructions(kernel_vsyscall_address);
        }
      } else {
        num_push_instructions = 0;
      }
    }
    if (num_push_instructions != 0 && kernel_rt_sigreturn_address != NULL &&
        old_sp[1] == kernel_rt_sigreturn_address) {
      const ucontext_t *ucv = static_cast<const ucontext_t *>(uc);
      
      
      void **const reg_ebp =
          reinterpret_cast<void **>(ucv->uc_mcontext.gregs[REG_EBP]);
      const unsigned char *const reg_eip =
          reinterpret_cast<unsigned char *>(ucv->uc_mcontext.gregs[REG_EIP]);
      if (new_sp == reg_ebp &&
          kernel_vsyscall_address <= reg_eip &&
          reg_eip - kernel_vsyscall_address < kMaxBytes) {
        
        
        void **const reg_esp =
            reinterpret_cast<void **>(ucv->uc_mcontext.gregs[REG_ESP]);
        
        if (reg_esp &&
            ((uintptr_t)reg_esp & (sizeof(reg_esp) - 1)) == 0) {
          
          
          
          
          
          
          static int page_size;
          if (page_size == 0) {
            
            page_size = getpagesize();
          }
          void *const reg_esp_aligned =
              reinterpret_cast<void *>(
                  (uintptr_t)(reg_esp + num_push_instructions - 1) &
                  ~(page_size - 1));
          if (msync(reg_esp_aligned, page_size, MS_ASYNC) == 0) {
            
            new_sp = reinterpret_cast<void **>(
                reg_esp[num_push_instructions - 1]);
          }
        }
      }
    }
  }
#endif

  
  
  if (STRICT_UNWINDING) {
    
    
    if (new_sp <= old_sp) return NULL;
    
    if ((uintptr_t)new_sp - (uintptr_t)old_sp > 100000) return NULL;
  } else {
    
    
    if (new_sp == old_sp) return NULL;
    if (new_sp > old_sp) {
      
      const uintptr_t delta = (uintptr_t)new_sp - (uintptr_t)old_sp;
      const uintptr_t acceptable_delta = 1000000;
      if (delta > acceptable_delta) {
        return NULL;
      }
    }
  }
  if ((uintptr_t)new_sp & (sizeof(void *) - 1)) return NULL;
#ifdef __i386__
  
  
  
  if ((uintptr_t)new_sp >= 0xffffe000) return NULL;
#endif
#ifdef HAVE_MMAP
  if (!STRICT_UNWINDING) {
    
    
    
    
    
    static int page_size = getpagesize();
    void *new_sp_aligned = (void *)((uintptr_t)new_sp & ~(page_size - 1));
    if (msync(new_sp_aligned, page_size, MS_ASYNC) == -1)
      return NULL;
  }
#endif
  return new_sp;
}

#endif  



int GET_STACK_TRACE_OR_FRAMES {
  void **sp;
#if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2) || __llvm__
  
  
  
  
  sp = reinterpret_cast<void**>(__builtin_frame_address(0));
#elif defined(__i386__)
  
  
  
  
  
  
  sp = (void **)&result - 2;
#elif defined(__x86_64__)
  unsigned long rbp;
  
  
  
  
  
  
  
  __asm__ volatile ("mov %%rbp, %0" : "=r" (rbp));
  
  
  sp = (void **) rbp;
#else
# error Using stacktrace_x86-inl.h on a non x86 architecture!
#endif

  int n = 0;
#if defined(KEEP_SHADOW_STACKS)
  void **shadow_ip_stack;
  void **shadow_sp_stack;
  int stack_size;
  shadow_ip_stack = (void**) get_shadow_ip_stack(&stack_size);
  shadow_sp_stack = (void**) get_shadow_sp_stack(&stack_size);
  int shadow_index = stack_size - 1;
  for (int i = stack_size - 1; i >= 0; i--) {
    if (sp == shadow_sp_stack[i]) {
      shadow_index = i;
      break;
    }
  }
  void **prev_sp = NULL;
#endif  
  while (sp && n < max_depth) {
    if (*(sp+1) == reinterpret_cast<void *>(0)) {
      
      
      break;
    }
#if !IS_WITH_CONTEXT
    const void *const ucp = NULL;
#endif
    void **next_sp = NextStackFrame<!IS_STACK_FRAMES, IS_WITH_CONTEXT>(sp, ucp);
    if (skip_count > 0) {
      skip_count--;
#if defined(KEEP_SHADOW_STACKS)
      shadow_index--;
#endif  
    } else {
      result[n] = *(sp+1);
#if defined(KEEP_SHADOW_STACKS)
      if ((shadow_index > 0) && (sp == shadow_sp_stack[shadow_index])) {
        shadow_index--;
      }
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
#if defined(KEEP_SHADOW_STACKS)
    prev_sp = sp;
#endif  
    sp = next_sp;
  }

#if defined(KEEP_SHADOW_STACKS)
  if (shadow_index >= 0) {
    for (int i = shadow_index; i >= 0; i--) {
      if (shadow_sp_stack[i] > prev_sp) {
        result[n] = shadow_ip_stack[i];
        if (n + 1 < max_depth) {
          n++;
          continue;
        }
      }
      break;
    }
  }
#endif  
  return n;
}
