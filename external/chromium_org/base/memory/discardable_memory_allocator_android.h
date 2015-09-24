// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MEMORY_DISCARDABLE_MEMORY_ALLOCATOR_H_
#define BASE_MEMORY_DISCARDABLE_MEMORY_ALLOCATOR_H_

#include <string>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread_checker.h"

namespace base {

class DiscardableMemory;

namespace internal {

class BASE_EXPORT_PRIVATE DiscardableMemoryAllocator {
 public:
  
  enum {
    kMinAshmemRegionSize = 32 * 1024 * 1024,
  };

  
  explicit DiscardableMemoryAllocator(const std::string& name);
  ~DiscardableMemoryAllocator();

  
  
  scoped_ptr<DiscardableMemory> Allocate(size_t size);

 private:
  class AshmemRegion;
  class DiscardableAshmemChunk;

  void DeleteAshmemRegion_Locked(AshmemRegion* region);

  base::ThreadChecker thread_checker_;
  const std::string name_;
  base::Lock lock_;
  ScopedVector<AshmemRegion> ashmem_regions_;

  DISALLOW_COPY_AND_ASSIGN(DiscardableMemoryAllocator);
};

}  
}  

#endif  
