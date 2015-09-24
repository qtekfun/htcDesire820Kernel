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
 */

#if !defined(_BASE_LOW_LEVEL_ALLOC_H_)
#define _BASE_LOW_LEVEL_ALLOC_H_


#include <config.h>
#include <stddef.h>             
#include "base/basictypes.h"

class LowLevelAlloc {
 public:
  struct Arena;       

  
  
  
  
  
  
  static void *Alloc(size_t request)
    ATTRIBUTE_SECTION(malloc_hook);
  static void *AllocWithArena(size_t request, Arena *arena)
    ATTRIBUTE_SECTION(malloc_hook);

  
  
  
  
  
  static void Free(void *s) ATTRIBUTE_SECTION(malloc_hook);

    
    
    
    

  
  
  
  
  enum {
    
    
    kCallMallocHook = 0x0001,

    
    
    kAsyncSignalSafe = 0x0002,

    
    
    
    
    
    
  };
  static Arena *NewArena(int32 flags, Arena *meta_data_arena);

  
  
  
  
  
  static bool DeleteArena(Arena *arena);

  
  static Arena *DefaultArena();

 private:
  LowLevelAlloc();      
};

#endif
