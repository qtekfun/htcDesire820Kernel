// Copyright (c) 2008, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef TCMALLOC_CENTRAL_FREELIST_H_
#define TCMALLOC_CENTRAL_FREELIST_H_

#include "config.h"
#include <stddef.h>                     
#ifdef HAVE_STDINT_H
#include <stdint.h>                     
#endif
#include "base/spinlock.h"
#include "base/thread_annotations.h"
#include "common.h"
#include "span.h"

namespace tcmalloc {

class CentralFreeList {
 public:
  
  
  
  CentralFreeList() : lock_(base::LINKER_INITIALIZED) { }

  void Init(size_t cl);

  

  
  
  void InsertRange(void *start, void *end, int N);

  
  int RemoveRange(void **start, void **end, int N);

  
  int length() {
    SpinLockHolder h(&lock_);
    return counter_;
  }

  
  int tc_length();

  
  
  
  
  size_t OverheadBytes();

 private:
  
  
  
  struct TCEntry {
    void *head;  
    void *tail;  
  };

  
  
#ifdef TCMALLOC_SMALL_BUT_SLOW
  
  static const int kMaxNumTransferEntries = 0;
#else
  
  
  
  static const int kMaxNumTransferEntries = 64;
#endif

  
  
  
  void* FetchFromSpans() EXCLUSIVE_LOCKS_REQUIRED(lock_);

  
  
  
  
  void* FetchFromSpansSafe() EXCLUSIVE_LOCKS_REQUIRED(lock_);

  
  
  
  void ReleaseListToSpans(void *start) EXCLUSIVE_LOCKS_REQUIRED(lock_);

  
  
  
  void ReleaseToSpans(void* object) EXCLUSIVE_LOCKS_REQUIRED(lock_);

  
  
  
  void Populate() EXCLUSIVE_LOCKS_REQUIRED(lock_);

  
  
  
  
  bool MakeCacheSpace() EXCLUSIVE_LOCKS_REQUIRED(lock_);

  
  
  
  
  
  static bool EvictRandomSizeClass(int locked_size_class, bool force);

  
  
  
  
  
  
  
  bool ShrinkCache(int locked_size_class, bool force) LOCKS_EXCLUDED(lock_);

  
  
  SpinLock lock_;

  
  size_t   size_class_;     
  Span     empty_;          
  Span     nonempty_;       
  size_t   num_spans_;      
  size_t   counter_;        

  
  
  
  
  
  TCEntry tc_slots_[kMaxNumTransferEntries];

  
  
  int32_t used_slots_;
  
  
  
  int32_t cache_size_;
  
  int32_t max_cache_size_;
};

template<int kFreeListSizeMod64>
class CentralFreeListPaddedTo : public CentralFreeList {
 private:
  char pad_[64 - kFreeListSizeMod64];
};

template<>
class CentralFreeListPaddedTo<0> : public CentralFreeList {
};

class CentralFreeListPadded : public CentralFreeListPaddedTo<
  sizeof(CentralFreeList) % 64> {
};

}  

#endif  
