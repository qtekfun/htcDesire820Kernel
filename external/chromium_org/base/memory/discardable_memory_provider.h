// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MEMORY_DISCARDABLE_MEMORY_PROVIDER_H_
#define BASE_MEMORY_DISCARDABLE_MEMORY_PROVIDER_H_

#include "base/base_export.h"
#include "base/containers/hash_tables.h"
#include "base/containers/mru_cache.h"
#include "base/memory/memory_pressure_listener.h"
#include "base/synchronization/lock.h"

namespace base {
class DiscardableMemory;
}  

#if defined(COMPILER_GCC)
namespace BASE_HASH_NAMESPACE {
template <>
struct hash<const base::DiscardableMemory*> {
  size_t operator()(const base::DiscardableMemory* ptr) const {
    return hash<size_t>()(reinterpret_cast<size_t>(ptr));
  }
};
}  
#endif  

namespace base {
namespace internal {

class BASE_EXPORT_PRIVATE DiscardableMemoryProvider {
 public:
  DiscardableMemoryProvider();
  ~DiscardableMemoryProvider();

  
  
  
  void SetDiscardableMemoryLimit(size_t bytes);

  
  void SetBytesToReclaimUnderModeratePressure(size_t bytes);

  
  void Register(const DiscardableMemory* discardable, size_t bytes);

  
  void Unregister(const DiscardableMemory* discardable);

  
  
  
  scoped_ptr<uint8, FreeDeleter> Acquire(
      const DiscardableMemory* discardable, bool* purged);

  
  
  void Release(const DiscardableMemory* discardable,
               scoped_ptr<uint8, FreeDeleter> memory);

  
  void PurgeAll();

  
  
  bool IsRegisteredForTest(const DiscardableMemory* discardable) const;

  
  
  bool CanBePurgedForTest(const DiscardableMemory* discardable) const;

  
  
  size_t GetBytesAllocatedForTest() const;

 private:
  struct Allocation {
   explicit Allocation(size_t bytes)
       : bytes(bytes),
         memory(NULL) {
   }

    size_t bytes;
    uint8* memory;
  };
  typedef HashingMRUCache<const DiscardableMemory*, Allocation> AllocationMap;

  
  void NotifyMemoryPressure(
      MemoryPressureListener::MemoryPressureLevel pressure_level);

  
  
  void Purge();

  
  
  void PurgeLRUWithLockAcquiredUntilUsageIsWithin(size_t limit);

  
  
  void EnforcePolicyWithLockAcquired();

  
  mutable Lock lock_;

  
  AllocationMap allocations_;

  
  size_t bytes_allocated_;

  
  
  size_t discardable_memory_limit_;

  
  size_t bytes_to_reclaim_under_moderate_pressure_;

  
  
  MemoryPressureListener memory_pressure_listener_;

  DISALLOW_COPY_AND_ASSIGN(DiscardableMemoryProvider);
};

}  
}  

#endif  
