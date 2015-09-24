// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_LOADER_RESOURCE_BUFFER_H_
#define CONTENT_BROWSER_LOADER_RESOURCE_BUFFER_H_

#include <queue>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/shared_memory.h"
#include "content/common/content_export.h"

namespace content {

//   // offset and count) of the bytes written into the ResourceBuffer's shared
class CONTENT_EXPORT ResourceBuffer
    : public base::RefCountedThreadSafe<ResourceBuffer> {
 public:
  ResourceBuffer();

  
  
  
  
  
  
  bool Initialize(int buffer_size,
                  int min_allocation_size,
                  int max_allocation_size);
  bool IsInitialized() const;

  
  
  
  
  bool ShareToProcess(base::ProcessHandle process_handle,
                      base::SharedMemoryHandle* shared_memory_handle,
                      int* shared_memory_size);

  
  bool CanAllocate() const;

  
  
  
  char* Allocate(int* size);

  
  
  int GetLastAllocationOffset() const;

  
  
  void ShrinkLastAllocation(int new_size);

  
  
  void RecycleLeastRecentlyAllocated();

 private:
  friend class base::RefCountedThreadSafe<ResourceBuffer>;
  ~ResourceBuffer();

  base::SharedMemory shared_mem_;

  int buf_size_;
  int min_alloc_size_;
  int max_alloc_size_;

  
  
  
  
  
  
  int alloc_start_;
  int alloc_end_;

  std::queue<int> alloc_sizes_;

  DISALLOW_COPY_AND_ASSIGN(ResourceBuffer);
};

}  

#endif  
