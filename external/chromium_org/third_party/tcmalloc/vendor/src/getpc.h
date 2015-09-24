// Copyright (c) 2005, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef BASE_GETPC_H_
#define BASE_GETPC_H_

#include "config.h"

#define _GNU_SOURCE 1

#include <string.h>         
#if defined(HAVE_SYS_UCONTEXT_H)
#include <sys/ucontext.h>
#elif defined(HAVE_UCONTEXT_H)
#include <ucontext.h>       
#elif defined(HAVE_CYGWIN_SIGNAL_H)
#include <cygwin/signal.h>
typedef ucontext ucontext_t;
#endif


struct CallUnrollInfo {
  
  
  
  
  
  int pc_offset;
  
  
  char ins[16];
  
  int ins_size;
  
  
  int return_sp_offset;
};



#if defined(__linux) && defined(__i386) && defined(__GNUC__)
static const CallUnrollInfo callunrollinfo[] = {
  
  
  { 0,
    {0x55, 0x89, 0xe5}, 3,
    0
  },
  
  
  { -1,
    {0x55, 0x89, 0xe5}, 3,
    4
  },
  
  
  { 0,
    {0xc3}, 1,
    0
  }
};

inline void* GetPC(const ucontext_t& signal_ucontext) {
  
  
  const int eip = signal_ucontext.uc_mcontext.gregs[REG_EIP];
  const int esp = signal_ucontext.uc_mcontext.gregs[REG_ESP];
  if ((eip & 0xffff0000) != 0 && (~eip & 0xffff0000) != 0 &&
      (esp & 0xffff0000) != 0) {
    char* eip_char = reinterpret_cast<char*>(eip);
    for (int i = 0; i < sizeof(callunrollinfo)/sizeof(*callunrollinfo); ++i) {
      if (!memcmp(eip_char + callunrollinfo[i].pc_offset,
                  callunrollinfo[i].ins, callunrollinfo[i].ins_size)) {
        
        void **retaddr = (void**)(esp + callunrollinfo[i].return_sp_offset);
        return *retaddr;
      }
    }
  }
  return (void*)eip;
}

#elif defined(_WIN32) || defined(__CYGWIN__) || defined(__CYGWIN32__) || defined(__MINGW32__)

#include "base/logging.h"   
#ifndef HAVE_CYGWIN_SIGNAL_H
typedef int ucontext_t;
#endif

inline void* GetPC(const struct ucontext_t& signal_ucontext) {
  RAW_LOG(ERROR, "GetPC is not yet implemented on Windows\n");
  return NULL;
}

#else
inline void* GetPC(const ucontext_t& signal_ucontext) {
  return (void*)signal_ucontext.PC_FROM_UCONTEXT;   
}

#endif

#endif  
