// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#ifndef BASE_DEEP_HEAP_PROFILE_H_
#define BASE_DEEP_HEAP_PROFILE_H_

#include "config.h"

#if defined(TYPE_PROFILING)
#include <typeinfo>
#endif

#if defined(__linux__) || defined(_WIN32) || defined(_WIN64)
#define USE_DEEP_HEAP_PROFILE 1
#endif

#include "addressmap-inl.h"
#include "heap-profile-table.h"
#include "memory_region_map.h"

class DeepHeapProfile {
 public:
  enum PageFrameType {
    DUMP_NO_PAGEFRAME = 0,  
    DUMP_PFN = 1,           
    DUMP_PAGECOUNT = 2,     
  };

  
  
  
  
  
  
  
  
  DeepHeapProfile(HeapProfileTable* heap_profile,
                  const char* prefix,
                  enum PageFrameType pageframe_type);
  ~DeepHeapProfile();

  
  
  
  
  void DumpOrderedProfile(const char* reason,
                          char raw_buffer[],
                          int buffer_size,
                          RawFD fd);

 private:
#ifdef USE_DEEP_HEAP_PROFILE
  typedef HeapProfileTable::Stats Stats;
  typedef HeapProfileTable::Bucket Bucket;
  typedef HeapProfileTable::AllocValue AllocValue;
  typedef HeapProfileTable::AllocationMap AllocationMap;

  enum MapsRegionType {
    
    
    ABSENT,

    
    
    ANONYMOUS,

    
    
    FILE_EXEC,
    FILE_NONEXEC,

    
    STACK,

    
    
    
    OTHER,

    NUMBER_OF_MAPS_REGION_TYPES
  };

  static const char* kMapsRegionTypeDict[NUMBER_OF_MAPS_REGION_TYPES];

  
  class TextBuffer {
   public:
    TextBuffer(char *raw_buffer, int size, RawFD fd)
        : buffer_(raw_buffer),
          size_(size),
          cursor_(0),
          fd_(fd) {
    }

    int Size();
    int FilledBytes();
    void Clear();
    void Flush();

    bool AppendChar(char value);
    bool AppendString(const char* value, int width);
    bool AppendInt(int value, int width, bool leading_zero);
    bool AppendLong(long value, int width);
    bool AppendUnsignedLong(unsigned long value, int width);
    bool AppendInt64(int64 value, int width);
    bool AppendBase64(uint64 value, int width);
    bool AppendPtr(uint64 value, int width);

   private:
    bool ForwardCursor(int appended);

    char *buffer_;
    int size_;
    int cursor_;
    RawFD fd_;
    DISALLOW_COPY_AND_ASSIGN(TextBuffer);
  };

  
  class MemoryResidenceInfoGetterInterface {
   public:
    virtual ~MemoryResidenceInfoGetterInterface();

    
    virtual void Initialize() = 0;

    
    
    virtual size_t CommittedSize(uint64 first_address,
                                 uint64 last_address,
                                 TextBuffer* buffer) const = 0;

    
    static MemoryResidenceInfoGetterInterface* Create(
        PageFrameType pageframe_type);

    virtual bool IsPageCountAvailable() const = 0;

   protected:
    MemoryResidenceInfoGetterInterface();
  };

#if defined(_WIN32) || defined(_WIN64)
  
  class MemoryInfoGetterWindows : public MemoryResidenceInfoGetterInterface {
   public:
    MemoryInfoGetterWindows(PageFrameType) {}
    virtual ~MemoryInfoGetterWindows() {}

    virtual void Initialize();

    virtual size_t CommittedSize(uint64 first_address,
                                 uint64 last_address,
                                 TextBuffer* buffer) const;

    virtual bool IsPageCountAvailable() const;
  };
#endif  

#if defined(__linux__)
  
  class MemoryInfoGetterLinux : public MemoryResidenceInfoGetterInterface {
   public:
    MemoryInfoGetterLinux(PageFrameType pageframe_type)
        : pageframe_type_(pageframe_type),
          pagemap_fd_(kIllegalRawFD),
          kpagecount_fd_(kIllegalRawFD) {}
    virtual ~MemoryInfoGetterLinux() {}

    
    
    
    
    virtual void Initialize();

    
    
    virtual size_t CommittedSize(uint64 first_address,
                                 uint64 last_address,
                                 TextBuffer* buffer) const;

    virtual bool IsPageCountAvailable() const;

   private:
    struct State {
      uint64 pfn;
      bool is_committed;  
      bool is_present;
      bool is_swapped;
      bool is_shared;
      bool is_mmap;
    };

    uint64 ReadPageCount(uint64 pfn) const;

    
    
