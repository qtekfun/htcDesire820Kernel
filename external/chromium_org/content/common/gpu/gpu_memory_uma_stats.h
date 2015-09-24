// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_GPU_MEMORY_UMA_STATS_H_
#define CONTENT_COMMON_GPU_GPU_MEMORY_UMA_STATS_H_

#include "base/basictypes.h"

namespace content {

struct GPUMemoryUmaStats {
  GPUMemoryUmaStats()
      : bytes_allocated_current(0),
        bytes_allocated_max(0),
        bytes_limit(0),
        client_count(0),
        context_group_count(0) {
  }

  
  size_t bytes_allocated_current;

  
  size_t bytes_allocated_max;

  
  size_t bytes_limit;

  
  size_t client_count;

  
  size_t context_group_count;

  
  uint32 window_count;
};

}  

#endif 
