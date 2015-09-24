// Copyright (c) 2006, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef BASE_HEAP_PROFILE_TABLE_H_
#define BASE_HEAP_PROFILE_TABLE_H_

#include "addressmap-inl.h"
#include "base/basictypes.h"
#include "base/logging.h"   

class HeapProfileTable {
 public:

  
  static const char kFileExt[];

  
  static const int kMaxStackDepth = 32;

  

  
  struct Stats {
    int32 allocs;      
    int32 frees;       
    int64 alloc_size;  
    int64 free_size;   

    
    bool Equivalent(const Stats& x) const {
      return allocs - frees == x.allocs - x.frees  &&
             alloc_size - free_size == x.alloc_size - x.free_size;
    }
  };

  
  struct AllocInfo {
    size_t object_size;  
    const void* const* call_stack;  
    int stack_depth;  
    bool live;
    bool ignored;
  };

  
  
  
  struct AllocContextInfo : public Stats {
    int stack_depth;                
    const void* const* call_stack;  
  };

  
  typedef void* (*Allocator)(size_t size);
  typedef void  (*DeAllocator)(void* ptr);

  

  HeapProfileTable(Allocator alloc, DeAllocator dealloc);
  ~HeapProfileTable();

  
  
  
  
  
  
  
  static int GetCallerStackTrace(int skip_count, void* stack[kMaxStackDepth]);

  
  
  
  void RecordAlloc(const void* ptr, size_t bytes,
                   int stack_depth, const void* const call_stack[]);

  
  void RecordFree(const void* ptr);

  
  
  bool FindAlloc(const void* ptr, size_t* object_size) const;
  
  bool FindAllocDetails(const void* ptr, AllocInfo* info) const;

  
  
  
  
  bool FindInsideAlloc(const void* ptr, size_t max_size,
                       const void** object_ptr, size_t* object_size) const;

  
  
  
  bool MarkAsLive(const void* ptr);

  
  
  
  void MarkAsIgnored(const void* ptr);

  
  
  const Stats& total() const { return total_; }

  
  
  typedef void (*AllocIterator)(const void* ptr, const AllocInfo& info);

  
  
  void IterateAllocs(AllocIterator callback) const {
    alloc_address_map_->Iterate(MapArgsAllocIterator, callback);
  }

  
  typedef void (*AllocContextIterator)(const AllocContextInfo& info);

  
  
  
  void IterateOrderedAllocContexts(AllocContextIterator callback) const;

  
  
  
  
  
  int FillOrderedProfile(char buf[], int size) const;

  
  static void CleanupOldProfiles(const char* prefix);

  
  
  
  
  class Snapshot;
  Snapshot* TakeSnapshot();

  
  
  void ReleaseSnapshot(Snapshot* snapshot);

  
  
  
  
  Snapshot* NonLiveSnapshot(Snapshot* base);

  
  
  
  void RefreshMMapData();

  
  
  
  void ClearMMapData();

 private:

  

  
  
  struct Bucket : public Stats {
    uintptr_t    hash;   
    int          depth;  
    const void** stack;  
    Bucket*      next;   
  };

  
  struct AllocValue {
    
    Bucket* bucket() const {
      return reinterpret_cast<Bucket*>(bucket_rep & ~uintptr_t(kMask));
    }
    
    void set_bucket(Bucket* b) { bucket_rep = reinterpret_cast<uintptr_t>(b); }
    size_t  bytes;   

    
    bool live() const { return bucket_rep & kLive; }
    void set_live(bool l) {
      bucket_rep = (bucket_rep & ~uintptr_t(kLive)) | (l ? kLive : 0);
    }

    
    bool ignore() const { return bucket_rep & kIgnore; }
    void set_ignore(bool r) {
      bucket_rep = (bucket_rep & ~uintptr_t(kIgnore)) | (r ? kIgnore : 0);
    }

   private:
    
    
    static const int kLive = 1;
    static const int kIgnore = 2;
    static const int kMask = kLive | kIgnore;

    uintptr_t bucket_rep;
  };

  
  static size_t AllocValueSize(const AllocValue& v) { return v.bytes; }

  typedef AddressMap<AllocValue> AllocationMap;

  
  struct DumpArgs {
    RawFD fd;  
    Stats* profile_stats;  

    DumpArgs(RawFD a, Stats* d)
      : fd(a), profile_stats(d) { }
  };

  

  
  
  
  
  
  
  
  
  
  
  
  static int UnparseBucket(const Bucket& b,
                           char* buf, int buflen, int bufsize,
                           const char* extra,
                           Stats* profile_stats);

  
  void DeallocateAllocationMap(AllocationMap* allocation);

  
  void DeallocateBucketTable(Bucket** table);

  
  
  
  
  Bucket* GetBucket(int depth, const void* const key[], Bucket** table,
                    int* bucket_count);

  
  
  static void MapArgsAllocIterator(const void* ptr, AllocValue* v,
                                   AllocIterator callback) {
    AllocInfo info;
    info.object_size = v->bytes;
    info.call_stack = v->bucket()->stack;
    info.stack_depth = v->bucket()->depth;
    info.live = v->live();
    info.ignored = v->ignore();
    callback(ptr, info);
  }

  
  
  inline static void DumpNonLiveIterator(const void* ptr, AllocValue* v,
                                         const DumpArgs& args);

  
  inline static void ZeroBucketCountsIterator(
      const void* ptr, AllocValue* v, HeapProfileTable* heap_profile);

  
  
  
  
  Bucket** MakeSortedBucketList() const;

  
  static void AddToSnapshot(const void* ptr, AllocValue* v, Snapshot* s);

  
  struct AddNonLiveArgs {
    Snapshot* dest;
    Snapshot* base;
  };

  
  
  static void AddIfNonLive(const void* ptr, AllocValue* v,
                           AddNonLiveArgs* arg);

  
  
  
  static bool WriteProfile(const char* file_name,
                           const Bucket& total,
                           AllocationMap* allocations);

  

  
  Allocator alloc_;
  DeAllocator dealloc_;

  
  
  
  Bucket total_;

  
  // We hand-craft one instead of using one of the pre-written
  
  
  Bucket** alloc_table_;
  int num_alloc_buckets_;

  
  
  
  Bucket** mmap_table_;
  int num_available_mmap_buckets_;

  
  AllocationMap* alloc_address_map_;
  AllocationMap* mmap_address_map_;

  DISALLOW_COPY_AND_ASSIGN(HeapProfileTable);
};

class HeapProfileTable::Snapshot {
 public:
  const Stats& total() const { return total_; }

  
  
  
  
  
  
  void ReportLeaks(const char* checker_name, const char* filename,
                   bool should_symbolize);

  
  
  void ReportIndividualObjects();

  bool Empty() const {
    return (total_.allocs == 0) && (total_.alloc_size == 0);
  }

 private:
  friend class HeapProfileTable;

  
  Bucket total_;

  
  
  AllocationMap map_;

  Snapshot(Allocator alloc, DeAllocator dealloc) : map_(alloc, dealloc) {
    memset(&total_, 0, sizeof(total_));
  }

  
  
  inline void Add(const void* ptr, const AllocValue& v) {
    map_.Insert(ptr, v);
    total_.allocs++;
    total_.alloc_size += v.bytes;
  }

  
  struct Entry;
  struct ReportState;
  static void ReportCallback(const void* ptr, AllocValue* v, ReportState*);
  static void ReportObject(const void* ptr, AllocValue* v, char*);

  DISALLOW_COPY_AND_ASSIGN(Snapshot);
};

#endif  
