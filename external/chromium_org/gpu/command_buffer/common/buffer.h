// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_COMMON_BUFFER_H_
#define GPU_COMMAND_BUFFER_COMMON_BUFFER_H_

#include "gpu/command_buffer/common/types.h"

namespace base {
  class SharedMemory;
}

namespace gpu {

struct Buffer {
  Buffer() : ptr(NULL), size(0), shared_memory(NULL) {
  }

  void* ptr;
  size_t size;

  
  base::SharedMemory* shared_memory;
};

}  

#endif  
