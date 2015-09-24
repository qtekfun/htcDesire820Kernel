// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_COMMON_GPU_MEMORY_ALLOCATION_H_
#define GPU_COMMAND_BUFFER_COMMON_GPU_MEMORY_ALLOCATION_H_

#include "base/basictypes.h"

namespace gpu {

struct MemoryAllocation {
  enum PriorityCutoff {
    
    CUTOFF_ALLOW_NOTHING,
    
    
    CUTOFF_ALLOW_REQUIRED_ONLY,
    
    
    
    CUTOFF_ALLOW_NICE_TO_HAVE,
    
    CUTOFF_ALLOW_EVERYTHING,
  };

  
  uint64 bytes_limit_when_visible;
  PriorityCutoff priority_cutoff_when_visible;

  MemoryAllocation()
      : bytes_limit_when_visible(0),
        priority_cutoff_when_visible(CUTOFF_ALLOW_NOTHING) {
  }

  MemoryAllocation(uint64 bytes_limit_when_visible)
      : bytes_limit_when_visible(bytes_limit_when_visible),
        priority_cutoff_when_visible(CUTOFF_ALLOW_EVERYTHING) {
  }

  bool Equals(const MemoryAllocation& other) const {
    return bytes_limit_when_visible ==
               other.bytes_limit_when_visible &&
        priority_cutoff_when_visible == other.priority_cutoff_when_visible;
  }
};

struct ManagedMemoryStats {
  
  uint64 bytes_required;

  
  
  uint64 bytes_nice_to_have;

  
  uint64 bytes_allocated;

  
  
  bool backbuffer_requested;

  ManagedMemoryStats()
      : bytes_required(0),
        bytes_nice_to_have(0),
        bytes_allocated(0),
        backbuffer_requested(false) {
  }

  ManagedMemoryStats(uint64 bytes_required,
                        uint64 bytes_nice_to_have,
                        uint64 bytes_allocated,
                        bool backbuffer_requested)
      : bytes_required(bytes_required),
        bytes_nice_to_have(bytes_nice_to_have),
        bytes_allocated(bytes_allocated),
        backbuffer_requested(backbuffer_requested) {
  }

  bool Equals(const ManagedMemoryStats& other) const {
    return bytes_required == other.bytes_required &&
        bytes_nice_to_have == other.bytes_nice_to_have &&
        bytes_allocated == other.bytes_allocated &&
        backbuffer_requested == other.backbuffer_requested;
  }
};

}  

#endif 
