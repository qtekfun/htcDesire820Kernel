// Copyright (c) 2008, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef TCMALLOC_STATIC_VARS_H_
#define TCMALLOC_STATIC_VARS_H_

#include <config.h>
#include "base/spinlock.h"
#include "central_freelist.h"
#include "common.h"
#include "page_heap.h"
#include "page_heap_allocator.h"
#include "span.h"
#include "stack_trace_table.h"

namespace tcmalloc {

class Static {
 public:
  
  static SpinLock* pageheap_lock() { return &pageheap_lock_; }

  
  static void InitStaticVars();

  
  
  static CentralFreeListPadded* central_cache() { return central_cache_; }

  static SizeMap* sizemap() { return &sizemap_; }

  
  
  

  
  static PageHeap* pageheap() { return pageheap_; }

  static PageHeapAllocator<Span>* span_allocator() { return &span_allocator_; }

  static PageHeapAllocator<StackTrace>* stacktrace_allocator() {
    return &stacktrace_allocator_;
  }

  static StackTrace* growth_stacks() { return growth_stacks_; }
  static void set_growth_stacks(StackTrace* s) { growth_stacks_ = s; }

  
  static Span* sampled_objects() { return &sampled_objects_; }
  static PageHeapAllocator<StackTraceTable::Bucket>* bucket_allocator() {
    return &bucket_allocator_;
  }

 private:
  static SpinLock pageheap_lock_;

  
  
  
  

  static SizeMap sizemap_;
  static CentralFreeListPadded central_cache_[kNumClasses];
  static PageHeapAllocator<Span> span_allocator_;
  static PageHeapAllocator<StackTrace> stacktrace_allocator_;
  static Span sampled_objects_;
  static PageHeapAllocator<StackTraceTable::Bucket> bucket_allocator_;

  
  
  
  
  static StackTrace* growth_stacks_;

  static PageHeap* pageheap_;
};

}  

#endif  
