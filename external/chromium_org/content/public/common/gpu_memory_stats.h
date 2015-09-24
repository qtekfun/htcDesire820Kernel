// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_GPU_MEMORY_STATS_H_
#define CONTENT_PUBLIC_COMMON_GPU_MEMORY_STATS_H_


#include <map>

#include "base/basictypes.h"
#include "base/process/process.h"
#include "content/common/content_export.h"

namespace content {

struct CONTENT_EXPORT GPUVideoMemoryUsageStats {
  GPUVideoMemoryUsageStats();
  ~GPUVideoMemoryUsageStats();

  struct CONTENT_EXPORT ProcessStats {
    ProcessStats();
    ~ProcessStats();

    
    size_t video_memory;

    
    
    
    bool has_duplicates;
  };
  typedef std::map<base::ProcessId, ProcessStats> ProcessMap;

  
  ProcessMap process_map;

  
  size_t bytes_allocated;

  
  size_t bytes_allocated_historical_max;
};

}  

#endif  
