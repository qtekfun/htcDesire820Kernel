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
 *
 * ---
 * Author: Maxim Lifantsev
 */

#ifndef BASE_MEMORY_REGION_MAP_H_
#define BASE_MEMORY_REGION_MAP_H_

#include <config.h>

#ifdef HAVE_PTHREAD
#include <pthread.h>
#endif
#include <stddef.h>
#include <set>
#include "base/stl_allocator.h"
#include "base/spinlock.h"
#include "base/thread_annotations.h"
#include "base/low_level_alloc.h"
#include "heap-profile-stats.h"


class MemoryRegionMap {
 private:
  
  
  
  
  static const int kMaxStackDepth = 32;

  
  
  static const int kHashTableSize = 179999;

 public:
  

  
  
  
  
  
  

  
  
  
  
  
  
  
  
  
  
  
  
  static void Init(int max_stack_depth, bool use_buckets);

  
  
  
  
  static bool Shutdown();

  
  
  static bool IsRecordingLocked();

  
  
  
  static void Lock() EXCLUSIVE_LOCK_FUNCTION(lock_);
  static void Unlock() UNLOCK_FUNCTION(lock_);

  
  static bool LockIsHeld();

  
  
  class LockHolder {
   public:
    LockHolder() { Lock(); }
    ~LockHolder() { Unlock(); }
   private:
    DISALLOW_COPY_AND_ASSIGN(LockHolder);
  };

  
  
  
  struct Region {
    uintptr_t start_addr;  
    uintptr_t end_addr;  
    int call_stack_depth;  
    const void* call_stack[kMaxStackDepth];  
                                             
    bool is_stack;  
                    

    
    
    
    
    
    
    
    uintptr_t caller() const {
      return reinterpret_cast<uintptr_t>(call_stack_depth >= 1
                                         ? call_stack[0] : NULL);
    }

    
    bool Overlaps(const Region& x) const {
      return start_addr < x.end_addr  &&  end_addr > x.start_addr;
    }

   private:  
    friend class MemoryRegionMap;

    
    void Create(const void* start, size_t size) {
      start_addr = reinterpret_cast<uintptr_t>(start);
      end_addr = start_addr + size;
      is_stack = false;  
      call_stack_depth = 0;
      AssertIsConsistent();
    }
    void set_call_stack_depth(int depth) {
      RAW_DCHECK(call_stack_depth == 0, "");  
      call_stack_depth = depth;
      AssertIsConsistent();
    }

    
    void set_is_stack() { is_stack = true; }
    void set_start_addr(uintptr_t addr) {
      start_addr = addr;
      AssertIsConsistent();
    }
    void set_end_addr(uintptr_t addr) {
      end_addr = addr;
      AssertIsConsistent();
    }

    
    void AssertIsConsistent() const {
      RAW_DCHECK(start_addr < end_addr, "");
      RAW_DCHECK(call_stack_depth >= 0  &&
                 call_stack_depth <= kMaxStackDepth, "");
    }

    
    
    void SetRegionSetKey(uintptr_t addr) {
      
      if (DEBUG_MODE) memset(this, 0xFF, sizeof(*this));
      end_addr = addr;
    }

    
    
    
    
    
    
    
    
    
  };

  
  
  
  
  static bool FindRegion(uintptr_t addr, Region* result);

  
  
  
  
  
  static bool FindAndMarkStackRegion(uintptr_t stack_top, Region* result);

  
  
  template<class Type>
  static void IterateBuckets(void (*callback)(const HeapProfileBucket*, Type),
                             Type arg);

  
  
  
  
  static HeapProfileBucket* GetBucket(int depth, const void* const key[]);

 private:  

  
  struct RegionCmp {
    bool operator()(const Region& x, const Region& y) const {
      return x.end_addr < y.end_addr;
    }
  };

  
  struct MyAllocator {
    static void *Allocate(size_t n) {
      return LowLevelAlloc::AllocWithArena(n, arena_);
    }
    static void Free(const void *p, size_t ) {
      LowLevelAlloc::Free(const_cast<void*>(p));
    }
  };

  
  typedef std::set<Region, RegionCmp,
              STL_Allocator<Region, MyAllocator> > RegionSet;

 public:  

  
  typedef RegionSet::const_iterator RegionIterator;

  
  
  
  
  
  
  
  static RegionIterator BeginRegionLocked();
  static RegionIterator EndRegionLocked();

  
  static int64 MapSize() { return map_size_; }
  static int64 UnmapSize() { return unmap_size_; }

  
  
  union RegionSetRep;

 private:
  

  
  static int client_count_;

  
  static int max_stack_depth_;

  
  static LowLevelAlloc::Arena* arena_;

  
  
  
  
  
  
  static RegionSet* regions_;

  
  static SpinLock lock_;
  
  static SpinLock owner_lock_;

  
  static int recursion_count_;
  
  static pthread_t lock_owner_tid_;

  
  static int64 map_size_;
  
  static int64 unmap_size_;

  
  static HeapProfileBucket** bucket_table_ GUARDED_BY(lock_);
  static int num_buckets_ GUARDED_BY(lock_);

  
  
  
  
  
  
  
  

  
  static int saved_buckets_count_ GUARDED_BY(lock_);

  
  
  
  
  static HeapProfileBucket saved_buckets_[20] GUARDED_BY(lock_);

  static const void* saved_buckets_keys_[20][kMaxStackDepth] GUARDED_BY(lock_);

  

  
  
  static const Region* DoFindRegionLocked(uintptr_t addr);

  
  
  inline static void DoInsertRegionLocked(const Region& region);
  
  
  inline static void HandleSavedRegionsLocked(
                       void (*insert_func)(const Region& region));

  
  
  static void RestoreSavedBucketsLocked();

  
  inline static void InitRegionSetLocked();

  
  
  inline static void InsertRegionLocked(const Region& region);

  
  
  static void RecordRegionAddition(const void* start, size_t size);
  
  
  static void RecordRegionRemoval(const void* start, size_t size);

  
  
  
  static void RecordRegionRemovalInBucket(int depth,
                                          const void* const key[],
                                          size_t size);

  
  static void MmapHook(const void* result,
                       const void* start, size_t size,
                       int prot, int flags,
                       int fd, off_t offset);
  static void MunmapHook(const void* ptr, size_t size);
  static void MremapHook(const void* result, const void* old_addr,
                         size_t old_size, size_t new_size, int flags,
                         const void* new_addr);
  static void SbrkHook(const void* result, ptrdiff_t increment);

  
  
  static void LogAllLocked();

  DISALLOW_COPY_AND_ASSIGN(MemoryRegionMap);
};

template <class Type>
void MemoryRegionMap::IterateBuckets(
    void (*callback)(const HeapProfileBucket*, Type), Type callback_arg) {
  for (int index = 0; index < kHashTableSize; index++) {
    for (HeapProfileBucket* bucket = bucket_table_[index];
         bucket != NULL;
         bucket = bucket->next) {
      callback(bucket, callback_arg);
    }
  }
}

#endif  
