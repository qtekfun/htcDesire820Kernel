// Copyright (c) 2008, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef TCMALLOC_THREAD_CACHE_H_
#define TCMALLOC_THREAD_CACHE_H_

#include <config.h>
#ifdef HAVE_PTHREAD
#include <pthread.h>                    
#endif
#include <stddef.h>                     
#ifdef HAVE_STDINT_H
#include <stdint.h>                     
#endif
#include <sys/types.h>                  
#include "common.h"            
#include "free_list.h"  
#include "internal_logging.h"  
#include "maybe_threads.h"
#include "page_heap_allocator.h"  
#include "sampler.h"           
#include "static_vars.h"       

namespace tcmalloc {

#if defined(HAVE_TLS)
extern bool kernel_supports_tls;   
void CheckIfKernelSupportsTLS();
inline bool KernelSupportsTLS() {
  return kernel_supports_tls;
}
#endif    


class ThreadCache {
 public:
  
  ThreadCache* next_;
  ThreadCache* prev_;

  void Init(pthread_t tid);
  void Cleanup();

  
  int freelist_length(size_t cl) const { return list_[cl].length(); }

  
  size_t Size() const { return size_; }

  
  
  void* Allocate(size_t size, size_t cl);
  void Deallocate(void* ptr, size_t size_class);

  void Scavenge();

  int GetSamplePeriod();

  
  
  bool SampleAllocation(size_t k);

  
  void AddToByteAllocatedTotal(size_t k) { total_bytes_allocated_ += k; }

  
  
  
  uint32 GetTotalBytesAllocated() const;

  
  static uint32 GetBytesAllocatedOnCurrentThread();

  static void         InitModule();
  static void         InitTSD();
  static ThreadCache* GetThreadHeap();
  static ThreadCache* GetCache();
  static ThreadCache* GetCacheIfPresent();
  static ThreadCache* CreateCacheIfNecessary();
  static void         BecomeIdle();

  
  static inline int HeapsInUse();

  
  
  
  
  
  static void GetThreadStats(uint64_t* total_bytes, uint64_t* class_count);

  
  
  
  static void set_overall_thread_cache_size(size_t new_size);
  static size_t overall_thread_cache_size() {
    return overall_thread_cache_size_;
  }

 private:
  class FreeList {
   private:
    void*    list_;       

#ifdef _LP64
    
    uint32_t length_;      
    uint32_t lowater_;     
    uint32_t max_length_;  
    
    
    
    uint32_t length_overages_;
#else
    
    uint16_t length_;
    uint16_t lowater_;
    uint16_t max_length_;
    uint16_t length_overages_;
#endif

   public:
    void Init() {
      list_ = NULL;
      length_ = 0;
      lowater_ = 0;
      max_length_ = 1;
      length_overages_ = 0;
    }

    
    size_t length() const {
      return length_;
    }

    
    size_t max_length() const {
      return max_length_;
    }

    
    
    void set_max_length(size_t new_max) {
      max_length_ = new_max;
    }

    
    size_t length_overages() const {
      return length_overages_;
    }

    void set_length_overages(size_t new_count) {
      length_overages_ = new_count;
    }

    
    bool empty() const {
      return list_ == NULL;
    }

    
    int lowwatermark() const { return lowater_; }
    void clear_lowwatermark() { lowater_ = length_; }

    void Push(void* ptr) {
      FL_Push(&list_, ptr);
      length_++;
    }

    void* Pop() {
      ASSERT(list_ != NULL);
      length_--;
      if (length_ < lowater_) lowater_ = length_;
      return FL_Pop(&list_);
    }

    void* Next() {
      if (list_ == NULL) return NULL;
      return FL_Next(list_);
    }

    void PushRange(int N, void *start, void *end) {
      FL_PushRange(&list_, start, end);
      length_ += N;
    }

    void PopRange(int N, void **start, void **end) {
      FL_PopRange(&list_, N, start, end);
      ASSERT(length_ >= N);
      length_ -= N;
      if (length_ < lowater_) lowater_ = length_;
    }
  };

  
  
