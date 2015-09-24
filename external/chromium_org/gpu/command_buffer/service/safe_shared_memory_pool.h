// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_SERVICE_SAFE_SHARED_MEMORY_POOL_H_
#define GPU_COMMAND_BUFFER_SERVICE_SAFE_SHARED_MEMORY_POOL_H_

#include <map>

#include "base/basictypes.h"
#include "base/memory/shared_memory.h"
#include "base/synchronization/lock.h"
#include "build/build_config.h"

namespace gpu {
class SafeSharedMemoryPool;


class ScopedSafeSharedMemory {
 public:
  base::SharedMemory* shared_memory();
  ScopedSafeSharedMemory(SafeSharedMemoryPool* pool,
                         base::SharedMemory* memory,
                         size_t shm_size);
  ~ScopedSafeSharedMemory();
 private:
  base::SharedMemory* safe_shared_memory_;
  base::SharedMemoryHandle original_handle_;
  SafeSharedMemoryPool* pool_;

  DISALLOW_COPY_AND_ASSIGN(ScopedSafeSharedMemory);
};

class SafeSharedMemoryPool {
 public:
  SafeSharedMemoryPool();
  virtual ~SafeSharedMemoryPool();

 private:
  friend class ScopedSafeSharedMemory;

  
  
  base::SharedMemory* AcquireSafeSharedMemory(base::SharedMemory*, size_t size);
  void ReleaseSafeSharedMemory(const base::SharedMemoryHandle&);

  
  base::SharedMemory* DuplicateSharedMemory(base::SharedMemory*, size_t size);

  
  struct TrackedMemory {
    base::SharedMemory* safe_shared_memory;
    size_t shm_size;
    int reference_count;
  };

  typedef std::map<base::SharedMemoryHandle, TrackedMemory> MemoryMap;
  MemoryMap memory_;

  
  int handles_acquired_;
  int handles_consumed_;
  size_t address_space_consumed_;
  int max_handles_acquired_;
  int max_handles_consumed_;
  size_t max_address_space_consumed_;

  base::Lock lock_;

  DISALLOW_COPY_AND_ASSIGN(SafeSharedMemoryPool);
};

}  

#endif  

