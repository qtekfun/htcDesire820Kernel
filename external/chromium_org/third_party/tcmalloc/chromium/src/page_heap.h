// Copyright (c) 2008, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef TCMALLOC_PAGE_HEAP_H_
#define TCMALLOC_PAGE_HEAP_H_

#include <config.h>
#include <stddef.h>                     
#ifdef HAVE_STDINT_H
#include <stdint.h>                     
#endif
#include <gperftools/malloc_extension.h>
#include "base/basictypes.h"
#include "common.h"
#include "packed-cache-inl.h"
#include "pagemap.h"
#include "span.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#ifdef NO_TCMALLOC_SAMPLES
  
# define GetStackTrace(stack, depth, skip)  (0)
#else
# include <gperftools/stacktrace.h>
#endif

namespace base {
struct MallocRange;
}

namespace tcmalloc {



template <int BITS> class MapSelector {
 public:
  typedef TCMalloc_PageMap3<BITS-kPageShift> Type;
  typedef PackedCache<BITS-kPageShift, uint64_t> CacheType;
};

template <> class MapSelector<32> {
 public:
#ifdef WIN32
  typedef TCMalloc_PageMap1_LazyCommit<32-kPageShift> Type;
#else
  
  typedef TCMalloc_PageMap2<32-kPageShift> Type;
#endif
  typedef PackedCache<32-kPageShift, uint16_t> CacheType;
};


class PERFTOOLS_DLL_DECL PageHeap {
 public:
  PageHeap();

  
  
  
  Span* New(Length n);

  
  
  
  void Delete(Span* span);

  
  
  
  
  void RegisterSizeClass(Span* span, size_t sc);

  
  
  
  
  
  
  
  
  Span* Split(Span* span, Length n);

  
  
  inline Span* GetDescriptor(PageID p) const {
    return reinterpret_cast<Span*>(pagemap_.get(p));
  }

  
  
  bool GetNextRange(PageID start, base::MallocRange* r);

  
  struct Stats {
    Stats() : system_bytes(0), free_bytes(0), unmapped_bytes(0) {}
    uint64_t system_bytes;    
    uint64_t free_bytes;      
    uint64_t unmapped_bytes;  
    uint64_t committed_bytes;  

  };
  inline Stats stats() const { return stats_; }

  struct SmallSpanStats {
    
    
    int64 normal_length[kMaxPages];
    int64 returned_length[kMaxPages];
  };
  void GetSmallSpanStats(SmallSpanStats* result);

  
  struct LargeSpanStats {
    int64 spans;           
    int64 normal_pages;    
    int64 returned_pages;  
  };
  void GetLargeSpanStats(LargeSpanStats* result);

  bool Check();
  
  bool CheckExpensive();
  bool CheckList(Span* list, Length min_pages, Length max_pages,
                 int freelist);  

  
  
  
  
  
  
  Length ReleaseAtLeastNPages(Length num_pages);

  
  
  
  
  
  size_t GetSizeClassIfCached(PageID p) const {
    return pagemap_cache_.GetOrDefault(p, 0);
  }
  void CacheSizeClass(PageID p, size_t cl) const { pagemap_cache_.Put(p, cl); }

 private:
  
  
  static const size_t kPageMapBigAllocationThreshold = 128 << 20;

  
  
  
  
  
  
  
  static const int kMinSystemAlloc = kMaxPages;

  
  
  
  
  static const int kMaxReleaseDelay = 1 << 14;

  
  
  
  static const int kDefaultReleaseDelay = 1 << 12;

  
  typedef MapSelector<kAddressBits>::Type PageMap;
  typedef MapSelector<kAddressBits>::CacheType PageMapCache;
  PageMap pagemap_;
  mutable PageMapCache pagemap_cache_;

  
  
  
  struct SpanList {
    Span        normal;
    Span        returned;
  };

  
  SpanList large_;

  
  SpanList free_[kMaxPages];

  
  Stats stats_;
  Span* SearchFreeAndLargeLists(Length n);

  bool GrowHeap(Length n);

  
  
  
  
  
  
  
  Span* Carve(Span* span, Length n);

  void RecordSpan(Span* span) {
    pagemap_.set(span->start, span);
    if (span->length > 1) {
      pagemap_.set(span->start + span->length - 1, span);
    }
  }

  
  
  Span* AllocLarge(Length n);

  
  
  void MergeIntoFreeList(Span* span);

  
  void CommitSpan(Span* span);

  
  void DecommitSpan(Span* span);

  
  void PrependToFreeList(Span* span);

  
  void RemoveFromFreeList(Span* span);

  
  
  void IncrementalScavenge(Length n);

  
  
  Length ReleaseLastNormalSpan(SpanList* slist);


  
  int64_t scavenge_counter_;

  
  int release_index_;
};

}  

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif  