    bool Seek(uint64 address) const;

    
    
    bool Read(State* state, bool get_pfn) const;

    PageFrameType pageframe_type_;
    RawFD pagemap_fd_;
    RawFD kpagecount_fd_;
  };
#endif  

  
  
  
  struct DeepBucket {
   public:
    void UnparseForStats(TextBuffer* buffer);
    void UnparseForBucketFile(TextBuffer* buffer);

    Bucket* bucket;
#if defined(TYPE_PROFILING)
    const std::type_info* type;  
#endif
    size_t committed_size;  
    bool is_mmap;  
    int id;  
    bool is_logged;  
    DeepBucket* next;  
  };

  
  class DeepBucketTable {
   public:
    DeepBucketTable(int size,
                    HeapProfileTable::Allocator alloc,
                    HeapProfileTable::DeAllocator dealloc);
    ~DeepBucketTable();

    
    
    DeepBucket* Lookup(Bucket* bucket,
#if defined(TYPE_PROFILING)
                       const std::type_info* type,
#endif
                       bool is_mmap);

    
    void UnparseForStats(TextBuffer* buffer);

    
    void WriteForBucketFile(const char* prefix,
                            int dump_count,
                            char raw_buffer[],
                            int buffer_size);

    
    void ResetCommittedSize();

    
    void ResetIsLogged();

   private:
    
    inline static void AddToHashValue(uintptr_t add, uintptr_t* hash_value);
    inline static void FinishHashValue(uintptr_t* hash_value);

    DeepBucket** table_;
    size_t table_size_;
    HeapProfileTable::Allocator alloc_;
    HeapProfileTable::DeAllocator dealloc_;
    int bucket_id_;
  };

  class RegionStats {
   public:
    RegionStats(): virtual_bytes_(0), committed_bytes_(0) {}
    ~RegionStats() {}

    
    void Initialize();

    
    
    
    uint64 Record(
        const MemoryResidenceInfoGetterInterface* memory_residence_info_getter,
        uint64 first_address,
        uint64 last_address,
        TextBuffer* buffer);

    
    void Unparse(const char* name, TextBuffer* buffer);

    size_t virtual_bytes() const { return virtual_bytes_; }
    size_t committed_bytes() const { return committed_bytes_; }
    void AddToVirtualBytes(size_t additional_virtual_bytes) {
      virtual_bytes_ += additional_virtual_bytes;
    }
    void AddToCommittedBytes(size_t additional_committed_bytes) {
      committed_bytes_ += additional_committed_bytes;
    }
    void AddAnotherRegionStat(const RegionStats& other) {
      virtual_bytes_ += other.virtual_bytes_;
      committed_bytes_ += other.committed_bytes_;
    }

   private:
    size_t virtual_bytes_;
    size_t committed_bytes_;
    DISALLOW_COPY_AND_ASSIGN(RegionStats);
  };

  class GlobalStats {
   public:
    
    void SnapshotMaps(
        const MemoryResidenceInfoGetterInterface* memory_residence_info_getter,
        DeepHeapProfile* deep_profile,
        TextBuffer* mmap_dump_buffer);

    
    void SnapshotAllocations(DeepHeapProfile* deep_profile);

    
    void Unparse(TextBuffer* buffer);

  private:
    
    
    static void RecordAlloc(const void* pointer,
                            AllocValue* alloc_value,
                            DeepHeapProfile* deep_profile);

    DeepBucket* GetInformationOfMemoryRegion(
        const MemoryRegionMap::RegionIterator& mmap_iter,
        const MemoryResidenceInfoGetterInterface* memory_residence_info_getter,
        DeepHeapProfile* deep_profile);

    
    
    
    
    RegionStats all_[NUMBER_OF_MAPS_REGION_TYPES];

    RegionStats unhooked_[NUMBER_OF_MAPS_REGION_TYPES];

    
    RegionStats profiled_malloc_;

    
    RegionStats profiled_mmap_;
  };

  
  
  static void WriteProcMaps(const char* prefix,
                            char raw_buffer[],
                            int buffer_size);

  
  bool AppendCommandLine(TextBuffer* buffer);

  MemoryResidenceInfoGetterInterface* memory_residence_info_getter_;

  
  pid_t most_recent_pid_;

  GlobalStats stats_;      
  int dump_count_;         
  char* filename_prefix_;  
  char run_id_[128];

  DeepBucketTable deep_table_;

  enum PageFrameType pageframe_type_;
#endif  

  HeapProfileTable* heap_profile_;

  DISALLOW_COPY_AND_ASSIGN(DeepHeapProfile);
};

#endif  