  void* FetchFromCentralCache(size_t cl, size_t byte_size);

  
  
  void ListTooLong(FreeList* src, size_t cl);

  
  void ReleaseToCentralCache(FreeList* src, size_t cl, int N);

  
  
  
  void IncreaseCacheLimit();
  
  void IncreaseCacheLimitLocked();

  
  
  
  
  
  
  
  
  
  
  
  
#ifdef HAVE_TLS
  static __thread ThreadCache* threadlocal_heap_
  
  
  
  
  
  
  
  
  
#if defined(HAVE___ATTRIBUTE__) && !defined(PGO_GENERATE)
   __attribute__ ((tls_model ("initial-exec")))
# endif
   ;
#endif

  
  
  
  
  
  static bool tsd_inited_;
  static pthread_key_t heap_key_;

  
  static ThreadCache* thread_heaps_;
  static int thread_heap_count_;

  
  
  
  
  static ThreadCache* next_memory_steal_;

  
  static size_t overall_thread_cache_size_;

  
  
  // fine as long as size_t can be written atomically and we don't place
  
  static volatile size_t per_thread_cache_size_;

  
  
  static ssize_t unclaimed_cache_space_;

  
  

  size_t        size_;                  
  size_t        max_size_;              

  
  
  
  
  
  
  uint32        total_bytes_allocated_;  

  
  Sampler       sampler_;               

  FreeList      list_[kNumClasses];     

  pthread_t     tid_;                   
  bool          in_setspecific_;        

  
  static ThreadCache* NewHeap(pthread_t tid);

  
  static void DestroyThreadCache(void* ptr);

  static void DeleteCache(ThreadCache* heap);
  static void RecomputePerThreadCacheSize();

  
  
  
} CACHELINE_ALIGNED;

extern PageHeapAllocator<ThreadCache> threadcache_allocator;

inline int ThreadCache::HeapsInUse() {
  return threadcache_allocator.inuse();
}

inline bool ThreadCache::SampleAllocation(size_t k) {
  return sampler_.SampleAllocation(k);
}

inline uint32 ThreadCache::GetTotalBytesAllocated() const {
  return total_bytes_allocated_;
}

inline void* ThreadCache::Allocate(size_t size, size_t cl) {
  ASSERT(size <= kMaxSize);
  ASSERT(size == Static::sizemap()->ByteSizeForClass(cl));

  FreeList* list = &list_[cl];
  if (list->empty()) {
    return FetchFromCentralCache(cl, size);
  }
  size_ -= size;
  return list->Pop();
}

inline void ThreadCache::Deallocate(void* ptr, size_t cl) {
  FreeList* list = &list_[cl];
  size_ += Static::sizemap()->ByteSizeForClass(cl);
  ssize_t size_headroom = max_size_ - size_ - 1;

  
  
  
  ASSERT(ptr != list->Next());

  list->Push(ptr);
  ssize_t list_headroom =
      static_cast<ssize_t>(list->max_length()) - list->length();

  
  
  
  if ((list_headroom | size_headroom) < 0) {
    if (list_headroom < 0) {
      ListTooLong(list, cl);
    }
    if (size_ >= max_size_) Scavenge();
  }
}

inline ThreadCache* ThreadCache::GetThreadHeap() {
#ifdef HAVE_TLS
  
  if (KernelSupportsTLS())
    return threadlocal_heap_;
#endif
  return reinterpret_cast<ThreadCache *>(
      perftools_pthread_getspecific(heap_key_));
}

inline ThreadCache* ThreadCache::GetCache() {
  ThreadCache* ptr = NULL;
  if (!tsd_inited_) {
    InitModule();
  } else {
    ptr = GetThreadHeap();
  }
  if (ptr == NULL) ptr = CreateCacheIfNecessary();
  return ptr;
}

inline ThreadCache* ThreadCache::GetCacheIfPresent() {
  if (!tsd_inited_) return NULL;
  return GetThreadHeap();
}

}  

#endif  
