// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_DEV_MEMORY_DEV_H_
#define PPAPI_CPP_DEV_MEMORY_DEV_H_

#include "ppapi/c/pp_stdint.h"


namespace pp {

class Memory_Dev {
 public:
  Memory_Dev() {}

  
  
  
  
  
  void* MemAlloc(uint32_t num_bytes);

  
  
  
  
  void MemFree(void* ptr);
};

}  

#endif  
